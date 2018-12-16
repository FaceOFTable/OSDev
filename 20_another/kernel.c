
#include "kernel/kernel.h"
#include "kernel/pic.c"
#include "kernel/vga.c"
#include "kernel/keyboard.c"

#include "ui/start.c"

void main() {

    irq_init(IRQ_KEYB);
    
    ui_start();

    sti;

    for(;;);
}
