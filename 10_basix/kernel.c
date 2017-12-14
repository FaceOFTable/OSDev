#include "kernel.h"
#include "kernel/strings.h"
#include "kernel/strings.c"
#include "mm/helpers.c"
#include "mm/palloc.c"
#include "mm/kalloc.c"
#include "fs/vfs.c"
#include "kernel/pic_redirect.c"
#include "kernel/isr_init.c"
#include "kernel/pic_keyb.c"
#include "kernel/init.c"
#include "display/util.c"
#include "display/vga.c"
#include "ui/start.c"

// ---------------------------------------------------------------------

void main() {

    kernel_init();
    kernel_pic_redirect(IRQ_KEYB);
    kernel_isr_init();        

    fs_init();

    int fd = fs_fat12_open("/walls/main.gif");
brk;    
    uint32_t m = fs_fat12_load(fd);
    
    ui_start();

    sti;
    for(;;);
}
