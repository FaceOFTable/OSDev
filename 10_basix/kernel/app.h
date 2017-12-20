
// Максимальное количество открытых программ
#define PROCESS_MAX         32

// Типы процессов. RAW - это тупо любой проект, FASM 32bit / GCC32
// Своего рода COM-файл
#define PROCESS_TYPE_RAW    1

struct PROCESS {
    
    uint8_t     busy;
    uint8_t     type;               // Тип процесса
    uint32_t    cr3_map;            // Страница разметки процесса
    uint8_t     tss[104];           // Здесь все регистры и флаги процесса
    
};

// Процессы
struct PROCESS apps[ PROCESS_MAX ];
