
#include "kernel.h"
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

    fs_init(0);
    int m = fs_find("coreboot.bin");

    mouse_show(1);

    // --
    int win = window_create(0, 0, 640, 452, "Управление");
    window_activate(win); window_repaint(win);
    // --

    int i, j, k;

    uint8_t sector[512];

    colorat(4,30,0,-1);

    at(4,30+0*16); print_int(m);
    at(4,30+1*16); print((char*)fs.filename);
    at(4,30+2*16); print_int((uint32_t)(mem_max - mem_lower)>>10);    
    
    // Проверить 
    at(4,30+4*16); print_hex32( get_pte(0x7FF2000) );    

    panel_repaint();

    sti; for(;;);
}
