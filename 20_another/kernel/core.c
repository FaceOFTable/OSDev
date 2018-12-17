// Очистка регионов памяти
void kernel_init() {

    int i;
    
    // Инициализация окон
    window_count = 0;
    
    for (i = 0; i < WINDOW_MAX; i++) {
        allwin[i].in_use = 0;
        allwin[i].active = 0;
    }
    
    // Все выделяемые блоки считаются неактивными
    for (i = 0; i < MEM_MAX_DESC; i++) {        
        mem_regions[i].param = 0;    
    }
}
