
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

#define ATADEV_UNKNOWN  0
#define ATADEV_PATAPI   1
#define ATADEV_SATAPI   2
#define ATADEV_PATA     3
#define ATADEV_SATA     4

struct DEVICE {    
    
    uint16_t base;          // Базовый адрес
    uint16_t dev_ctl;       // Управляющий
    uint8_t  type;          // Тип девайса, например ATADEV_PATA
    uint8_t  identify[512]; // Информация от устройства
};

// Список драйвов
struct DEVICE drive[4];
