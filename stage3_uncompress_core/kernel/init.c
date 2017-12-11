void kernel_init() {
    
    int i;
    keyb_buffer_position = 0;
    for (i = 0; i < 32; i++) {
        keyb_press[i] = 0;
    }

}
