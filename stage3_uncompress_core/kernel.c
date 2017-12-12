#include "kernel.h"
#include "kernel/pic_redirect.c"
#include "kernel/isr_init.c"
#include "kernel/pic_keyb.c"
#include "display/util.c"
#include "display/vga.c"
#include "kernel/init.c"
#include "mm/palloc.c"
#include "ui/start.c"

// ---------------------------------------------------------------------

void main() {

    kernel_init();
    kernel_pic_redirect(IRQ_KEYB);
    kernel_isr_init();        

    data_alloc = palloc(); // выделяется страница из пустого каталога
    // data_alloc = palloc(); // выделается новая страница из того же каталога
    
    ui_start();

    sti;
    for(;;);
}
