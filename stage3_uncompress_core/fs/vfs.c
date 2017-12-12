#include "vfs.h"

// --------- InitRD FS ------------
#include "initrd.h"
#include "initrd/file_open.c"

#include "fopen.c"

// Искать не занятый файловый дескриптор
uint32_t vfs_search_free_descriptor() {
    
    uint32_t id, i;
    
    for (i = 1; i < 256; i++) {
        if (file_descriptors[i] == 0) {
            return i;
        }
    }
    
    return 0;    
}
