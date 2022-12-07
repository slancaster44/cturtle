#include "tbin.h"
#include "instructions.h"
#include "test.h"

void testWriteCode() {
    struct TurtleBinary* tb = newBinary();
    byte code[10] = {
        LDA_IMM,
        0x08,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    writeCode(tb, code, 10);
    
    byte code2[1] = {
        EXIT,
    };
    writeCode(tb, code2, 1);

    assert(tb->code[10] == EXIT, "WRITE_CODE");
    deleteTurtleBinary(tb);
}

int main() {
    testWriteCode();
    return 0;
}
