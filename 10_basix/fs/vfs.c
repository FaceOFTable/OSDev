#include "vfs.h"
#include "fat12.h"
#include "initrd.h"

#include "fat12/open.c"

/*
 * Инициализация файловых дескрипторов в NULL
 */

void fs_init() {
    
    int i;
    
    // Размаппить на RAM-диск, т.к. реального диска все равно не будет
    for (i = 0; i < 256; i++) { 
               
        fat12_desc[i].busy = 0;
        fat12_desc[i].device_id = FAT12_DEVICE_RAM;
    }    
    
}
