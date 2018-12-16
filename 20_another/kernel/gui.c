
#include "gui.h"

// Инициализировать окно
void window_init(struct window* win, int x, int y, int w, int h, char* title) {
    
    win->x1 = x;
    win->y1 = y;
    win->x2 = x + w;
    win->y2 = y + h;
    
    win->w = w;
    win->h = h;
    win->title = title;    
}

// Обновление окна
void window_repaint(struct window* win, int active) {
    
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
    vga_block(win->x1+2, win->y1+2, win->x2-2, win->y1+21, active ? 1 : 8);
    vga_putf8(win->x1+6, win->y1+4, win->title, 15);
}

// Нарисовать кнопку
void button(int x1, int y1, int w, int h) {
    
    vga_block(x1,   y1,   x1+w, y1, 15);
    vga_block(x1+w, y1,   x1+w, y1+h, 0);
    vga_block(x1,   y1,   x1,   y1+h, 15);
    vga_block(x1,   y1+h, x1+w, y1+h, 0);
    
    vga_block(x1+1, y1+h-1, x1+w-1, y1+h-1, 8);
    vga_block(x1+w-1, y1+1, x1+w-1, y1+h-1, 8);
    
}

// Панель снизу перерисовать
void panel_repaint() {
    
    vga_block(0, 454, 639, 455, 0);
    vga_block(0, 455, 639, 456, 15);
    vga_block(0, 456, 639, 479, 7);
    
    // Кнопка пуск
    button(3, 458, 72, 19);
    
    // Лого
    vga_block(6, 461, 11, 466, 4); vga_block(13, 461, 18, 466, 2);
    vga_block(6, 468, 11, 473, 1); vga_block(13, 468, 18, 473, 6);
    vga_putf8(3 + 20, 459 + 1, "Запуск", 0);
    
    // Вертикальная полоса-разделитель
    vga_block(78, 458, 78, 477, 8);
    vga_block(79, 458, 79, 477, 15);
}
