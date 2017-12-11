enum OPCODE_NAMES { 
    OPC_UNK,    // Неизвестно
    OPC_PRX,    // Префикс
    OPC_ALU,    // <alu> modrm
    OPC_ALUI,   // <alu> A, imm
    OPC_PUSHS,  // Сегментный регистр
    OPC_PUSH,
    OPC_POP,
    OPC_POPS,
    OPC_DCM,    // AAA, DAS, AAS, AAS
    OPC_INC,    // Регистр 16/32
    OPC_DEC,
    OPC_JCC,
    OPC_GR1,    // grp арифметические
    OPC_TEST,   // test modrm
    OPC_XCH,    // xchg modrm
    
};

static const int dis_opcode_tables[ 0x80 ] = {
    
    /* 00 */ OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALUI, OPC_ALUI, OPC_PUSHS, OPC_POPS,
    /* 08 */ OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALUI, OPC_ALUI, OPC_PUSHS, OPC_PRX,
    /* 10 */ OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALUI, OPC_ALUI, OPC_PUSHS, OPC_POPS,
    /* 18 */ OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALUI, OPC_ALUI, OPC_PUSHS, OPC_POPS,    
    /* 20 */ OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALUI, OPC_ALUI, OPC_PRX,   OPC_DCM,
    /* 28 */ OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALUI, OPC_ALUI, OPC_PRX,   OPC_DCM,
    /* 30 */ OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALUI, OPC_ALUI, OPC_PRX,   OPC_DCM,
    /* 38 */ OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALU,  OPC_ALUI, OPC_ALUI, OPC_PRX,   OPC_DCM,
    /* 40 */ OPC_INC,  OPC_INC,  OPC_INC,  OPC_INC,  OPC_INC,  OPC_INC,  OPC_INC,   OPC_INC,
    /* 48 */ OPC_DEC,  OPC_DEC,  OPC_DEC,  OPC_DEC,  OPC_DEC,  OPC_DEC,  OPC_DEC,   OPC_DEC,
    /* 50 */ OPC_PUSH, OPC_PUSH, OPC_PUSH, OPC_PUSH, OPC_PUSH, OPC_PUSH, OPC_PUSH,  OPC_PUSH,
    /* 58 */ OPC_POP,  OPC_POP,  OPC_POP,  OPC_POP,  OPC_POP,  OPC_POP,  OPC_POP,   OPC_POP,
    /* 60 */ OPC_UNK,  OPC_UNK,  OPC_UNK,  OPC_UNK,  OPC_UNK,  OPC_UNK,  OPC_UNK,   OPC_UNK,
    /* 68 */ OPC_JCC,  OPC_JCC,  OPC_JCC,  OPC_JCC,  OPC_JCC,  OPC_JCC,  OPC_JCC,   OPC_JCC,
    /* 70 */ OPC_JCC,  OPC_JCC,  OPC_JCC,  OPC_JCC,  OPC_JCC,  OPC_JCC,  OPC_JCC,   OPC_JCC,
    /* 78 */ OPC_GR1,  OPC_GR1,  OPC_GR1,  OPC_GR1,  OPC_TEST, OPC_TEST, OPC_XCH,   OPC_XCH,
    
};
