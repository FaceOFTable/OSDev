
/*
 * Эмулятор x86 для выполнения инструкции BIOS
 */
 
enum SEGMENTS {
    SEG_ES,
    SEG_CS,
    SEG_SS,
    SEG_DS,
};
 
uint8_t  emu_cbuffer[16]; // Буфер кода
uint32_t rmode_intr[256]; // Список прерываний
uint32_t emu_register[8] = {0, 0, 0, 0, 0, 0, 0, 0};    // EAX  ECX  EDX  EBX   ESP  EBP  ESI  EDI
uint16_t emu_segments[6] = {0, 0, 0, 0, 0, 0};          // ES CS SS DS FS GS
uint32_t emu_eip = 0;
uint32_t emu_eflags = 0;

#include "emulator/disasm.h"
#include "emulator/disasm.c"

// Перенос прерываний реального режима в таблицу
void app_emulator_init() {
    
    int id;    
    
    // ЭТО ХАК! UD2A инструкцию Clang вставляет, если =0
    uint32_t* addr = (uint32_t*)4;
    
    for (id = 0; id < 256; id++) {        
        rmode_intr[ id ] = addr[id - 1];
    }
}

// Получение 16 байт для анализа опкода
void ae_fetch() {
    
    // ..
    
}

// Симуляция вызова INT
void ae_intr(int int_id) {
    
    // Записать CS:IP прерывания
    emu_eip = rmode_intr[ int_id ] & 0xffff;
    emu_segments[ SEG_CS ] = (rmode_intr[ int_id ] >> 16) & 0xffff;
    
}

// Показать дизассемблерный листинг
void app_emulator() {
    
    brk;
    
    ae_intr(0x10);
    
}
