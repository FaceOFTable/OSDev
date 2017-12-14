#include "put_bmp.h"

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

    // Построение таблицы преобразований (только 16 или 256)
    if (bits == 4 || bits == 5) {

        for (i = 0; i < (bits == 4 ? 16 : 256); i++) {
            
            uint8_t  closest = 0;
            uint32_t closest_diff = -1;
            uint32_t diff;

            for (j = 0; j < 16; j++) {
                          
                 diff = color_distance(
                    mm_readb(color_table + i*4 + 2),    
                    mm_readb(color_table + i*4 + 1),    
                    mm_readb(color_table + i*4 + 0),    
                    vga_palette_16[j*3 + 0],
                    vga_palette_16[j*3 + 1],
                    vga_palette_16[j*3 + 2]
                );
                
                if (closest_diff > diff) {
                    closest_diff = diff;
                    closest = j;
                }                                        
            }
            
            bmp_palette_convert[i] = closest;
        }
    }
    
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

                    display_vga_pixel(x + j, y + i, bmp_palette_convert[ color ]);
                }
            }
        }
        
    } else if (bits == 8) {
        
        // ..
        
    }
    
}
