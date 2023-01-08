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
    LET_NT,
    IDENT_NT,
    ASSIGN_NT,
    LIST_NT,
    BUILTIN_NT,
};

/* Type the node returns when evaluated */
enum BaseType {
    INVALID_BT, /* Returned when a value is invalid */
    NULL_BT, /* Statements will use this as their return type */
    INT_BT,
    BOOL_BT,
    FLT_BT,
    CHR_BT,
    LIST_BT,
};

struct ValueType {
    enum BaseType base_type;

    union {
        struct ValueType* ListEntryType;
    } subtype_info;
};

struct ValueType* newType(enum BaseType bt);
void deleteType(struct ValueType* vt);
bool typesMatch(struct ValueType* v1, struct ValueType* v2);
bool isTypeComplex(struct ValueType* v1);

struct Node {
    union {
        struct IntNode* Int;
        struct FltNode* Flt;
        struct ChrNode* Chr;
        struct BinOp* BinOp;
        struct BoolNode* Bool;
        struct IfElseNode* IfEl;
        struct WhileNode* While;
        struct LetNode* Let;
        struct IdentifierNode* Ident;
        struct ListNode* List;
        struct AssignNode* Assign;
        struct BuiltinNode* Builtin;
    } as;

    struct Token* tok;
    enum NodeType nt;
    struct ValueType* vt;
};

struct Block {
    struct Node** Statements;
    int numStatements;

    /* Number of variables in this block's private scope */
    int numPrimativeVarsInScope; //Used to pop stack frame
    int numCompositeVarsInScope;
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

struct LetNode {
    char* Identifier;
    struct Node* Value;
    long long StackLocation;
};

struct IdentifierNode {
    char* Identifier;
    long long StackLocation;
};

struct AssignNode {
    struct Node* Value;
    long long StackLocation;
};

struct ListNode {
    struct Node** Values;
    int numValues;
};

struct BuiltinNode {
    char* builtinName;
    int numArgs;
    struct Node** Args;
};

void deleteNode(struct Node* n);
void printNode(struct Node* n);

#define node_panic(NODE, ...) \
    panic(NODE->tok->line, NODE->tok->column, NODE->tok->filename, __VA_ARGS__)

#endif