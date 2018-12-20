
#include "kernel/kernel.h"
#include "kernel/fs.h"
#include "kernel/task.h"
#include "kernel/core.h"

// Графика
#include "kernel/vga.c"
#include "kernel/canvas.c"
#include "kernel/gui.c"

// Устройства
#include "kernel/keyboard.c"
#include "kernel/ps2mouse.c"

// Ядро
#include "kernel/fs.c"
#include "kernel/mm.c"
#include "kernel/pic.c"
#include "kernel/task.c"
#include "kernel/core.c"

// ---------------------------------------------------------------------

void main() {

    init();

    cls(3);
    mouse_show(1);
    
    fs_open(0);
    
    //
    // fs_open() -- открыть корневой каталог в фс
    // fs_rewind() -- перейти к первому файлу
    // fs_next() -- перейти к следующему (если есть, то =1)
    // fs_find() -- найти файл в каталоге
    // fs_enter() -- зайти в каталог
    
    // fopen()
    // fseek()
    // ftell()
    // fread() | fwrite()
    // fclose()
    

    // --
    int win = window_create(0, 0, 640, 452, "Управление");
    window_activate(win); window_repaint(win);
    // --
    
    int i, j, k;
    
    uint8_t sector[512];
    
    colorat(4,30,0,-1); 
    
    at(4,30+0*16); print_int(fatfs[0].root_cluster);
    
    /*    
    for (k = 0x800; k <= 0x800; k++) {
    
        if (drive_read_sectors(sector, 0, k, 1)) {
            colorat(4,24,0,-1); print("ERROR READ");
        }    
        
        colorat(4,24,0,7); print_int(k);
        
        for (i = 0; i < 16; i++) {
            colorat(4, i*16+40, 7, 1);
            for (j = 0; j < 16; j++) {
                 print_hex8(sector[j+16*i]);
                 print(" ");
                 
                 
                 //if (sector[j+16*i]) for(;;);
            }
        }
        
        break;
    }
    */

    panel_repaint();

    sti; for(;;);
}
