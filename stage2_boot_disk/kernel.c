#include "decl/kernel.h"
#include "kernel/io_redirect.c"
#include "kernel/io_display_util.c"
#include "app/start_interface.c"

// ---------------------------------------------------------------------

void main() {
    
    int i;

    kernel_io_redirect(0);

    app_start_interface();
    
    
    for(;;);
}
