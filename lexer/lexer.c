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
    setPair(l.singleCharMap, "(", LPAREN_TT);
    setPair(l.singleCharMap, ")", RPAREN_TT);
    setPair(l.singleCharMap, "=", EQ_TT);
    setPair(l.singleCharMap, "+", PLUS_TT);
    setPair(l.singleCharMap, "-", MINUS_TT);
    setPair(l.singleCharMap, "/", DIV_TT);
    setPair(l.singleCharMap, "*", MUL_TT);
    setPair(l.singleCharMap, ":", COLON_TT);
    setPair(l.singleCharMap, ",", COMMA_TT);
    setPair(l.singleCharMap, "[", LBRACK_TT);
    setPair(l.singleCharMap, "]", RBRACK_TT);
    setPair(l.singleCharMap, "\n", EOS_TT);

    l.doubleCharMap = newMap();
    setPair(l.doubleCharMap, "==", BOOL_EQ_TT);
    setPair(l.doubleCharMap, "!=", BOOL_NE_TT);
    setPair(l.doubleCharMap, "||", BOOL_OR_TT);
    setPair(l.doubleCharMap, "&&", BOOL_AND_TT);

    l.keywordMap = newMap();
    setPair(l.keywordMap, "let", LET_TT);
    setPair(l.keywordMap, "true", TRUE_TT);
    setPair(l.keywordMap, "false", FALSE_TT);
    setPair(l.keywordMap, "if", IF_TT);
    setPair(l.keywordMap, "else", ELSE_TT);
    setPair(l.keywordMap, "elif", ELIF_TT);
    setPair(l.keywordMap, "end", END_TT);
    setPair(l.keywordMap, "while", WHILE_TT);
    setPair(l.keywordMap, "do", DO_TT);
    setPair(l.keywordMap, "print", BUILTIN_TT);

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
    if (TOK.Type != INVALID_TT) { return TOK; }

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

    if (feof(l->fp)) {
        t.Contents = new_array(char, 4);
        strcpy(t.Contents, "EOF\0");
        t.Type = EOF_TT;
        return t;
    }

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

    lexer_panic(l, "Invalid character '%c'\n", curChar);
    return t;
}

void singleCharTok(struct Lexer* l, struct Token* t) {
    char* curStr = new_array(char, 2);
    curStr[0] = getc(l->fp);
    curStr[1] = '\0';

    int tt = lookup(l->singleCharMap, curStr);
    if (tt == -1) {
        t->Type = INVALID_TT;
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
        t->Type = INVALID_TT;
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
        t->Type = IDENT_TT;
    }

    t->Contents = curStr;
}

void numberTok(struct Lexer* l, struct Token* t) {
    char* curStr = new_array(char, 1);
    t->Type = INT_TT;

    int location = 0;
    int curStrSize = 1;
    char curChar = getc(l->fp);
    while (isNumber(curChar) || curChar == '.') {
        if (curChar == '.') {
            t->Type = FLT_TT;
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

char getCharLiteral(struct Lexer* l, char terminator) {
    char retVal = getc(l->fp);

    if (retVal == terminator) {
        ungetc(retVal, l->fp);
        retVal = '\0';
    } else if (retVal == '\\') {
        char val = getc(l->fp);
        if (val == terminator) {
            retVal = terminator;
            return retVal;
        }

        switch (val) {
        case 'n':
            retVal = '\n';
            break;
        case 't':
            retVal = '\t';
            break;
        case '\\':
            retVal = '\\';
            break;
        case 'r':
            retVal = '\r';
            break;
        default:
            lexer_panic(l, "Expected valid escape character");
        }
    }

    return retVal;
}

void charTok(struct Lexer* l, struct Token* t) {
    char* value = new_array(char, 2)
    value[0] = getCharLiteral(l, '\'');
    value[1] = '\0';
    t->Type = CHAR_TT;

    if (getc(l->fp) != '\'') {
        lexer_panic(l, "Expected closing quote\n");
    }

    t->Contents = value;
}

void strTok(struct Lexer* l, struct Token* t) {
    char* curStr = new_array(char, 1);
    t->Type = STR_TT;

    int location = 0;
    int curStrSize = 1;
    char curChar = getCharLiteral(l, '"');
    char boundsCheck = getc(l->fp);
    while (boundsCheck != '"' && !feof(l->fp)) {
        ungetc(boundsCheck, l->fp);

        curStr[location] = curChar;

        int oldSize = curStrSize ++;
        expand_array(char, curStr, oldSize, curStrSize);

        location ++;
        curChar = getCharLiteral(l, '"');
        boundsCheck = getc(l->fp);
    }

    if (feof(l->fp)) {
        lexer_panic(l, "Expected closing quote\n");
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

void deleteToken(struct Token* t) {
    free(t->filename);
    free(t->Contents);
}

void copyToken(struct Token* dest, struct Token* src) {
    dest->column = src->column;
    dest->line = src->line;
    dest->Type = src->Type;

    dest->Contents = new_array(char, strlen(src->Contents) + 1);
    strcpy(dest->Contents, src->Contents);

    dest->filename = new_array(char, strlen(src->filename) + 1);
    strcpy(dest->filename, src->filename);
}
