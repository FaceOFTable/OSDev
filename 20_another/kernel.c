
#include "kernel/kernel.h"
#include "kernel/mm.c"
#include "kernel/pic.c"
#include "kernel/vga.c"
#include "kernel/gui.c"
#include "kernel/keyboard.c"

void main() {

    irq_init(IRQ_KEYB);

    struct window win1, win2;

    // Создаем окно
    window_init(& win1, 16, 16, 320, 240, "Windows рулит всегда и везде");
    window_init(& win2, 150, 300, 320, 140, "Второе окно тоже кульное");

    vga_mode(VGA_640x480);
    vga_cls(3);

    window_repaint(& win1, 1);
    window_repaint(& win2, 0);
    panel_repaint();

    vga_putf8(22, 40, "Необходимо срочно же откинуться на", 0);
    vga_putf8(22, 52, "спинку кресла и начать созерцать ОС", 0);

    sti;

    for(;;);
}
