/**
 * @desc Слой над vga.c
 */

#include "canvas.h"

// Очистка экрана
void cls(unsigned char color) {

    int i;

    // Заполнить цветом
    for (i = 0; i < 640*480; i++)
        canvas[i] = color;

    vga_cls( color );

    // Инициализация курсора
    cursor.x = 0;
    cursor.y = 0;
    cursor.frcolor       = 0;
    cursor.bgcolor       = -1;
    cursor.border_top    = 0;
    cursor.border_right  = 639;
    cursor.border_bottom = 479;
    cursor.border_left   = 0;
    cursor.max_chars     = 0;
}

// Цвета курсора
void cursor_color(int frcolor, int bgcolor) {

    cursor.frcolor = frcolor;
    cursor.bgcolor = bgcolor;
}

// Проверка на наличие МЫШИ в данной точке
unsigned char point(int x, int y) {

    unsigned char color;
    int mx = cursor.mouse_x,
        my = cursor.mouse_y;

    // Возможно, тут находится МЫШЬ
    if (mx <= x && x < mx + 12 && my <= y && y < my + 21) {

        color = cursor.mouse_show ? (mouse_icon[y - my] >> (2*(11 - x - mx))) & 3 : 0;

        if (color) {

            switch (color) {
                case 1:  color = 7;  break;
                case 2:  color = 15; break;
                case 3:  color = 8;  break;
            }

            return color;

        }
    }

    return 0;
}

// ---------------------------------------------------------------------
// Нарисовать точку как на экране, так и в BB
void pset(int x, int y, unsigned char color) {

    if (x >= 0 && x < 640 && y >= 0 && y < 480) {

        int mc = point(x, y);

        // Если тут есть мышь, то ее нарисовать вместо точки
        vga_pixel(x, y, mc ? mc : color);

        // А саму точку отправить в BB
        canvas[640*y + x] = color;
    }
}

// Реально нарисовать блок
void block_draw(int x1, int y1, int x2, int y2, unsigned char color) {

    int i, j;

    if (x1 < 0 && x2 < 0)
        return;

    if (y1 < 0 && y2 < 0)
        return;

    if ((x1 > 639 && x2 > 639) || x1 > x2)
        return;

    if ((y1 > 479 && y2 > 479) || y1 > y2)
        return;

    if (x1 > 639) x1 = 639; if (y1 > 479) y1 = 479;
    if (x2 > 639) x2 = 639; if (y2 > 479) y2 = 479;
    if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0;
    if (x2 < 0) x2 = 0; if (y2 < 0) y2 = 0;
    
    // bbuf
    for (i = y1; i <= y2; i++)
    for (j = x1; j <= x2; j++)
        canvas[640*i + j] = color;

    // На экране
    vga_block(x1, y1, x2, y2, color);
}

// Нарисовать блок :: очищается I=0, чтобы не вызвать прерывание мыши
void block(int x1, int y1, int x2, int y2, unsigned char color) {

    cli;
    int i, j;
    
    // Мышь показана, применить другой метод рисования блока
    if (cursor.mouse_show) {
        
        for (i = y1; i <= y2; i++)
        for (j = x1; j <= x2; j++)
            pset(j, i, color);        

    } else {
        
        block_draw(x1, y1, x2, y2, color);
    }
    sti;
}
// ---------------------------------------------------------------------

/** Печать символа на экране в режиме телетайпа
 * @param x, y позиция в пикселях
 * @param symb символ
 * @param color, bgcolor цвета
 */
void print_char(unsigned char chr) {

    int x = cursor.x;
    int y = cursor.y;

    int i, j, f = chr * 16;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 8; j++) {

            if (disp_vga_8x16_font[ f + i ] & (1 << (7 - j))) {
                pset(x + j, y + i, cursor.frcolor);

            } else if (cursor.bgcolor != -1) {
                pset(x + j, y + i, cursor.bgcolor);
            }
        }
    }

    cursor.x += 8;
}

// Распознать символ UTF8
unsigned char get_utf8(char** m) {

    unsigned char chr = **m;

    // Преобразовать UTF-8 в RUS
    if (chr == 0xD0) {

        (*m)++;
        chr = (**m) - 0x10;

    } else if (chr == 0xD1) {

        (*m)++;
        chr = (**m);
        chr = chr + (chr < 0xB0 ? 0x60 : 0x10);
    }

    return chr;
}

/** Пропечатать utf8 строку (прозрачную/непрозрачную)
 * @param bgcolor (-1 --  использовать прозрачноть)
 * @param maxchar
 */
int print(char* m) {

    int num = 0;
    unsigned char chr;

    while (*m) {

        chr = get_utf8(& m); m++; num++;
        print_char(chr);

        if (cursor.max_chars && num >= cursor.max_chars) {
            return 0;
        }
    }

    return num;
}

/** Печать строки по по (X,Y)
 */
int print_xy(char* m, int x, int y) {

    cursor.x = x;
    cursor.y = y;

    return print(m);
}

/** Положение мыши
 * */
void set_mouse_xy(int x, int y) {

    cursor.mouse_x = x;
    cursor.mouse_y = y;
}
