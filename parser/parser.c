#include "parser.h"
#include "lexer.h"
#include "mem_mac.h"
#include "hash.h"
#include "panic.h"
#include "common_types.h"

#include <stdlib.h>
#include <string.h>

/* Helper Functions */
void setCurTok(struct Parser* p);
void skipWhitespaceAndSetTok(struct Parser* p); //Returns first non-whitespace token
void deleteCurNode(struct Parser* p);

void prattParse(struct Parser* p, int precedence);

void parsePrefix(struct Parser* p);
bool parseInfix(struct Parser* p); //returns true on success

void parseIfElse(struct Parser* p);

void parseInt(struct Parser* p);
void parseFlt(struct Parser* p);
void parseChr(struct Parser* p);
void parseBool(struct Parser* p);

void parseBinOp(struct Parser* p);
enum ReturnType CheckTypeBinOp(struct Parser* p, char* op, enum ReturnType lhs_type, enum ReturnType rhs_type);

/********/

struct Parser newParser(char* filename) {
    struct Parser retVal;
    retVal.lex = new(struct Lexer);

    struct Lexer newLex = newLexer(filename);
    memcpy(retVal.lex, &newLex, sizeof(struct Lexer));

    retVal.precMap = newMap();
    setPair(retVal.precMap, "==", 2);
    setPair(retVal.precMap, "!=", 2);
    setPair(retVal.precMap, "||", 1);
    setPair(retVal.precMap, "&&", 1);
    setPair(retVal.precMap, "+", 3);
    setPair(retVal.precMap, "-", 3);
    setPair(retVal.precMap, "*", 4);
    setPair(retVal.precMap, "/", 4);

    retVal.curTok = NULL;
    retVal.curNode = NULL;

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
    deleteCurNode(p);
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
    case TRUE_TT:
        parseBool(p);
        break;
    case FALSE_TT:
        parseBool(p);
        break;
    case IF_TT:
        parseIfElse(p);
        break;
    default:
        panic(p->curTok->line, 
            p->curTok->column, 
            p->curTok->filename, 
            "Unexpected token '%s'", p->curTok->Contents); 
    }
}

void parseInt(struct Parser* p) {
    p->curNode = new(struct Node);

    p->curNode->nt = INT_NT;

    p->curNode->tok = new(struct Token);
    copyToken(p->curNode->tok, p->curTok);

    p->curNode->as.Int = new(struct IntNode);
    p->curNode->as.Int->Value = atoi(p->curTok->Contents);

    p->curNode->rt = INT_RT;
    setCurTok(p);
}

void parseBool(struct Parser* p) {
    p->curNode = new(struct Node);

    p->curNode->nt = BOOL_NT;

    p->curNode->tok = new(struct Token);
    copyToken(p->curNode->tok, p->curTok);

    p->curNode->as.Bool = new(struct BoolNode);
    p->curNode->as.Bool->Value = p->curTok->Type == TRUE_TT;

    p->curNode->rt = BOOL_RT;
    setCurTok(p);
}

void parseFlt(struct Parser* p) {
    p->curNode = new(struct Node);

    p->curNode->nt = FLT_NT;
    p->curNode->tok = new(struct Token);
    copyToken(p->curNode->tok, p->curTok);

    p->curNode->as.Flt = new(struct FltNode);
    p->curNode->as.Flt->Value = atof(p->curTok->Contents);

    p->curNode->rt = FLT_RT;
    setCurTok(p);
}

void parseChr(struct Parser* p) {
    p->curNode = new(struct Node);

    p->curNode->nt = CHR_NT;
    p->curNode->tok = new(struct Token);
    copyToken(p->curNode->tok, p->curTok);

    p->curNode->as.Chr = new(struct ChrNode);
    p->curNode->as.Chr->Value = p->curTok->Contents[0];

    p->curNode->rt = CHR_RT;
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
    case DIV_TT:
        parseBinOp(p);
        return true;
    case MINUS_TT:
        parseBinOp(p);
        return true;
    case BOOL_EQ_TT:
        parseBinOp(p);
        return true;
    case BOOL_NE_TT:
        parseBinOp(p);
        return true;
    case BOOL_OR_TT:
        parseBinOp(p);
        return true;
    case BOOL_AND_TT:
        parseBinOp(p);
        return true;
    default:
        return false;
    }

    return false;
}

