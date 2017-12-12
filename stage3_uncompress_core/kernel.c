#include "kernel.h"
#include "kernel/pic_redirect.c"
#include "kernel/isr_init.c"
#include "kernel/pic_keyb.c"
#include "display/util.c"
#include "display/vga.c"
#include "ui/start.c"
#include "kernel/init.c"

// ---------------------------------------------------------------------

void main() {

    kernel_init();
    kernel_pic_redirect(IRQ_KEYB);
    kernel_isr_init();    

    display_vga_mode(VGA_640x480);
    
    sti;
    
    int i, j;
    for (i = 0; i < 480; i++) {
    for (j = 0; j < 640; j++) {    
        display_vga_pixel(j,i,i+j);     
    } }
    
    //ui_start();
    for(;;);
}
