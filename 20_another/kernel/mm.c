#include "mm.h"

// Получение байта из памяти
volatile uint8_t peek(uint32_t address) {

    int8_t* t = (int8_t*)8;
    return t[address - 8];
}

// Определение максимального объема памяти бинарным поиском
// Максимальное количество итерации ~24
void init_memory_size() {

    int8_t*  m   = (int8_t*)1;
    uint32_t max = 0xDFFFFFFF;
    uint32_t min = 0x00200000;
    uint32_t mid;

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
    if (size < 64)
        size = 64;

    // Нахождение "зазоров" нужного размера между ними
    // -- если найден, вставляется между блоками памяти

    // Отрезаем "кусок" памяти
    // ----------------

    // Вставляем информацию об управляющей цепи памяти
    mem_lower -= sizeof(struct mem_chain);

    struct mem_chain* cm = (struct mem_chain*)mem_lower;

    // Память опускается на нужное количество байт
    mem_lower -= size;

    // Запись новой информации о цепи
    cm->size = size;      // Объем выделенных данных
    cm->next = 0;         // Следующая точка неизвестна

    return (void*)mem_lower;
}

// Освободить память
void free(void* ptr) {

    // ..
}

// Изменить размер
void realloc(void* ptr, size_t size) {
}
