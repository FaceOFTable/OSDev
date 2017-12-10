#define brk __asm__ __volatile__("xchg %bx, %bx");

// ---------------------------------------------------------------------

void main() {
    
    brk;
    
    int i;
    char* videomemory = (char*)0xB8000;
    
    for (i = 0; i < 4000; i += 2) {
        videomemory[i] = 0x41;
    }
    
    for(;;);    
}
