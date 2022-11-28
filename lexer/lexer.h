#ifndef LEXER_H
#define LEXER_H

#include "hash.h"

#include <stdio.h>

enum TokenType {
    INVALID,
    EOS, /* End of statement */
    IDENT,
    CHAR,
    INT,
    FLT,
    STR,
    LPAREN,
    RPAREN,
    BOOL_EQ,
    BOOL_NE,
    LET,
    FUNC,
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

void printToken(struct Token* t);
#endif