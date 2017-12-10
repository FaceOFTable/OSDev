/*
 * Утилиты для рисования на текстовом дисплее 
 */

// Рисование блока (фонового), начиная с [x,y] позиции
void io_display_bgblock(int x, int y, int length, char bgcolor) {
        
    int i;
    char* addr = DISPLAY_TEXT_ADDR + (x + y*80) * 2;
    
    for (i = 0; i < length; i++) {
        addr[2*i + 1] = bgcolor;
    }    
}

// Рисование фрейма
void io_display_frame(int x1, int y1, int x2, int y2) {
    
    int i;
    char* addr = DISPLAY_TEXT_ADDR;
    
    // Уголки
    addr[ 2*(x1 +y1*80) ] = 0xDA;
    addr[ 2*(x2 +y1*80) ] = 0xBF;
    addr[ 2*(x1 +y2*80) ] = 0xC0;
    addr[ 2*(x2 +y2*80) ] = 0xD9;
    
    // Горизонтальные линии
    for (i = x1 + 1; i < x2; i++) {
        addr[ 2*(i + y1*80) ] = 0xC4;
        addr[ 2*(i + y2*80) ] = 0xC4;
    }
    
    // Вертикальные линиии
    for (i = y1 + 1; i < y2; i++) {
        addr[ 2*(x1 + i*80) ] = 0xB3;
        addr[ 2*(x2 + i*80) ] = 0xB3;
    }
}

// Печать строки
void io_display_text(int x1, int y1, char* string) {
    
    char* addr = DISPLAY_TEXT_ADDR + 2*(x1 + y1*80);
    
    while (*string) {
        
        *addr = *string;
        addr += 2;
        string++;
    }
    
}
