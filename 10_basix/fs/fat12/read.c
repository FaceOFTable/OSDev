/*
 * Последовательное скачивание файла по кластерам
 */

void fs_fat12_read(int file_id, uint32_t size) {
    
    // ..
    
}

/*
 * Загрузка файла целиком
 */
 
uint32_t fs_fat12_load(uint16_t file_id) {
    
    // Выделить пространство
    uint32_t addr = kalloc(fat12_desc[ file_id ].file_size);
    
    
    return addr;
}
