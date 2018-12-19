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

    // Выбор устройства (primary | slave)
    IoWrite8(ctrl->base + REG_DEVSEL, 0xA0 | slavebit << 4);

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
void drive_read(int base, uint8_t* address) {
    
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
void drive_identify(int id) {

    int i;

    // Не использовать идентификацию для нерабочего устройства
    if (drive[id].type == ATADEV_UNKNOWN)
        return;

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
    if (w == 0) return;

    // Ожидание устройства
    for (i = 0; i < 4096; i++) {

        // Ждем BSY=0
        if ((IoRead8(ctrl->base + REG_CMD) & 0x80) == 0) {

            // Читаем 1 сектор в режиме PIO
            drive_read(ctrl->base, ctrl->identify );
        }
    }
}

// Найти ATA диски
void ata_drive_detect() {

    // IDE 0, Primary
    drive[0].base    = 0x1F0;
    drive[0].dev_ctl = 0x3F6;
    drive[0].type    = detect_devtype(0, & drive[0]);

    drive_identify(0);

    // IDE 0, Primary
    drive[1].base    = 0x1F0;
    drive[1].dev_ctl = 0x3F6;
    drive[1].type    = detect_devtype(1, & drive[1]);

    // IDE 1, Primary
    drive[2].base    = 0x170;
    drive[2].dev_ctl = 0x376;
    drive[2].type    = detect_devtype(0, & drive[2]);

    // IDE 1, Slave
    drive[3].base    = 0x170;
    drive[3].dev_ctl = 0x376;
    drive[3].type    = detect_devtype(1, & drive[3]);
}

