#include "mm.h"

void detect_memory_size() {
    
    int8_t*  m   = (int8_t*)1;
    uint32_t max = 0xDFFFFFFF;
    uint32_t min = 0x55100000;
    uint32_t mid;
    
    brk;
   
    for (mid = min; mid < max; mid += 4096) {
        
        volatile int8_t a = m[ mid - 1 ];
        m[ mid-1 ] ^= 0x55;
        volatile int8_t b = m[ mid - 1 ];
        
        if (a == b) {
            break;
        }        
    }
    
    
}

// Выделение новой памяти
void* malloc(size_t __size) {
    
    void* m;
    return m;
}

// Освободить память
void free(void* __ptr) {
}

// Изменить размер
void realloc(void* __ptr, size_t __size) {
}
