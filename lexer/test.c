#include "lexer.h"

void assert(int expr, char* name) {
    if (expr)
        printf("%s Passed\n", name);
    else {
        printf("%s Failed\n", name);
    }
}

void testSingleChar() {
    struct Lexer l = newLexer("testsrc/test_singleChar.trtl");
   
    struct Token t = newToken(&l);
    assert(t.Type == LPAREN, "LPAREN");
    
    struct Token t2 = newToken(&l);
    assert(t2.Type == RPAREN, "RPAREN");
    
    deleteLexer(&l);
}

void testDoubleChar() {
    struct Lexer l = newLexer("testsrc/test_doubleChar.trtl");
   
    struct Token t = newToken(&l);
    assert(t.Type == LPAREN, "LPAREN2");
    
    struct Token t2 = newToken(&l);
    assert(t2.Type == BOOL_EQ, "BOOL_EQ");

    struct Token t3 = newToken(&l);
    assert(t3.Type == RPAREN, "RPAREN2");
    
    deleteLexer(&l);
}

int main() {
    testSingleChar();
    testDoubleChar();
    return 0;
}