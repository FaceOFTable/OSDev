/*
 * Выдача меню
 */
 
static const char* menu [3] = {
    "README",
    "ZX Spectrum Experience",
    "Homebrew BASIC"
};

// Выдача и обработка стартового простого интерфейса
void ui_start() {
    
    int i;
    
    set_cursor(16,5);
    set_cursor_mode(0,15);
    
    bgblock(0, 0, 2000, 0x70); // Очистить экран (цвет)
    draw_frame(0xf, 0x5, 0x41, 0x14); // Нарисовать фрейм

    // Заголовок
    print_text(0x10, 0x05, " DISKETTE DREAM 2000/11 ");
    bgblock(0x10, 0x05, 24, 0x47);
        
    for (i = 0; i < 3; i++) {    
        print_text(0x11, 0x07 + i, (char*)menu[i]);
    }
    
    bgblock(0x10, 0x07, 49, 0x07);
}
