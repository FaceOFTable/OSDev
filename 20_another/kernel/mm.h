// Более нельзя выделить дескрипторов для ядра
#define MEM_MAX_DESC        4096
#define MEM_PARAM_BUSY      1       // Признак занятости дескриптора

uint32_t mem_max;       // Максимальное кол-во памяти в системе
uint32_t mem_lower;     // Нижняя граница свободного пространства
uint32_t mem_used;      // Количество использованной памяти

// Структура дескриптора
struct mem_descriptor {
    
    void *   start;      // Начало региона
    uint32_t limit;      // Его размер
    uint32_t param;      // Параметры блока
    
};

// Всего возможно выделить MEM_MAX_DESC блоков памяти
struct mem_descriptor mem_regions[ MEM_MAX_DESC ];
