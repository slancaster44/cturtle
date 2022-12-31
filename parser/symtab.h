#ifndef SYMTAB_H
#define SYMTAB_H

#include "hash.h"
#include "ast.h"

struct Symtab {
    int numFrames;
    struct Map** StackFrames;
};


struct SymbolInfo {
    struct ValueType* Type; /*TODO: Cleanup */
    long long StackLocation;
};

struct Symtab* newSymtab();
void pushNewStackFrame(struct Symtab* st);
void popStackFrame(struct Symtab* st);
void addVariable(struct Symtab* st, char* name, struct SymbolInfo* si);
int numVarsInCurFrame(struct Symtab* st);
long long getStackOffSet(struct Symtab* st);
struct SymbolInfo* getVariable(struct Symtab* st, char* name);
void deleteSymtab(struct Symtab* st);

#endif