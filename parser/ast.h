#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef int bool;

enum NodeType {
    INT_NT,
    FLT_NT,
    CHR_NT,
    BOOL_NT,

    BINOP_NT,
};

struct Node {
    union {
        struct IntNode* Int;
        struct FltNode* Flt;
        struct ChrNode* Chr;
        struct BinOp* BinOp;
        struct BoolNode* Bool;
    } as;

    struct Token* tok;
    enum NodeType nt;
};

struct IntNode {
    int Value;
};

struct FltNode {
    double Value;
};

struct ChrNode {
    char Value;
};

struct BinOp {
    struct Node* LHS;
    struct Node* RHS;
    char* Op;
};

struct BoolNode {
    bool Value;
};

void deleteNode(struct Node* n);
void printNode(struct Node* n);

#endif