#include "kernel.h"
#include "kernel/pic_redirect.c"
#include "kernel/isr_init.c"
#include "kernel/pic_keyb.c"
#include "display/util.c"
#include "display/vga.c"
#include "kernel/init.c"
#include "ui/start.c"

// ---------------------------------------------------------------------

void main() {

    kernel_init();
    kernel_pic_redirect(IRQ_KEYB);
    kernel_isr_init();    

    display_vga_mode(VGA_640x480);
    display_vga_cls(1);        
    display_vga_putf8(1, 1, "Вас приветствует программа установки системы", 11);
    display_vga_putf8(1, 3, "Откиньтесь на спинку кресла и наслаждайтесь бесконечностью установки ОС.", 7);
    display_vga_putf8(1, 4, "Поверьте, вы заслужили отдых от трудов и я, компьютер, о вас позабочусь!", 7);
    display_vga_putf8(1, 5, "Пожалуйста, наслаждайтесь красивой картинкой, пока я делаю установку.", 7);
  
    sti;
    
    //ui_start();
    for(;;);
}
