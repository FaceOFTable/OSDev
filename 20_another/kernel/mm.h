/*
 * 1mb     Ядро
 * 2-32mb  Область системных данных
 * 32..n   Физическая свободная память
 * n...3gb Виртуальная память
 */

#define PTE_PRESENT     1
#define PTE_RW          2
#define SYSMEM          8       // Минимальное количество памяти для ядра

uint32_t*   PDBR;           // Page Directory Base Root
uint32_t*   CPage;          // Особая страница для создания других страниц
uint32_t*   PTE;            // Страницы каталогов page tables

uint32_t    mem_max;        // Память для системы
uint32_t    mem_real_max;   // Реальное кол-в памяти 
uint32_t    mem_lower;      // Нижняя граница свободного пространства

void* kalloc(size_t);