void parseBinOp(struct Parser* p) {
    struct Node* LHS = p->curNode;

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
    result->rt = CheckTypeBinOp(p, result->as.BinOp->Op, result->as.BinOp->LHS->rt, result->as.BinOp->RHS->rt);

    p->curNode = result;
}

/* Utility function
 * determines if a given token exists within a set of tokens
 */
bool in_tokenType(enum TokenType curTokType, enum TokenType* terminators, int numTerminators) {
    for (int i = 0; i < numTerminators; i++) {
        if (curTokType == terminators[i]) {
            return true;
        }
    }
    return false;
}

/* Parses statements until a token is reached that matches
 * one of the given terminating tokens
 */
struct Block* parseBlock(struct Parser* p, enum TokenType* terminators, int numTerminators) {
    struct Block* retVal = new(struct Block);
    retVal->Statements = NULL;
    retVal->numStatements = 0;

    while (!in_tokenType(p->curTok->Type, terminators, numTerminators)) {
        retVal->numStatements++;
        
        if (retVal->Statements == NULL) {
            retVal->Statements = new_array(struct Node*, retVal->numStatements);
        } else {
            expand_array(struct Node*, retVal->Statements, retVal->numStatements, retVal->numStatements+1);
        }

        prattParse(p, 0);
        retVal->Statements[retVal->numStatements-1] = p->curNode;
        skipWhitespaceAndSetTok(p);
    }

    return retVal;
}

void parseIfElse(struct Parser* p) {
    struct Node* result = new(struct Node);
    result->tok = new(struct Token)
    copyToken(result->tok, p->curTok);
    result->nt = IFEL_NT;
    result->rt = NULL_RT;

    result->as.IfEl = new(struct IfElseNode);

    result->as.IfEl->numBlocks = 1;
    result->as.IfEl->Conditions = new_array(struct Node*, result->as.IfEl->numBlocks);
    result->as.IfEl->Blocks = new_array(struct Block*, result->as.IfEl->numBlocks);

    enum TokenType terminators[4] = {ELIF_TT, ELSE_TT, END_TT, EOF_TT};
    
    do {
        setCurTok(p); /* Move over if/elif token */

        prattParse(p, 0); /* Parse condition for this branch */
        int index = result->as.IfEl->numBlocks-1;
        result->as.IfEl->Conditions[index] = p->curNode;

        if (p->curTok->Type != COLON_TT) {
            parser_panic(p, "Expected ':' got '%s'\n", p->curTok->Contents);
        }
        setCurTok(p);

        result->as.IfEl->Blocks[index] = parseBlock(p, terminators, 4);
        expand_array(struct Node*, result->as.IfEl->Conditions, result->as.IfEl->numBlocks, result->as.IfEl->numBlocks+1); /*But why?*/
        expand_array(struct Block*, result->as.IfEl->Blocks, result->as.IfEl->numBlocks, result->as.IfEl->numBlocks+1);
        result->as.IfEl->numBlocks++;
        skipWhitespaceAndSetTok(p);

    } while (p->curTok->Type == ELIF_TT);
    result->as.IfEl->numBlocks--;

    if (p->curTok->Type == END_TT) {
        result->as.IfEl->ElseBlock = NULL;
        setCurTok(p); /* Skip over 'end' */

    } else if (p->curTok->Type == ELSE_TT) {
        setCurTok(p); /* Skip over 'else' */
        result->as.IfEl->ElseBlock = parseBlock(p, terminators, 4);

        if (p->curTok->Type != END_TT) {
            parser_panic(p, "Expected 'end,' got '%s'\n", p->curTok->Contents);
        }
    } else {
        parser_panic(p, "Expected 'else' or 'end' token, got '%s'\n", p->curTok->Contents);
    }

    if (p->curTok->Type != END_TT) {
        parser_panic(p, "Expected 'end' token got '%s'\n", p->curTok->Contents);
    }
    setCurTok(p);

    p->curNode = result;
}