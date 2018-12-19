
#define REG_DATA    0
#define REG_ERR     1
#define REG_COUNT   2

#define REG_SEC_NUM 3
#define REG_LBA_LO  3

#define REG_CYL_LO  4
#define REG_LBA_MID 4

#define REG_CYL_HI  5
#define REG_LBA_HI  5

#define REG_DEVSEL  6
#define REG_CMD     7

#define ATADEV_UNKNOWN      0
#define ATADEV_PATAPI       1
#define ATADEV_SATAPI       2
#define ATADEV_PATA         3
#define ATADEV_SATA         4

// Типы FS
#define FS_TYPE_FAT12       0x01
#define FS_TYPE_FAT16       0x06
#define FS_TYPE_FAT32       0x0B

// Устройство ATA
struct DEVICE {

    uint16_t base;          // Базовый адрес
    uint16_t start;         // Стартовый сектор (0 или 1)
    uint16_t dev_ctl;       // Управляющий
    uint8_t  type;          // Тип девайса, например ATADEV_PATA
    uint8_t  identify[512]; // Информация от устройства
};

// Блок файловой системы
struct FAT_BLOCK {

    uint8_t   fs_type;      // Тип файловой системы
    uint8_t   device_id;    // Где была найдена, device_id=[1..4] -- ATA, 0-не найдено
    uint32_t  lba_start;    // Стартовый сектор fs
    uint32_t  lba_limit;    // Длина раздела
    uint16_t  cluster_size; // Секторов в кластере
    uint32_t  data_start;   // Стартовый сектор данных
    uint32_t  fat_start;    // Старт FAT
    uint32_t  fat_count;    // Количество
};

// ---------------------------------------

int fat_found;  // Количество найденных FAT

// Список драйвов
struct DEVICE    drive[4];  // 4 канала
struct FAT_BLOCK fatfs[16]; // До 16 файловых систем на 4 разделах
