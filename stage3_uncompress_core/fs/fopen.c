/*
 * Найти и открыть файл
 */
  
struct FILE fopen(const char* __filename, const char* __modes) {
    
    struct FILE file;
brk;
    // Определить, что доступ идет к initrd, другие блокировать
    if (cmpstr(__filename, "/initrd/")) {
        
        file.fs_type = FILE_FSTYPE_INITRD;
        file.file_id = fs_initrd_file_open(__filename + 7);        
        file.status = FILE_STATUS_OPEN;
        
    } else {
        
        file.fs_type = FILE_FSTYPE_NONE;
        file.status = FILE_STATUS_ERROR;
    }
    
    return file;    
}
