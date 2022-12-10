#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "hash.h"

struct Parser {
    struct Lexer* lex;
    struct Map* precMap;
    struct Token* curTok;
    struct Node* curNode;
};

struct Parser newParser(char* filename);
void deleteParser(struct Parser p);
void ParseStmt(struct Parser* p);

#endif