#include "lexer.h"
#include "mem_mac.h"
#include "hash.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void printToken(struct Token* t) {
    printf("%s: %d\n", t->Contents, t->Type);
}

struct Lexer newLexer(char* filename) {
    struct Lexer l;
    
    l.filename = new_array(char, strlen(filename) + 1);
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
    setPair(l.singleCharMap, "\n", EOS);

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
void keywordTok(struct Lexer* l, struct Token* t);
void numberTok(struct Lexer* l, struct Token* t);
void charTok(struct Lexer* l, struct Token* t);
void strTok(struct Lexer* l, struct Token* t);

void skipWhitespace(struct Lexer* l);

int isAlpha(char c);
int isNumber(char c);
int isAlphaOrNumber(char c);

struct Token newToken(struct Lexer* l) {
    struct Token t;
    t.filename = new_array(char, strlen(l->filename) + 1);
    strcpy(t.filename, l->filename);
    t.column = l->curColumn;
    t.line = l->curLine;

    skipWhitespace(l);

    doubleCharTok(l, &t);
    RET_IF_TOK_VALID(t);
    
    singleCharTok(l, &t);
    RET_IF_TOK_VALID(t);

    char curChar = getc(l->fp);
    if (isAlpha(curChar)) {
        ungetc(curChar, l->fp);
        keywordTok(l, &t);
        return t;
    } else if (isNumber(curChar)) {
        ungetc(curChar, l->fp);
        numberTok(l, &t);
        return t;
    } else if (curChar == '\'') {
        charTok(l, &t);
        return t;
    } else if (curChar == '"') {
        strTok(l, &t);
        return t;
    }

    printf("Invalid Token '%c'\n", curChar); //TODO: Better error message
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

void keywordTok(struct Lexer* l, struct Token* t) {
    char* curStr = new_array(char, 1);

    int location = 0;
    int curStrSize = 1;
    char curChar = getc(l->fp);
    while (isAlphaOrNumber(curChar)) {
        curStr[location] = curChar;

        int oldSize = curStrSize++;
        expand_array(char, curStr, oldSize, curStrSize);

        location ++;
        curChar = getc(l->fp);
    }
    ungetc(curChar, l->fp);
    curStr[location] = '\0';

    t->Type = lookup(l->keywordMap, curStr);
    if (t->Type == -1) {
        t->Type = IDENT;
    }

    t->Contents = curStr;
}

void numberTok(struct Lexer* l, struct Token* t) {
    char* curStr = new_array(char, 1);
    t->Type = INT;

    int location = 0;
    int curStrSize = 1;
    char curChar = getc(l->fp);
    while (isNumber(curChar) || curChar == '.') {
        if (curChar == '.') {
            t->Type = FLT;
        }

        curStr[location] = curChar;

        int oldSize = curStrSize ++;
        expand_array(char, curStr, oldSize, curStrSize);

        location ++;
        curChar = getc(l->fp);
    }
    ungetc(curChar, l->fp);
    curStr[location] = '\0';

    t->Contents = curStr;
}

void charTok(struct Lexer* l, struct Token* t) {
    char* value = new_array(char, 2);
    value[0] = getc(l->fp);
    value[1] = '\0';
    t->Type = CHAR;

    if (getc(l->fp) != '\'') {
        printf("Expected closing quote on character");
        exit(1);
    }

    t->Contents = value;
}

void strTok(struct Lexer* l, struct Token* t) {
    char* curStr = new_array(char, 1);
    t->Type = STR;

    int location = 0;
    int curStrSize = 1;
    char curChar = getc(l->fp);
    while (curChar != '"' && !feof(l->fp)) {
        curStr[location] = curChar;

        int oldSize = curStrSize ++;
        expand_array(char, curStr, oldSize, curStrSize);

        location ++;
        curChar = getc(l->fp);
    }

    if (feof(l->fp)) {
        printf("Expected closing quote on string\n");
        exit(1);
    }

    curStr[location] = '\0';
    t->Contents = curStr;
}

void skipWhitespace(struct Lexer* l) {
    char curChar = getc(l->fp);
    while (curChar == ' ' || curChar == '\t' || curChar == '\r') {
        curChar = getc(l->fp);
    }

    ungetc(curChar, l->fp);
}

int isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

int isNumber(char c) {
    return (c >= '0' && c <= '9');
}

int isAlphaOrNumber(char c) {
    return isNumber(c) || isAlpha(c);
}