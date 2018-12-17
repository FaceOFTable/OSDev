
#include "gui.h"


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
    block(win->x1+1, win->y1+1, win->x2-1, win->y2-1, 7);

    // Черный ободок
    block(win->x1,   win->y1,   win->x2,   win->y1, 0);
    block(win->x1,   win->y1,   win->x1,   win->y2, 0);
    block(win->x1,   win->y2,   win->x2,   win->y2, 0);
    block(win->x2,   win->y1,   win->x2,   win->y2, 0);

    // Белый ободок
    block(win->x1+1, win->y1+1, win->x2-1, win->y1+1, 15);
    block(win->x1+1, win->y1+1, win->x1+1, win->y2-1, 15);

    // Заголовок
    block(win->x1+2, win->y1+2, win->x2-2, win->y1+21, win->active ? 1 : 8);
    print_utf8(win->x1+6, win->y1+4, win->title, 15, -1, 0);
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
    int x2 = x1 + w, 
        y2 = y1 + h;

    block(x1,   y1,   x2, y1, pressed ? 0  : 15);
    block(x1+w, y1,   x2, y2, pressed ? 15 : 0);
    block(x1,   y1,   x1, y2, pressed ? 0  : 15);
    block(x1,   y1+h, x2, y2, pressed ? 15 : 0);

    if (pressed) {

        // Полусерая область
        block(x1+1, y1+1, x2-1, y2-1, 15);

        for (i = y1+2; i < y2-1; i++)
        for (j = x1+2+i%2; j < x2-1; j += 2)
            pset(j, i, 7);

        block(x1+1, y1+1, x2-1, y1+1, 8);
        block(x1+1, y1+1, x1+1, y2-1, 8);
        block(x1+1, y2-1, x2-1, y2-1, 7);
        block(x2-1, y1+1, x2-1, y2-1, 7);

    } else {

        block(x1+1, y2-1, x2-1, y2-1, 8);
        block(x2-1, y1+1, x2-1, y2-1, 8);
    }
}

// Панель снизу перерисовать
void panel_repaint() {

    int t = 454, id, num = 0;

    block(0, t,   639, t,   0);
    block(0, t+1, 639, t+1, 15);
    block(0, t+2, 639, 479, 7);

    // Кнопка пуск
    button(3, 458, 72, 19, 0);

    // Лого
    block(6, 461, 11, 466, 4); block(13, 461, 18, 466, 2);
    block(6, 468, 11, 473, 1); block(13, 468, 18, 473, 6);
    print_utf8(3 + 20, 459 + 1, "Запуск", 0, -1, 0);

    // Вертикальная полоса-разделитель
    block(78, 458, 78, 477, 8);
    block(79, 458, 79, 477, 15);

    // Отрисовать текущие окна
    for (id = 1; id < WINDOW_MAX; id++) {

        if (allwin[id].in_use) {

            int x = 83 + 127*num;
            int y = 460;

            // Нарисовать кнопку
            button(x, 458, 125, 19, allwin[id].active);

            // Написать текст (макс 13 символов)
            int n = print_utf8(x + 4, y, allwin[id].title, 0, -1, 13);

            // Если превышение, то дорисовать ..
            if (n == 0) print_utf8(x + 4 + 13*8, y, "..", 0, -1, 0);

            num++;
        }
    }
}
