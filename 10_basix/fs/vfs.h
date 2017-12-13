// Текущие статусы 
#define FILE_STATUS_ERROR       0
#define FILE_STATUS_OPEN        1

// Типы FS
#define FILE_FSTYPE_NONE        0
#define FILE_FSTYPE_INITRD      1

// Не более 256 открытых файлов
// Каждый дескриптор - это указатель на другие массивы (для файлов)
uint32_t file_descriptors[256];

struct FILE {
    
    uint32_t file_id;
    uint32_t fs_type;
    uint16_t status;
    
};
