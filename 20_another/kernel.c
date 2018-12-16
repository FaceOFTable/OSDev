
#include "kernel/kernel.h"
#include "kernel/pic.c"
#include "kernel/vga.c"
#include "kernel/keyboard.c"

void main() {

    irq_init(IRQ_KEYB);
    
    vga_mode(VGA_640x480);
    vga_cls(1);    
    vga_putf8(0,0,"Привет, UTF8",15);
    
    sti;

    for(;;);
}
