
#include "ast.h"
#include "parser.h"
#include "test.h"

void testParsePrimatives() {
    struct Parser p = newParser("testsrc/primatives.trtl");

    ParseStmt(&p);
    printNode(p.curNode);
    assert(p.curNode->nt == INT_NT && p.curNode->as.Int->Value == 10, "Integer Parsing");

    ParseStmt(&p);
    printNode(p.curNode);
    assert(p.curNode->nt == FLT_NT && p.curNode->as.Flt->Value == 11.1, "Floating Point Parsing");

    ParseStmt(&p);
    printNode(p.curNode);
    assert(p.curNode->nt == CHR_NT && p.curNode->as.Chr->Value == 'c', "Character Parsing");

    deleteParser(p);
}

void testInfix() {
    struct Parser p = newParser("testsrc/binop1.trtl");
    ParseStmt(&p);
    printNode(p.curNode);
}

void test() {
    struct Parser p = newParser("testsrc/test.trtl");
    ParseStmt(&p);
    printNode(p.curNode);
}

int main() {
    //testParsePrimatives();
    test();
}