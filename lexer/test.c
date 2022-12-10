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

void testKeyword() {
    struct Lexer l = newLexer("testsrc/test_keywordLex.trtl");
   
    struct Token t = newToken(&l);
    assert(t.Type == LET, "LET");
    
    struct Token t4 = newToken(&l);
    assert(t4.Type == EOS, "EOS");

    struct Token t2 = newToken(&l);
    assert(t2.Type == IDENT, "IDENT");

    newToken(&l); /* Skip over newline */

    struct Token t3 = newToken(&l);
    assert(t3.Type == FUNC, "FUNC");
    
    deleteLexer(&l);
}

void testNumber() {
    struct Lexer l = newLexer("testsrc/test_intLex.trtl");
   
    struct Token t = newToken(&l);
    assert(t.Type == INT, "INT");
    
    newToken(&l); /* Skip over whitespace */

    struct Token t4 = newToken(&l);
    assert(t4.Type == FLT, "FLT");
    
    deleteLexer(&l);
}

void testChar() {
    struct Lexer l = newLexer("testsrc/test_charLex.trtl");
   
    struct Token t = newToken(&l);
    assert(t.Type == CHAR, "CHAR");

    newToken(&l);

    struct Token t2 = newToken(&l);
    assert(t2.Type == CHAR, "CHAR2");

    deleteLexer(&l);
}

void testStr() {
    struct Lexer l = newLexer("testsrc/test_strLex.trtl");
   
    struct Token t = newToken(&l);
    assert(t.Type == STR, "STR");

    newToken(&l);

    struct Token t2 = newToken(&l);
    assert(t2.Type == STR, "STR2");

    deleteLexer(&l);
}

int main() {
    testSingleChar();
    testDoubleChar();
    testKeyword();
    testNumber();
    testChar();
    testStr();
    return 0;
}