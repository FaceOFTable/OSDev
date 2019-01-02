
#define PS2_BUTTON_LEFT         1
#define PS2_BUTTON_RIGHT        2
#define PS2_BUTTON_MIDDLE       4

int     ps2_pressed;        // =1 Мышь нажата
int     ps2_mouse_state;    // Текущие нажатые биты
int     ps2_time_at;        // Когда именно нажата

// ----------------------------------------------------
// https://wiki.osdev.org/%228042%22_PS/2_Controller

// bit=1 read, bit=2 write
void kb_wait(int rw) {

    int i;
    for (i = 0; i < KB_WAIT; i++)
        if ((IoRead8(0x64) & rw) == 0)
            return;
}

// Ожидание специального случая
void kb_wait_not() {

    int i;
    for (i = 0; i < 8*KB_WAIT; i++)
        if ((IoRead8(0x64) & 1))
            return;
}

// Отправка команды
void kb_cmd(int comm) {

    kb_wait(2);
    IoWrite8(0x64, comm);

    kb_wait(2);
}

// Запись данных
void kb_write(int data) {

    kb_wait(0x20);           // (May be "receive time-out" or "second PS/2 port output buffer full") Ожидание готовности
    IoRead8(0x60);           // Чтение данных из порта (не имеет значения)

    kb_wait(2);                 // Ждать для записи
    IoWrite8(0x60, data);       // Записать данные

    kb_wait(2);                 // Ждать для записи
    kb_wait_not();              // Подождать, пока будет =1 на чтение
}

// Чтение данных
int kb_read() {

    kb_wait_not();
    delay();
    return IoRead8(0x60);
}

// Инициализацировать мышь
void init_ps2_mouse() {

    int a;

    kb_cmd(0xA8);     kb_read();
    kb_cmd(0x20); a = kb_read();

    kb_cmd(0x60);
    kb_write(a | 3);

    // Отослать команду для PS/2 разрешени
    kb_cmd(0xD4);
    kb_write(0xF4);
    kb_read();

    // Инициализировать переменные
    ps2_pressed = 0;
    ps2_time_at = 0;
    ps2_mouse_state = 0;
}

// Отослать события нажатия мыши
// key = 1 | 2 | 4
// dir = 1 (down) 0 (up)

void push_event_click(int key, int dir) {

    int i, hwnd = 0, hit = 0, active_last = 0;
    int x = cursor.mouse_x;
    int y = cursor.mouse_y;

    struct window* w;

    // Обнаружить, где именно нажата мышь
    for (i = 1; i < WINDOW_MAX; i++) {

        w = & allwin[i];

        if (w->active) {
            active_last = i;
        }

        // Проверять все
        if (w->in_use) {

            // Проверка попадания в окно
            if (w->x1 <= x && x <= w->x2 && w->y1 <= y && y <= w->y2) {
                hwnd = i;
                hit = 1;
            }
        }
    }

    // Отослать событие
    if (hit) {

        w = & allwin[ hwnd ];

        // Активация нового окна
        if (w->active == 0) {

            window_activate(hwnd);

            // Перерисова предыдущего активного окна
            if (active_last) {
                if (allwin[ active_last ].active == 0) {
                    window_repaint(active_last);
                }
            }

            window_repaint(hwnd);
            panel_repaint();
        }

        // Вызвать callback, если есть
        if (dir) {
            if (w->event_mousedown)
                w->event_mousedown();
        }
        else {
            if (w->event_mouseup)
                w->event_mouseup();
        }
    }
}

// Тест на нажатие и отпускание мыши
void ps2_edge_click(int cmd, int key) {

    // Левая кнопка мыши
    if (cmd & key) {

        if ((ps2_mouse_state & key) == 0) {
            push_event_click(key, 1);
        }
        ps2_mouse_state |= key;
    }
    else {

        if (ps2_mouse_state & key) {
            push_event_click(key, 0);
        }
        ps2_mouse_state &= ~key;
    }
}

// Перехватчик событий мыши
// https://wiki.osdev.org/PS/2_Mouse
void pic_ps2mouse() {

    int x, y, cmd;

    kb_cmd(0xAD);       // Команда и блокировка клавиатуры
    cmd = kb_read();    // Конфигурация
    x   = kb_read();
    y   = kb_read();

    // @todo overflow x,y

    int xn = cursor.mouse_x;
    int yn = cursor.mouse_y;
    int xo = xn, yo = yn;

    // Знаковое расширение (x,y)
    if (cmd & 0x10) x = -((x ^ 0xFF) + 1);
    if (cmd & 0x20) y = -((y ^ 0xFF) + 1);

    xn += x;
    yn -= y;

    if (xn < 0)   xn = 0;
    if (yn < 0)   yn = 0;
    if (xn > 639) xn = 639;
    if (yn > 479) yn = 479;

    // Новая позиция мыши
    mouse_xy(xn, yn);

    // Обнаружение событий нажатия и отпускания мыши
    ps2_edge_click(cmd, PS2_BUTTON_LEFT);
    ps2_edge_click(cmd, PS2_BUTTON_RIGHT);
    ps2_edge_click(cmd, PS2_BUTTON_MIDDLE);

    // @todo отследить изменения мыши

    update_region(xo, yo, xo + 12, yo + 21); // Старый регион затереть
    update_region(xn, yn, xn + 12, yn + 21); // Новый установить

    kb_cmd(0xAE); // Разблокировка клавиатуры
}
