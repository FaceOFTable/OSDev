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
    
    display_vga_cls(1);
    
    int i; 
    for (i = 0; i < 80; i++) display_vga_pchar(i*8,0,i+128,15);
    for (i = 80; i < 128; i++) display_vga_pchar((i-80)*8,16,i+128,15);
    
    // display_vga_print(0, 0, "Технически невозможно", 15);
    
    //ui_start();
    for(;;);
}
