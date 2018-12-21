// Более нельзя выделить дескрипторов для ядра
#define MEM_MAX_DESC        4096
#define MEM_PARAM_BUSY      1       // Признак занятости дескриптора

uint32_t mem_max;       // Максимальное кол-во памяти в системе
uint32_t mem_lower;     // Нижняя граница свободного пространства

struct mem_chain {
    
    uint32_t next;      // Следующий блок (вниз)
    uint32_t size;      // Размер этого блока. Если =0, блок не занят    
};

void* kalloc(size_t);
