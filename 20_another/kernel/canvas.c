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

    vga_cls(color);
}

// Нарисовать точку как на экране, так и в back-буфере
void pset(int x, int y, unsigned char color) {

    if (x >= 0 && x < 640 && y >= 0 && y < 480) {

        vga_pixel(x, y, color);
        canvas[640*y + x] = color;
    }
}

// Нарисовать блок
void block(int x1, int y1, int x2, int y2, unsigned char color) {

    int i, j;

    if ((x1 > 639 && x2 > 639) || x1 > x2)
        return;

    if ((y1 > 479 && y2 > 479) || y1 > y2)
        return;

    if (x1 > 479) x1 = 639; if (y1 > 479) y1 = 479;
    if (x2 > 479) x2 = 639; if (y2 > 479) y2 = 479;

    // bbuf
    for (i = y1; i <= y2; i++)
    for (j = x1; j <= x2; j++)
        canvas[640*i + j] = color;

    // На экране
    vga_block(x1, y1, x2, y2, color);
}

/** Печать символа на экране
 * @param x, y позиция в пикселях
 * @param symb символ
 * @param color, bgcolor цвета
 */
void print_char(int x, int y, unsigned char symb, char color, int bgcolor) {

    int i, j, f = symb * 16;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 8; j++) {

            if (disp_vga_8x16_font[ f + i ] & (1 << (7 - j))) {
                pset(x + j, y + i, color);

            } else if (bgcolor != -1) {
                pset(x + j, y + i, bgcolor);
            }
        }
    }
}

/** Пропечатать utf8 строку (прозрачную/непрозрачную)
 * @param bgcolor (-1 --  использовать прозрачноть)
 * @param maxchar
 */
int print_utf8(int x, int y, char* m, int color, int bgcolor, int maxchar) {

    int num = 0;
    unsigned char chr;

    while (*m) {

        chr = *m;

        // Преобразовать UTF-8 в RUS
        if (chr == 0xD0) {

            m++;
            chr = (*m) - 0x10;

        } else if (chr == 0xD1) {

            m++;
            chr = (*m);
            chr = chr + (chr < 0xB0 ? 0x60 : 0x10);
        }

        // Псевдографика недоступна
        print_char(x, y, chr, color, bgcolor);

        x += 8;

        // Overflows
        if (x > 631) {

            x = 0;
            y += 16;
            if (y > 463) {
                y = 464;
            }
        }

        m++;
        num++;

        if (maxchar && num >= maxchar) {
            return 0;
        }
    }

    return num;
}
