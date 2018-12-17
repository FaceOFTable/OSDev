#include "mm.h"

// Получение байта из памяти
volatile uint8_t peek(uint32_t address) {

    int8_t* t = (int8_t*)8;
    return t[address - 8];
}

// Определение максимального объема памяти бинарным поиском
// Максимальное количество итерации ~24
void detect_memory_size() {

    int8_t*  m   = (int8_t*)1;
    uint32_t max = 0xDFFFFFFF;
    uint32_t min = 0x00100000;
    uint32_t mid;
    
    // Сброс количества
    mem_used = 0;

    while (min < max) {
        
        mid = (min + max) >> 1;
        
        // Область страниц совпала. Максимальное количество памяти в `max`
        if ((min & 0xfffff000) + 0x1000 >= (max & 0xfffff000)) {

            mem_max   = (max & 0xfffff000);
            mem_lower = mem_max;
            break;
        }

        // Проверка на способность памяти измениться в этой точке
        volatile int8_t a = peek(mid); m[ mid-1 ] ^= 0x55;
        volatile int8_t b = peek(mid); m[ mid-1 ] ^= 0x55;
        
        if (a == b) {
            max = mid; // Здесь mid слишком высок
        } else {
            min = mid; // Поднять mid
        }
    }
}

// Выделение нового дескриптора памяти
void* malloc(size_t size) {
    
    int i;

    // Слишком малый размер неэффективен
    if (size < 32)
        size = 32;  
        
    // Сортировка доступных регионов по возрастанию
    // Нахождение "зазоров" нужного размера между ними
    // -- если найден, вставляется между блоками памяти
            
    // Отрезаем "кусок" памяти
    // ----------------
    mem_lower -= size;
    
    // Установка параметров
    mem_regions[ mem_used ].param |= MEM_PARAM_BUSY;
    mem_regions[ mem_used ].start = (void*)mem_lower;
    mem_regions[ mem_used ].limit = size;
    // ----------------
    
    mem_used++;
    
    return (void*)mem_lower;
}

// Освободить память
void free(int desc_id) {
    
    // .. 
}

// Изменить размер
void realloc(int desc_id, size_t __size) {
}

// Сдвигает регионы в одну кучу
void compact() {
}

// Получение адреса региона памяти
void* resolve(int desc_id) {

    return mem_regions[ desc_id ].start;    
}
