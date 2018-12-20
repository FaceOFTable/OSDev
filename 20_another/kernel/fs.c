// https://wiki.osdev.org/ATA_PIO_Mode

// SRST
int ata_soft_reset(int devctl) {

    int i;

    IoWrite8(devctl, 4); // do a "software reset" on the bus
    IoWrite8(devctl, 0); // reset the bus to normal operation

    IoRead8(devctl);
    IoRead8(devctl);
    IoRead8(devctl);
    IoRead8(devctl);

    // want BSY clear and RDY set
    for (i = 0; i < 4096; i++) {
        if ((IoRead8(devctl) & 0xC0) == 0x40)
            return 0;
    }

    return 1;
}

// Выбор устройства для работы
void drive_select(int slavebit, struct DEVICE* ctrl) {

    // Выбор устройства (primary | slave) | 0x40=Set LBA Bit
    IoWrite8(ctrl->base + REG_DEVSEL, 0xA0 | 0x40 | slavebit << 4);

    // Ожидать 400ns, пока драйв включится в работу
    IoRead8(ctrl->dev_ctl);
	IoRead8(ctrl->dev_ctl);
	IoRead8(ctrl->dev_ctl);
	IoRead8(ctrl->dev_ctl);
}

/* Primary bus:
 * ctrl->base    = 0x1F0
 * ctrl->dev_ctl = 0x3F6
 */
int detect_devtype(int slavebit, struct DEVICE* ctrl) {

    /* Ждать, пока девай будет готов */
    if (ata_soft_reset(ctrl->dev_ctl)) {
        return ATADEV_UNKNOWN;
    }

    // Выбор устройства (primary | slave)
    drive_select(slavebit, ctrl);

    // Получение битов сигнатуры
	unsigned cl = IoRead8(ctrl->base + REG_CYL_LO);
	unsigned ch = IoRead8(ctrl->base + REG_CYL_HI);

	// Различение ATA, ATAPI, SATA и SATAPI
	if (cl == 0x14 && ch == 0xEB) return ATADEV_PATAPI;
	if (cl == 0x69 && ch == 0x96) return ATADEV_SATAPI;
	if (cl == 0x00 && ch == 0x00) return ATADEV_PATA;
	if (cl == 0x3c && ch == 0xc3) return ATADEV_SATA;

	return ATADEV_UNKNOWN;
}

// Куда читать сектор
void drive_pio_read(int base, uint8_t* address) {

    __asm__ __volatile__("pushl %%ecx" ::: "ecx");
    __asm__ __volatile__("pushl %%edx" ::: "edx");
    __asm__ __volatile__("pushl %%edi" ::: "edi");
    __asm__ __volatile__("movl  $0x100, %%ecx" ::: "ecx");
    __asm__ __volatile__("movl  %0, %%edx" :: "r"(base) : "edx" );
    __asm__ __volatile__("movl  %0, %%edi" :: "r"(address) : "edi" );
    __asm__ __volatile__("rep   insw");
    __asm__ __volatile__("popl  %%edi" ::: "edi");
    __asm__ __volatile__("popl  %%edx" ::: "edx");
    __asm__ __volatile__("popl  %%ecx" ::: "ecx");
}

// id = [0..3], номер ata
int drive_identify(int id) {

    int i;

    // Не использовать идентификацию для нерабочего устройства
    if (drive[id].type == ATADEV_UNKNOWN)
        return 0;

    int slavebit = id & 1;
    struct DEVICE * ctrl = & drive[id];

    // Установить рабочий драйв
    drive_select(id & 1, & drive[id]);

    // Команда на считывание информации о диске
    IoWrite8(ctrl->base + REG_COUNT,   0x00);
    IoWrite8(ctrl->base + REG_LBA_LO,  0x00);
    IoWrite8(ctrl->base + REG_LBA_MID, 0x00);
    IoWrite8(ctrl->base + REG_LBA_HI,  0x00);

    // IDENTIFY
    IoWrite8(ctrl->base + REG_CMD,     0xEC);

    int w = IoRead8(ctrl->base + REG_CMD);

    // Ошибка драйва?
    if (w == 0) return 0;

    // Ожидание устройства
    for (i = 0; i < 4096; i++) {

        // Ждем BSY=0
        if ((IoRead8(ctrl->base + REG_CMD) & 0x80) == 0) {

            // Читаем 1 сектор в режиме PIO
            drive_pio_read(ctrl->base, ctrl->identify);

            // Определяем стартовый сектор #0
            drive[id].start = 0;

            return 1;
        }
    }

    return 0;
}

