/*
 * Выдача меню
 */
 
static const char* menu [3] = {
    "README",
    "ZX Spectrum Experience",
    "Homebrew BASIC"
};

// Выдача и обработка стартового простого интерфейса
void app_start_interface() {
    
    int i;
    
    io_display_bgblock(0, 0, 2000, 0x70); // Очистить экран (цвет)
    io_display_frame(0xf, 0x5, 0x41, 0x14); // Нарисовать фрейм

    // Заголовок
    io_display_text(0x10, 0x05, " DISKETTE DREAM 2000/11 ");
    io_display_bgblock(0x10, 0x05, 24, 0x47);
    
    //brk;
    for (i = 0; i < 3; i++) {    
        io_display_text(0x11, 0x07 + i, (char*)menu[i]);
    }
    
    io_display_bgblock(0x10, 0x07, 49, 0x07);
}
