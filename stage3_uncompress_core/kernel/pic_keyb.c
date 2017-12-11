// Обработка нажатии клавиатуры
void kernel_pic_keyb() {
    
    brk;

    IoRead8(0x60);
    
    
}
