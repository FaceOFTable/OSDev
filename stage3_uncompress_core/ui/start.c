/*
 * Выдача меню
 */
 
static const char* ui_menu [3] = {
    "README",
    "ZX Spectrum Experience",
    "Homebrew BASIC"
};

// Выдача и обработка стартового простого интерфейса
void ui_start() {
    
    int i;
    
    display_cursor_at(0,0);
    display_cursor_mode(15,14);
    
    display_bgblock(0, 0, 2000, 0x70);      // Очистить экран (цвет)
    display_frame(0xf, 0x5, 0x41, 0x14);    // Нарисовать фрейм

    // Заголовок
    display_text(0x10, 0x05, " DISKETTE DREAM 2000/11 ");
    display_bgblock(0x10, 0x05, 24, 0x47);
        
    for (i = 0; i < 3; i++) {    
        display_text(0x11, 0x07 + i, (char*)ui_menu[i]);
    }
    
    display_bgblock(0x10, 0x07, 49, 0x07);
}
