
#include "kernel/kernel.h"
#include "kernel/pic.c"
#include "kernel/vga.c"
#include "kernel/keyboard.c"

void main() {

    irq_init(IRQ_KEYB);
    
    vga_mode(VGA_640x480);
    vga_cls(3);    
    
    
    //vga_block(6, 15, 17, 161, 15);
    //for(;;);
    
    vga_block(15, 15, 481, 161, 0);
    vga_block(16, 16, 480, 160, 7);
    vga_block(17, 17, 479, 17+18, 1);
    vga_putf8(24, 19, "Вендеканец 95", 15);
    
    sti;

    for(;;);
}
