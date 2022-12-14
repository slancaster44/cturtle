#include "codegen.h"
#include "instructions.h"
#include "test.h"
#include "tdb.h"

void test() {
    struct CodeGenerator* cg = newCodeGenerator("testsrc/primatives.trtl");
    compileCurrentStatement(cg);
    byte expected[9] = {LDA_IMM, list8bitImm(0x0A)};
    assertArrayEquals(cg->currentStatementCode, expected, 9, "Load Integer");
    deleteCodeGenerator(cg);

    cg = newCodeGenerator("testsrc/binop1.trtl");
    compileCurrentStatement(cg);
    DebugRawByteCode(cg->currentStatementCode, cg->codeSize);
    deleteCodeGenerator(cg);
}

int main() {
    test();
    return 0;
};