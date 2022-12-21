#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "panic.h"
#include "common_types.h"

enum NodeType {
    INT_NT,
    FLT_NT,
    CHR_NT,
    BOOL_NT,
    IFEL_NT,
    BINOP_NT,
    WHILE_NT,
};

/* Type the node returns when evaluated */
enum ReturnType {
    INVALID_RT, /* Returned when a value is invalid */
    NULL_RT, /* Statements will use this as their return type */
    INT_RT,
    BOOL_RT,
    FLT_RT,
    CHR_RT,
};

struct Node {
    union {
        struct IntNode* Int;
        struct FltNode* Flt;
        struct ChrNode* Chr;
        struct BinOp* BinOp;
        struct BoolNode* Bool;
        struct IfElseNode* IfEl;
        struct WhileNode* While;
    } as;

    struct Token* tok;
    enum NodeType nt;
    enum ReturnType rt;
};

struct Block {
    struct Node** Statements;
    int numStatements;
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

struct IfElseNode {
    int numBlocks;
    struct Node** Conditions;
    struct Block** Blocks;
    struct Block* ElseBlock;
};

struct WhileNode {
    struct Block* Block;
    struct Node* Condition;
    bool isDoWhile;
};

void deleteNode(struct Node* n);
void printNode(struct Node* n);

#define node_panic(NODE, ...) \
    panic(NODE->tok->line, NODE->tok->column, NODE->tok->filename, __VA_ARGS__)

#endif