
#include "kernel/kernel.h"
#include "kernel/mm.c"
#include "kernel/pic.c"
#include "kernel/vga.c"
#include "kernel/canvas.c"
#include "kernel/gui.c"
#include "kernel/keyboard.c"
#include "kernel/ps2mouse.c"
#include "kernel/core.c"

// ---------------------------------------------------------------------

void main() {

    kernel_init();
    
    vga_init();
    cls(3);

    mouse_show(1);

    // --
    int win = window_create(0, 0, 640, 452, "Управление");
    window_activate(win); window_repaint(win);
    // --

    // Печатаем, сколько памяти
//char t[24]; i2a(mem_max>>20, t); print_xy(t, 4, 28);

    panel_repaint();

    sti; for(;;);
}
