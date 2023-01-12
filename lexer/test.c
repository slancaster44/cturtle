#include "lexer.h"


int main() {
    struct Lexer l = newLexer("testsrc/test.trtl");
    struct Token t = newToken(&l);
    while (t.Type != EOF_TT) {
        printf("%s\n", t.Contents);
        t = newToken(&l);
    }
    return 0;
}