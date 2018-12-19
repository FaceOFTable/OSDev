
#include "kernel/kernel.h"
#include "kernel/fs.h"
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
#include "kernel/core.c"
#include "kernel/task.c"

// ---------------------------------------------------------------------

void main() {

    init();

    cls(3);
    mouse_show(1);

    // --
    int win = window_create(0, 0, 640, 452, "Управление");
    window_activate(win); window_repaint(win);
    // --
    
    int i, j;
    /*
    for (i = 0; i < 16; i++) {
        
        colorat(4, i*16+32, 7, 0);
        for (j = 0; j < 16; j++) {
             print_hex8(drive[0].identify[j+16*i]);
             print(" ");
        }
    }
    */
    
    colorat(4,400,0,-1);
    print_int(fat_found);

    // 

    panel_repaint();

    sti; for(;;);
}
