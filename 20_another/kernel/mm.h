/*
 * 1mb     Ядро
 * 2-32mb  Область системных данных
 * 32..n   Физическая свободная память
 * n...3gb Виртуальная память
 */

#define PTE_PRESENT 1
#define PTE_RW      2

#define SYSMEM      32      // 32mb

uint32_t    mem_max;        // Память для системы
uint32_t    mem_real_max;   // Реальное кол-в памяти 
uint32_t    mem_lower;      // Нижняя граница свободного пространства
uint32_t*   pdbr;           // Page Directory Base Root

void* kalloc(size_t);
