
#include "gui.h"

// Инициализация окон
void gui_init() {

    window_count = 0;

    int i;
    for (i = 0; i < WINDOW_MAX; i++) {
        allwin[i].in_use = 0;
        allwin[i].active = 0;
    }
}

// Инициализировать окно id=[1..n]
void window_init(int id, int x, int y, int w, int h, char* title) {

    struct window* win = & allwin[ id ];

    win->x1 = x;
    win->y1 = y;
    win->x2 = x + w;
    win->y2 = y + h;

    win->w = w;
    win->h = h;
    win->title = title;
}

// Создать окно в системе
int window_create(int x, int y, int w, int h, char* title) {

    int id;

    // Найти свободное место в структурах
    for (id = 1; id < WINDOW_MAX; id++) {

        if (allwin[id].in_use == 0) {

            allwin[id].in_use = 1;
            allwin[id].active = 0;

            window_init(id, x, y, w, h, title);
            return id;
        }
    }

    return 0;
}

// Полное обновление окна
void window_repaint(int id) {

    struct window* win = & allwin[ id ];

    // Подложка
    vga_block(win->x1+1, win->y1+1, win->x2-1, win->y2-1, 7);

    // Черный ободок
    vga_block(win->x1,   win->y1,   win->x2,   win->y1, 0);
    vga_block(win->x1,   win->y1,   win->x1,   win->y2, 0);
    vga_block(win->x1,   win->y2,   win->x2,   win->y2, 0);
    vga_block(win->x2,   win->y1,   win->x2,   win->y2, 0);

    // Белый ободок
    vga_block(win->x1+1, win->y1+1, win->x2-1, win->y1+1, 15);
    vga_block(win->x1+1, win->y1+1, win->x1+1, win->y2-1, 15);

    // Заголовок
    vga_block(win->x1+2, win->y1+2, win->x2-2, win->y1+21, win->active ? 1 : 8);
    vga_putf8(win->x1+6, win->y1+4, win->title, 15, 0);
}

// Активировать новое окно
void window_activate(int id) {

    int i;

    for (i = 1; i < WINDOW_MAX; i++) {
        allwin[i].active = 0;
    }

    allwin[id].active = 1;

}

// Нарисовать кнопку
void button(int x1, int y1, int w, int h, int pressed) {

    int i, j;

    vga_block(x1,   y1,   x1+w, y1,   pressed ? 0  : 15);
    vga_block(x1+w, y1,   x1+w, y1+h, pressed ? 15 : 0);
    vga_block(x1,   y1,   x1,   y1+h, pressed ? 0  : 15);
    vga_block(x1,   y1+h, x1+w, y1+h, pressed ? 15 : 0);

    if (pressed) {

        // Полусерая область
        vga_block(x1+1, y1+1, x1+w-1, y1+h-1, 15);

        for (i = y1+2; i < y1+h-1; i++)
        for (j = x1+2+i%2; j < x1+w-1; j += 2)
            vga_pixel(j, i, 7);

        vga_block(x1+1,   y1+1,   x1+w-1, y1+1,   8);
        vga_block(x1+1,   y1+1,   x1+1,   y1+h-1, 8);
        vga_block(x1+1,   y1+h-1, x1+w-1, y1+h-1, 7);
        vga_block(x1+w-1, y1+1,   x1+w-1, y1+h-1, 7);

    } else {

        vga_block(x1+1,   y1+h-1, x1+w-1, y1+h-1, 8);
        vga_block(x1+w-1, y1+1,   x1+w-1, y1+h-1, 8);
    }
}

// Панель снизу перерисовать
void panel_repaint() {

    int t = 454, id, num = 0;

    vga_block(0, t,   639, t,   0);
    vga_block(0, t+1, 639, t+1, 15);
    vga_block(0, t+2, 639, 479, 7);

    // Кнопка пуск
    button(3, 458, 72, 19, 0);

    // Лого
    vga_block(6, 461, 11, 466, 4); vga_block(13, 461, 18, 466, 2);
    vga_block(6, 468, 11, 473, 1); vga_block(13, 468, 18, 473, 6);
    vga_putf8(3 + 20, 459 + 1, "Запуск", 0, 0);

    // Вертикальная полоса-разделитель
    vga_block(78, 458, 78, 477, 8);
    vga_block(79, 458, 79, 477, 15);

    // Отрисовать текущие окна
    for (id = 1; id < WINDOW_MAX; id++) {

        if (allwin[id].in_use) {

            int x = 83 + 127*num;
            int y = 460;

            // Нарисовать кнопку
            button(x, 458, 125, 19, allwin[id].active);

            // Написать текст (макс 13 символов)
            int n = vga_putf8(x + 4, y, allwin[id].title, 0, 13);

            // Если превышение, то дорисовать ..
            if (n == 0) vga_putf8(x + 4 + 13*8, y, "..", 0, 0);

            num++;
        }
    }
}
