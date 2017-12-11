/*
 * Вывод текущего положения (дизассемблирование)
 */
 
void app_emulator_disasm(uint32_t cursor) {
    
    uint8_t* zb = (uint8_t*)0;
    
    // uint8_t opcode;    
    // opcode = zb[ cursor ];
    
    display_bgblock(0, 0, 2000, 0x17);    
    display_bgblock(0, 24, 80, 0x70);
    
    display_frame(0, 0, 59, 17);
    display_frame(0, 18, 59, 23);
    display_frame(60, 0, 79, 23);

    display_text(1, 0, " Disasm ");
    display_text(61, 0, " Register ");
    display_text(1, 18, " Memory dump ");
    
}