// Подготовка устройства к запросу на чтение или запись
// command = 0x24 READ; 0x34 WRITE
void drive_prepare_lba(int device_id, uint32_t lba, int count, int command) {

    int base   = drive[ device_id ].base;
    int devctl = drive[ device_id ].dev_ctl;

    // Коррекция
    lba += drive[ device_id ].start;

    // Выбор устройства (primary | slave) | 0x40=Set LBA Bit | LBA[27:24]
    IoWrite8(base + REG_DEVSEL, 0xA0 | 0x40 | (device_id & 1) << 4 | ((lba >> 24) & 0xF) );

    // Ожидать 400ns, пока драйв включится в работу
    IoRead8(devctl);
	IoRead8(devctl);
	IoRead8(devctl);
	IoRead8(devctl);

    // Старшие разряды
    IoWrite8(base + REG_COUNT,   (count >>  8) & 0xFF);
    IoWrite8(base + REG_LBA_LO,  (lba   >> 24) & 0xFF);
    IoWrite8(base + REG_LBA_MID, 0);
    IoWrite8(base + REG_LBA_HI,  0);

    // Младшие
    IoWrite8(base + REG_COUNT,   (count    ) & 0xFF);
    IoWrite8(base + REG_LBA_LO,  (lba      ) & 0xFF);
    IoWrite8(base + REG_LBA_MID, (lba >>  8) & 0xFF);
    IoWrite8(base + REG_LBA_HI,  (lba >> 16) & 0xFF);

    // Запрос чтения
    IoWrite8(base + REG_CMD, command);
}

// Чтение сектора с выбранного ATA
int drive_read_sectors(uint8_t* address, int device_id, int lba, int count) {

    int i;
    int base = drive[ device_id ].base;

    // Подготовить для чтения
    drive_prepare_lba(device_id, lba, count, 0x24);

    // Ждем BSY=0
    for (i = 0; i < 4096; i++)
    if ((IoRead8(base + REG_CMD) & 0x80) == 0) {

        // При DRQ=1, ERR=0, CORR=0, IDX=0, RDY=1, DF=0
        if ((IoRead8(base + REG_CMD) & 0x6F) == 0x48) {
            drive_pio_read(base, address);
            return 0;
        }
    }

    return 1;
}

// Определение FAT на устройстве
void fat_detect(int device_id) {

    int i, j;
    uint8_t sector[512];

    // Прочесть один сектор с диска для распознания MBR
    drive_read_sectors(sector, device_id, 0, 1);

    struct MBR_BLOCK* block = (struct MBR_BLOCK*)(sector + 0x1BE);
    struct FAT_BLOCK* fb;

    // Читаем MBR разделы
    for (i = 0; i < 4; i++) {

        switch (block[i].type) {

            case FS_TYPE_FAT12:
            case FS_TYPE_FAT16:
            case FS_TYPE_FAT32:

                fb = & fatfs[ fat_found ];

                fb->fs_type   = block[i].type;
                fb->device_id = device_id;
                fb->lba_start = block[i].lba_start;
                fb->lba_limit = block[i].lba_limit;

                // Дальнейшее сканирование сектора FAT
                drive_read_sectors(sector, device_id, fb->lba_start, 1);

                // Скопировать блок BPB2.0
                for (j = 0; j < sizeof(struct BPB_331); j++) 
                    ((uint8_t*)&fb->bpb331)[j] = sector[0x03 + j];
                    
                    
                // Количество секторов
                fb->root_dirsec = (fb->bpb331.entry_root_num * 32) / fb->bpb331.bytes2sector;

                // Это FAT12/16
                if (fb->bpb331.entry_root_num) {    
                                    
                    //total_sectors = fb->bpb331.count_sectors;                                    
                } 
                // Либо FAT32
                else {
                    
                    // Скопировать блок BPB7.1
                    for (j = 0; j < sizeof(struct BPB_71); j++) 
                        ((uint8_t*)&fb->bpb71)[j] = sector[0x24 + j];
                        
                    // Общий размер FAT, кластеры и откуда fat начинается
                    fb->fat_size     = fb->bpb71.fat_sectors;
                    fb->cluster_size = fb->bpb331.cluster_size;
                    fb->fat_start    = fb->bpb331.reserved_sector;

                    // Начало данных
                    fb->data_start   = fb->bpb331.reserved_sector + (fb->bpb331.fat_count * fb->fat_size) + fb->root_dirsec;
                    
                    // Количество секторов в данных                
                    fb->data_sectors = fb->bpb331.total_sectors - fb->data_start;              
                }
                
                fat_found++;
                break;
        }
    }
}

// Найти ATA диски
void init_ata_drives() {

    fat_found = 0;

    int device_id;

    // Перечисление 4 типов шин
    for (device_id = 0; device_id < 4; device_id++) {

        // Определить тип устройства
        drive[device_id].base    = device_id < 2 ? 0x1F0 : 0x170;
        drive[device_id].dev_ctl = device_id < 2 ? 0x3F6 : 0x376;
        drive[device_id].type    = detect_devtype(device_id & 1, & drive[ device_id ]);

        // Устройство готов
        if (drive_identify(device_id)) {
            fat_detect(device_id);
        }
    }
}
