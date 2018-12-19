
#include "kernel/kernel.h"

// Графика
#include "kernel/vga.c"
#include "kernel/canvas.c"
#include "kernel/gui.c"

// Устройства
#include "kernel/keyboard.c"
#include "kernel/ps2mouse.c"

// Ядро
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


    // Печатаем, сколько памяти
//char t[24]; i2a(g, t); print_xy(t, 4, 28);

    panel_repaint();

    sti; for(;;);
}
