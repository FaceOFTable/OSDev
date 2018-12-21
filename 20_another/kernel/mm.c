
// Получение байта из памяти
volatile uint8_t peek(uint32_t address) {

    int8_t* t = (int8_t*)8;
    return t[address - 8];
}

// Определение максимального объема памяти
void get_max_memsize() {

    int8_t*  m   = (int8_t*)1;
    uint32_t max = 0xDFFFFFFF;
    uint32_t min = 0x00200000;
    uint32_t mid;

    while (min < max) {

        mid = (min + max) >> 1;

        // Область страниц совпала. Максимальное количество памяти в `max`
        if ((min & 0xfffff000) + 0x1000 >= (max & 0xfffff000)) {

            // Настоящее количество памяти
            mem_real_max  = (max & 0xfffff000); 
            
            // Память ОС
            mem_max       = mem_real_max > (SYSMEM<<20) ? (SYSMEM<<20) : mem_real_max; 
            mem_lower     = mem_max;
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

// Установка страничной адресации
void init_paging() {

    int i, j;
    uint32_t cr0;

    // Выровнять память для использования страниц
    uint32_t aligned = (mem_lower & ~0xFFF);

    // Выделить какое-то пустое пространство
    kalloc(mem_lower - aligned);

    // Выделит память под PDBR Ring 0
    uint32_t* pdbr = kalloc(4096);

    // Выделить память под страницы 4mb = 4kb
    uint32_t* pt_cats = kalloc((mem_real_max >> 10) & ~0xFFF);

    // Разметка страниц PDBR 1:1
    for (i = 0; i < 1024; i++) {

        // Адрес каталогов страниц
        uint32_t* ptc = pt_cats + (i << 10);

        // Просматриваем PDBR
        if ((i << 22) < mem_real_max) {

            pdbr[i] = (uint32_t)ptc | (PTE_RW | PTE_PRESENT);

            // Разметить страницы по 4Мб
            for (j = 0; j < 1024; j++) {

                uint32_t mp = (i << 22) | (j << 12);
                ptc[j] = (mp < mem_real_max ? mp | (PTE_RW | PTE_PRESENT) : 0);
            }
        }
        // Страница пуста
        else pdbr[i] = 0;
    }

    // Поместить в CR3 значение PDBR
    __asm__ __volatile__("movl %0, %%cr3"       : : "r"(pdbr) );
    __asm__ __volatile__("movl %%cr0, %0"       : : "r"(cr0) );
    __asm__ __volatile__("orl  $0x80000000, %0" :   "=r"(cr0) );
    __asm__ __volatile__("movl %0, %%cr0"       :   "=r"(cr0) );
    __asm__ __volatile__("jmp  localm");
    __asm__ __volatile__("localm:");
}

// Выделение резидентной памяти
void* kalloc(size_t size) {

    mem_lower -= size;
    return (void*)mem_lower;
}

// Освободить память
void free(void* ptr) {

    // ..
}

// Изменить размер
void realloc(void* ptr, size_t size) {
}

// Инициализировать память
void init_memory() {
    get_max_memsize();
}

// Найти свободную память и выделить ее
void* get_free_page() {
    
    void* m;
    return m;
}

// Выделение линейного куска памяти в страничной организации в виртуальном пространстве
void* malloc(uint32_t size) {
    
    void* m;
    return m;
}
