#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "hash.h"
#include "panic.h"
#include "symtab.h"

struct Parser {
    struct Lexer* lex;
    struct Map* precMap;
    struct Token* curTok;
    struct Node* curNode;
    struct Symtab* PrimativeSymbols; /*Ints, chars, and flts*/
    struct Symtab* CompositeSymbols; /* lists, structs */
};

struct Parser newParser(char* filename);
void deleteParser(struct Parser p);
void ParseStmt(struct Parser* p);

#define parser_panic(PARSER, ...) { \
    panic(PARSER->curTok->line, PARSER->curTok->column,  PARSER->curTok->filename, __VA_ARGS__); \
}

#endif