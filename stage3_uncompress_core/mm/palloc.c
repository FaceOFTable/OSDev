#include "mm.h"

/*
 * Поиск свободной страницы
 * 
 * Ситуация 1. Свободных страниц осталось 1, необходимо выделить 4 кб
 * еще для того, чтобы записать новый каталог из PDBR.
 * 
 * Ситуация 2. Памяти вообще не осталось. Это определяется через размер,
 * который задал loader. Важно.
 * 
 * @RETURN  0 -- Не найдено больше страниц
 */
 
uint32_t palloc() {

    int dir_id, id, k;
    
    // Сначала пройтись по каталогам
    uint32_t* pdbr = (uint32_t*)0x1000;
    
    // Пересмотреть все 4Гб каталогов
    for (id = 0; id < 1024; id++) {

        uint32_t* dir_addr = (uint32_t*)(pdbr[ id ] & 0xFFFFF000);
        uint16_t  dir_attr = pdbr[ id ] & 0x0FFF;
        
        // Каталог не должен быть полным, иначе нет смысла смотреть
        if (dir_attr & PT_FULL) {        
            continue;
        
        }
        // Каталог 4Мб в памяти представлен, просмотреть его        
        else if (dir_attr & PT_PRESENT) {

            // Поиск свободных страниц
            for (dir_id = 0; dir_id < 1024; dir_id++) {
                
                uint32_t page_addr = (id << 22) | (dir_id << 12);
                uint16_t page_attr = dir_addr[ dir_id ] & 0x0FFF;
                
                // Данная страница в этом каталоге не занята? 
                if (!(page_attr & PT_PRESENT)) {                    
                    
                    // Занять страницу!                
                    dir_addr[ dir_id ] = page_addr | 3;
                    
                    // Очистить эту страницу
                    uint32_t* tmp_addr = (uint32_t*) page_addr;
                    for (k = 0; k < 1024; k++) {
                        tmp_addr[k] = 0;
                    }
                    
                    return page_addr;
                }
            }
            
        // В ранее просмотренных каталогах ничего нет. Выделить новый.
        } else {

            // Отметить предыдущий как "полный" (не найдено свободных)
            pdbr[id - 1] |= PT_FULL;
            
            uint32_t  page_raw = (id << 22);
            
            // Адрес 4000h-4FFFh теперь указывает на [page_raw ... +4095]
            uint32_t *page_reloc = (uint32_t*)0x2010;
                     *page_reloc = page_raw | 3;

            // Указатель директории каталогов указывает на новый каталог
            pdbr[ id ] = page_raw | 3;

            // Через смаппленный 4xxxh мы записываем новый каталог
            uint32_t* temp_table = (uint32_t*)0x4000;

            // Первая страница каталога всегда будет указывать на себя
            for (k = 0; k < 1024; k++) {
                
                if (k == 0) {                
                    temp_table[k] = page_raw | 3;
                } else {
                    temp_table[k] = 0;
                }
            }
            
            // Установка указателя
            temp_table[ 1 ] = (page_raw + 0x1000) | 3;

            // Очистка этой страницы 4kb -> ZERO
            uint32_t* page_addr = (1024 + (uint32_t*)page_raw);
                        
            for (k = 0; k < 1024; k++) {
                page_addr[k] = 0;
            }

            return (page_raw + 0x1000);
        }
    }
 
    return 0;   
}
