
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

    init(); cls(3); mouse_show(1);

    /* fs_init(0); int m = fs_find("coreboot.bin"); */

    // int win = window_create(0, 0, 640, 452, "Управление");
    // window_activate(win); window_repaint(win);

    palloc(4096);

    panel_repaint();

    sti; for(;;);
}
