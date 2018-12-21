
#include "kernel/kernel.h"
#include "kernel/fs.h"
#include "kernel/mm.h"
#include "kernel/task.h"
#include "kernel/core.h"
#include "kernel/canvas.h"

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
    
    fs_init(0);
    int m = fs_find("coreboot.bin");
    
    // --
    int win = window_create(0, 0, 640, 452, "Управление");
    window_activate(win); window_repaint(win);
    // --
    
    int i, j, k;
    
    uint8_t sector[512];
    
    colorat(4,30,0,-1); 
    
    at(4,30+0*16); print_int(m);
    at(4,30+1*16); print((char*)fs.filename);
    
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
