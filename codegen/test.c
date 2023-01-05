

#include "codegen.h"
#include "tdb.h"

int main() {
    struct CodeObj code = Compile("testsrc/test.trtl");
    DebugRawByteCode(code.code, code.codelen);
    return 0;
}