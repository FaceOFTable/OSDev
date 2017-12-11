#include "kernel.h"
#include "kernel/pic_redirect.c"
#include "kernel/isr_init.c"
#include "kernel/pic_keyb.c"
#include "display/util.c"
#include "ui/start.c"
#include "kernel/init.c"

// ---------------------------------------------------------------------

void main() {

    kernel_init();
    kernel_pic_redirect(IRQ_KEYB);
    kernel_isr_init();    
    sti;
    
    ui_start();
    for(;;);
}
