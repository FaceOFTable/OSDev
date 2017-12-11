#include "kernel.h"
#include "kernel/pic_redirect.c"
#include "display/util.c"
#include "ui/start.c"
#include "app/emulator.c"

// ---------------------------------------------------------------------

void main() {
    
// brk;

    // app_emulator_init();
    kernel_pic_redirect(0);
    
    ui_start();

    for(;;);
}
