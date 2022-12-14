#include "hash.h"
#include "symtab.h"
#include "mem_mac.h"

struct Symtab* newSymtab() {
    struct Symtab* retVal = new(struct Symtab);
    retVal->numFrames = 1;
    retVal->StackFrames = new_array(struct Map*, 1);
    retVal->StackFrames[0] = newMap();

    return retVal;
}

void pushNewStackFrame(struct Symtab* st) {
    expand_array(struct Map*, st->StackFrames, st->numFrames, (st->numFrames+1));
    st->StackFrames[st->numFrames] = newMap();
    st->numFrames++;
}

void popStackFrame(struct Symtab* st) {
    struct Map* thisFrame = st->StackFrames[st->numFrames-1];

    for (int i = 0; i < thisFrame->capacity; i++) {
        if (thisFrame->pairs[i] != NULL) {
            free((struct SymtbolInfo*) thisFrame->pairs[i]->Value);
        }
    }

    deleteMap(thisFrame);

    if (st->numFrames == 1) {
        free(st->StackFrames);
        st->StackFrames = NULL;
        st->numFrames--;
    } else {
        shrink_array(struct Map*, st->StackFrames, st->numFrames, (st->numFrames-1));
        st->numFrames--;
    }
}

long long getStackOffSet(struct Symtab* st) {
    long long result = 0;

    for (int i = 0; i < st->numFrames; i ++) {
        result += st->StackFrames[i]->pairsUsed;
    }

    return result;
}

void addVariable(struct Symtab* st, char* name, struct SymbolInfo* si) {
    setPair(st->StackFrames[st->numFrames-1], name, (long long) si);
}

void deleteSymtab(struct Symtab* st) {
    while (st->numFrames > 0) {
        popStackFrame(st);
    }

    free(st->StackFrames);
    free(st);
}

struct SymbolInfo* getVariable(struct Symtab* st, char* name) {
    for (int i = st->numFrames-1; i > -1; i--) {
        long long val = lookup(st->StackFrames[i], name);
        
        if (val != -1) {
            return (struct SymbolInfo*) val;
        }
    }

    return NULL;
}

int numVarsInCurFrame(struct Symtab* st) {
    struct Map* thisFrame = st->StackFrames[st->numFrames-1];
    return thisFrame->pairsUsed;
}