
#include "kernel/kernel.h"
#include "kernel/mm.c"
#include "kernel/pic.c"
#include "kernel/vga.c"
#include "kernel/gui.c"
#include "kernel/keyboard.c"

void main() {

    irq_init(IRQ_KEYB);
    // detect_memory_size();

    vga_init();
    vga_cls(3);
    gui_init();

    // Создать 2 окна
    int win1 = window_create(16, 16, 320, 240, "Windows рулит всегда и везде");
    int win2 = window_create(150, 300, 320, 140, "Второе окно тоже кульное");

    // Активировать их
    window_activate(win1);

    // Перерисовать оба
    window_repaint(win1);
    window_repaint(win2);

    panel_repaint();

    sti; for(;;);
}
