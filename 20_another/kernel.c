
#include "kernel/kernel.h"
#include "kernel/mm.c"
#include "kernel/pic.c"
#include "kernel/vga.c"
#include "kernel/canvas.c"
#include "kernel/gui.c"
#include "kernel/keyboard.c"
#include "kernel/core.c" 

void main() {

    kernel_init();
    detect_memory_size();

    irq_init(IRQ_KEYB);

    vga_init();
    cls(3);

    // Создать 2 окна
    int win1 = window_create(16, 16, 320, 240, "Windows рулит всегда и везде");
    int win2 = window_create(150, 300, 320, 140, "Второе окно тоже кульное");

    // Активировать их
    window_activate(win1);

    // Перерисовать оба
    window_repaint(win1);
    window_repaint(win2);
    
    print_utf8(19, 39, "/dev/null:", 0, -1, 0);

    panel_repaint();

    sti; for(;;);
}
