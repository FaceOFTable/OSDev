#include "put_bmp.h"

int abs(int __x) {    
    return __x < 0 ? -__x : __x;
}

// -------
uint8_t colortable_16[48] = {
    
    0, 0, 0,
    0, 0, 128,
    0, 128, 0,
    0, 128, 128,
    128, 0, 0,
    128, 0, 128,
    128, 128, 0,
    192, 192, 192,
    128, 128, 128,
    0, 0, 255,
    0, 255, 0,
    0, 255, 255,
    255, 0, 0,
    255, 0, 255,
    255, 255, 0,
    255, 255, 255,

};

/*
 * Рисование BMP
 */
 
void ui_put_bmp(uint32_t addr, int x, int y, uint32_t opacity) {
    
    int i, j, k;
    
    if (addr == 0) {
        return;
    }

    // Где находятся пиксельные данные
    uint32_t pix = addr + mm_readd(addr + 0x0A);
    
    // ... 12 пропуск
    if (mm_readw(addr + 0x0E) == 12) {
        return;
    }
    
    uint16_t width  = mm_readd(addr + 0x12);
    uint16_t height = mm_readd(addr + 0x16);
    uint16_t bits   = mm_readw(addr + 0x1C);
    uint32_t color_table = addr + 0x0E + mm_readw(addr + 0x0E);
    
    // @todo Сделать потом корректирующую буферизацию для floyd-steinberg
    
    // 16 цветов
    if (bits == 4) {
   
        for (i = 0; i < height; i++) {

            for (j = 0; j < width; j++) {
                
                if (x + j < 640 && i + y < 480) {                            
                
                    uint32_t pixel = (height - i - 1)*width + j;
                    uint32_t shift = pixel >> 1;
                    uint8_t  color = mm_readb(pix + shift);
                    
                    if (pixel % 2) {  
                        color &= 0xf;
                    } else {
                        color >>= 4;                                                      
                    }
                    
                    if (opacity == color) {
                        continue;
                    }
      
                    uint8_t r = mm_readb(color_table + color*4 + 2);
                    uint8_t g = mm_readb(color_table + color*4 + 1);
                    uint8_t b = mm_readb(color_table + color*4 + 0);
                    
                    uint8_t closest = 0;
                    uint32_t closest_diff = -1;

                    // поиск ближайшего цвета
                    for (k = 0; k < 16; k++) {

                        // Расчет методом квадратного приближения
                        int diff = (colortable_16[k*3 + 0] - r) * (colortable_16[k*3 + 0] - r) + 
                                   (colortable_16[k*3 + 1] - g) * (colortable_16[k*3 + 1] - g) +
                                   (colortable_16[k*3 + 2] - b) * (colortable_16[k*3 + 2] - b);
                                            
                        if (closest_diff > diff) {
                            closest_diff = diff;
                            closest = k;
                        } 
                    }

                    display_vga_pixel(x + j, y + i, closest);
                }
            }
        }
        
    } else if (bits == 8) {
        
        // ..
        
    }
    
}
