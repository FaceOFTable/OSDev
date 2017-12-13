#define FAT12_DEVICE_FLOPPY     0
#define FAT12_DEVICE_ERR        0xFE
#define FAT12_DEVICE_RAM        0xFF

// Область памяти, откуда начинается виртуальный floppy-диск
#define INITRD_START        0x00200000

// Описатель дескриптора
struct FS_FAT12 {
    
    uint8_t     busy;                   // >0, открыт
    uint8_t     device_id;              // Номер устройства (0-Floppy, FE-error, FF-ram)
    uint16_t    cluster_dir;            // Указатель на текущую директорию
    uint16_t    cluster_current;        // Текущий кластер в файле
    uint32_t    file_size;              // Размер
    uint32_t    current;                // Текущая позиция в файле
    
};

// Описатели дескрипторов
struct FS_FAT12 fat12_desc[256];
