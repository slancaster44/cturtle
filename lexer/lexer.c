#include "lexer.h"
#include "mem_mac.h"
#include "hash.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Lexer newLexer(char* filename) {
    struct Lexer l;
    
    l.filename = new_array(char, strlen(filename));
    strcpy(l.filename, filename);
    
    l.curColumn = 1;
    l.curLine = 1;

    l.fp = fopen(filename, "r");
    if (!l.fp) {
        printf("Could not open file '%s'\n", filename);
        exit(1);
    }

    l.singleCharMap = newMap();
    setPair(l.singleCharMap, "(", LPAREN);
    setPair(l.singleCharMap, ")", RPAREN);

    l.doubleCharMap = newMap();
    setPair(l.doubleCharMap, "==", BOOL_EQ);
    setPair(l.doubleCharMap, "!=", BOOL_NE);

    l.keywordMap = newMap();
    setPair(l.keywordMap, "let", LET);
    setPair(l.keywordMap, "func", FUNC);

    return l;
}

void deleteLexer(struct Lexer* l) {
    deleteMap(l->singleCharMap);
    deleteMap(l->doubleCharMap);
    deleteMap(l->keywordMap);

    fclose(l->fp);
    free(l->filename);
}

/* Tokanization Utilty Functions & Macros */
#define RET_IF_TOK_VALID(TOK) \
    if (TOK.Type != INVALID) { return TOK; }

void singleCharTok(struct Lexer* l, struct Token* t);
void doubleCharTok(struct Lexer* l, struct Token* t);

struct Token newToken(struct Lexer* l) {
    struct Token t;
    t.filename = new_array(char, strlen(l->filename));
    strcpy(t.filename, l->filename);
    t.column = l->curColumn;
    t.line = l->curLine;

    doubleCharTok(l, &t);
    RET_IF_TOK_VALID(t);

    singleCharTok(l, &t);
    RET_IF_TOK_VALID(t);

    printf("Invalid Token\n"); //TODO: Better error message
    exit(1);
    return t;
}

void singleCharTok(struct Lexer* l, struct Token* t) {
    char* curStr = new_array(char, 2);
    curStr[0] = getc(l->fp);
    curStr[1] = '\0';

    int tt = lookup(l->singleCharMap, curStr);
    if (tt == -1) {
        t->Type = INVALID;
        ungetc(curStr[0], l->fp);
        free(curStr);
        return;
    }

    t->Type = (enum TokenType) tt;
    t->Contents = curStr;
}

void doubleCharTok(struct Lexer* l, struct Token* t) {
    char* curStr = new_array(char, 3);
    curStr[0] = getc(l->fp);
    curStr[1] = getc(l->fp);
    curStr[2] = '\0';

    int tt = lookup(l->doubleCharMap, curStr);
    if (tt == -1) {
        t->Type = INVALID;
        ungetc(curStr[1], l->fp);
        ungetc(curStr[0], l->fp);
        free(curStr);
        return;
    }

    t->Type = (enum TokenType) tt;
    t->Contents = curStr;
}