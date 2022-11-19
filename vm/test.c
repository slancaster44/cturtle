#include "vm.h"
#include "instructions.h"

int standardTimeTest() {
        byte code[39] = {
        LDA_IMM,
        0x0F,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        LDB_IMM,
        0xF1,
        0xFF,
        0xFF,
        0x0E,
        0x00,
        0x00,
        0x00,
        0x00,
        ADD_A_IMM,
        0x0F,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        BUILTIN,
        DUMP_REGS,
        EXIT,
    };
    Execute(code, 39);
}

int main() {
    standardTimeTest();

    return 0;
}