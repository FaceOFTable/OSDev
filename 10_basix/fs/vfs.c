#include "vfs.h"
#include "fat12.h"

/*
 * Инициализация файловых дескрипторов в NULL
 */

void fs_init() {
    
    int i;
    
    // Размаппить на RAM-диск, т.к. реального диска все равно не будет
    for (i = 0; i < VFS_MAX_FILES; i++) { 
               
        fat12_desc[i].busy = 0;
        
        // Указатель на ROOT-директорию
        fat12_desc[i].cluster_dir = 0;
        
        // Все FAT12 находятся в RAM
        fat12_desc[i].device_id = FAT12_DEVICE_RAM;
    }    
    
}
