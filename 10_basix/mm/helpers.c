// Читать BYTE из памяти
uint8_t mm_readb(uint32_t addr) {
    
    uint8_t* paddr = (uint8_t*)addr;
    return *paddr;    
}

// Читать WORD из памяти
uint16_t mm_readw(uint32_t addr) {
    
    uint8_t* paddr = (uint8_t*)addr;
    return paddr[0] + (paddr[1] << 8);
}

// Читать DWORD из памяти
uint32_t mm_readd(uint32_t addr) {
    
    uint8_t* paddr = (uint8_t*)addr;
    return paddr[0] + (paddr[1] << 8) + (paddr[2] << 16) + (paddr[3] << 24);
}
