#ifndef LEXER_H
#define LEXER_H

#include "hash.h"

#include <stdio.h>

enum TokenType {
    EOF_TT,
    INVALID_TT,
    EOS_TT, /* End of statement */
    IDENT_TT,
    CHAR_TT,
    INT_TT,
    FLT_TT,
    STR_TT,
    LPAREN_TT,
    RPAREN_TT,

    LET_TT,
    FUNC_TT,
    EQ_TT,

    PLUS_TT,
    MINUS_TT,
    DIV_TT,
    MUL_TT,

    BOOL_EQ_TT,
    BOOL_NE_TT,
    BOOL_LT_TT,
    BOOL_GT_TT,
    BOOL_LE_TT,
    BOOL_GE_TT,
    BOOL_OR_TT,
    BOOL_AND_TT,
    NOT_TT,

    TRUE_TT,
    FALSE_TT,
};

struct Token {
    char* filename;
    int line;
    int column;

    char* Contents;
    enum TokenType Type;
};

struct Lexer {
    FILE* fp;

    char* filename;
    int curLine;
    int curColumn;

    struct Map* singleCharMap;
    struct Map* doubleCharMap;
    struct Map* keywordMap;
};

struct Lexer newLexer(char* filename);
void deleteLexer(struct Lexer* l);

struct Token newToken(struct Lexer* l);
void deleteToken(struct Token* t);
void copyToken(struct Token* dest, struct Token* src);

void printToken(struct Token* t);

#define lexer_panic(LEXER, ...) \
    panic(LEXER->curLine, LEXER->curColumn, LEXER->filename, __VA_ARGS__)
    
#endif