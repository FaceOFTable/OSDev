u8   cursor_x = 0;
u8   cursor_y = 0;                   // Положение курсора в консоли
u16* console = 0;                    // Память для отображения данных в консоли

// Пропечатать строку
void addstr(const char* str) {

    while (*str) {

        console[cursor_x + cursor_y*80] = *str;

        cursor_x++;
        if (cursor_x >= 80) {
            cursor_y++;
        }

        str++;
    }

}

/* ***********************************************
 * Терминал
 * *********************************************** */

// Опрос приложения из основного цикла системы
void app_console() {

    int i;

    // Создать и очистить консоль
    console = (u16*)malloc(80*30);
    for (i = 0; i < 80*30; i++) console[i] = 0;



    // Очистить область вывода
    vga_fillrect(0, 0, 639, 479, 0x1);

    // Основной бесконечный цикл
    while (1) {




    }   
}