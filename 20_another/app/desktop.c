
// На рабочий стол было нажатие
void desktop_mousedown() {
    
    colorat(0,0,0xffffff,0);
    print_int(cursor.mouse_x);    
    print(", ");
    print_int(cursor.mouse_y);
}

// Создать базовое окно
void make_desktop() {
    
    desktop_hwnd = window_create(0, 0, 640, 480, "Desktop");
    
    allwin[ desktop_hwnd ].panel = 0;
    allwin[ desktop_hwnd ].state = 0;

    // Назначение событий на основной рабочий стол
    window_event(desktop_hwnd, EVENT_MOUSEDOWN, & desktop_mousedown);
}

// Панель снизу
void panel_repaint() {

    int t = 454, id, num = 0;

    color(0, -1);

    block(0, t,   639, t,   0);
    block(0, t+1, 639, t+1, 15);
    block(0, t+2, 639, 479, 7);

    // Кнопка пуск
    button(3, 458, 72, 19, 0);

    // Лого
    block(6, 461, 11, 466, 4); block(13, 461, 18, 466, 2);
    block(6, 468, 11, 473, 1); block(13, 468, 18, 473, 6);
    print_at(3 + 20, 459 + 1, "Запуск");

    // Вертикальная полоса-разделитель
    block(78, 458, 78, 477, 8);
    block(79, 458, 79, 477, 15);

    // Параметры вывода
    color(0, -1);
    cursor.max_chars = 13;

    // Отрисовать текущие окна
    for (id = 1; id < WINDOW_MAX; id++) {

        // Окно есть и можно рисовать его в панели
        if (allwin[id].in_use && allwin[id].panel) {

            int x = 83 + 127*num;
            int y = 460;

            // Нарисовать кнопку
            button(x, 458, 125, 19, allwin[id].active);

            // Написать текст (макс 13 символов)
            int n = print_at(x + 4, y, allwin[id].title);

            // Если превышение, то дорисовать ..
            if (n == 0) print_at(x + 4 + 13*8, y, "..");

            num++;
        }
    }

    cursor.max_chars = 0;
}
