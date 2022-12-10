#include "parser.h"
#include "lexer.h"
#include "mem_mac.h"
#include "hash.h"

typedef int bool;
#define true 1
#define false 0

#include <stdlib.h>
#include <string.h>

/* Helper Functions */
void setCurTok(struct Parser* p);
void skipWhitespaceAndSetTok(struct Parser* p); //Returns first non-whitespace token
void deleteCurNode(struct Parser* p);

void prattParse(struct Parser* p, int precedence);

void parsePrefix(struct Parser* p);
bool parseInfix(struct Parser* p); //returns true on success

void parseInt(struct Parser* p);
void parseFlt(struct Parser* p);
void parseChr(struct Parser* p);

void parseBinOp(struct Parser* p);
/********/

struct Parser newParser(char* filename) {
    struct Parser retVal;
    retVal.lex = new(struct Lexer);

    struct Lexer newLex = newLexer(filename);
    memcpy(retVal.lex, &newLex, sizeof(struct Lexer));

    retVal.precMap = newMap();
    setPair(retVal.precMap, "+", 1);
    setPair(retVal.precMap, "*", 2);

    retVal.curTok = NULL;

    return retVal;
}

void deleteParser(struct Parser p) {
    deleteLexer(p.lex);
    free(p.lex);

    deleteMap(p.precMap);

    if (p.curTok != NULL) {
        deleteToken(p.curTok);
        free(p.curTok);
    }

    if (p.curNode != NULL) 
        deleteNode(p.curNode);
    
}

void ParseStmt(struct Parser* p) {
    prattParse(p, 0);
}

void prattParse(struct Parser* p, int precedence) {
    skipWhitespaceAndSetTok(p);

    parsePrefix(p);
    bool didParse = true;

    while (didParse && 
        precedence < lookup(p->precMap, p->curTok->Contents)) {

        didParse = parseInfix(p);
    }
}

void setCurTok(struct Parser* p) {
    if (p->curTok != NULL) {
        deleteToken(p->curTok);
        free(p->curTok);
    }

    struct Token nextTok = newToken(p->lex);
    p->curTok = new(struct Token);
    copyToken(p->curTok, &nextTok);
    deleteToken(&nextTok);
}

void skipWhitespaceAndSetTok(struct Parser* p) {
    while (p->curTok == NULL || p->curTok->Type == EOS_TT) {
        setCurTok(p);
    }
}

void deleteCurNode(struct Parser* p) {
    if (p->curNode != NULL)
        deleteNode(p->curNode);
}

void parsePrefix(struct Parser* p) {
    switch (p->curTok->Type) {
    case INT_TT:
        parseInt(p);
        break;
    case FLT_TT:
        parseFlt(p);
        break;
    case CHAR_TT:
        parseChr(p);
        break;
    default:
        printf("Invalid Token '%s'\n", p->curTok->Contents);
        exit(1);
    }
}

void parseInt(struct Parser* p) {
    deleteCurNode(p);
    p->curNode = new(struct Node);

    p->curNode->nt = INT_NT;

    p->curNode->tok = new(struct Token);
    copyToken(p->curNode->tok, p->curTok);

    p->curNode->as.Int = new(struct IntNode);
    p->curNode->as.Int->Value = atoi(p->curTok->Contents);
    setCurTok(p);
}

void parseFlt(struct Parser* p) {
    deleteCurNode(p);
    p->curNode = new(struct Node);

    p->curNode->nt = FLT_NT;
    p->curNode->tok = new(struct Token);
    copyToken(p->curNode->tok, p->curTok);

    p->curNode->as.Flt = new(struct FltNode);
    p->curNode->as.Flt->Value = atof(p->curTok->Contents);
    setCurTok(p);
}

void parseChr(struct Parser* p) {
    deleteCurNode(p);
    p->curNode = new(struct Node);

    p->curNode->nt = CHR_NT;
    p->curNode->tok = new(struct Token);
    copyToken(p->curNode->tok, p->curTok);

    p->curNode->as.Chr = new(struct ChrNode);
    p->curNode->as.Chr->Value = p->curTok->Contents[0];
    setCurTok(p);
}

bool parseInfix(struct Parser* p) {
    switch (p->curTok->Type) {
    case PLUS_TT:
        parseBinOp(p);
        return true;
    case MUL_TT:
        parseBinOp(p);
        return true;
    default:
        return false;
    }

    return false;
}

void parseBinOp(struct Parser* p) {
    struct Node* LHS = p->curNode;
    p->curNode = NULL; //Keeps the parser from deleting the LHS

    struct Node* result = new(struct Node);

    result->as.BinOp = new(struct BinOp);
    result->as.BinOp->LHS = LHS;

    result->nt = BINOP_NT;
    result->tok = new(struct Token);
    copyToken(result->tok, p->curTok);

    result->as.BinOp->Op = new_array(char, strlen(p->curTok->Contents) + 1);
    strcpy(result->as.BinOp->Op, p->curTok->Contents);

    int prec = lookup(p->precMap, p->curTok->Contents);
    setCurTok(p);
    prattParse(p, prec);

    result->as.BinOp->RHS = p->curNode;
    p->curNode = result;
}