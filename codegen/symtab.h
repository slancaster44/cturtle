#ifndef SYMTAB_H
#define SYMTAB_H

/* My advice, read this file from the bottom up,
 * the data types go from least-important to most
 * important as you work your way down.
 */

#include "hash.h"

enum SymbolType {
    FUNC_ST,
    INT_ST,
    FLT_ST,
    CHR_ST,
};

struct SymTabEntry {
    union {} metadata; /* Contains more granular metadata specific to individual types */
    enum SymbolType Type;
};

struct SymTabEntry* newEntry(enum SymbolType Type);
void deleteSymTabEntry(struct SymTabEntry* ste);

struct SymTab {
    struct Map* entryLookupMap; //Maps symbols to indexes in 'entries' array
    struct SymTabEntry** entries;
};

struct SymTab* newSymTab();
void deleteSymTab(struct SymTab* st);
void setEntry(struct SymTab* st, char* symbol, struct SymTabEntry* ste);

#endif