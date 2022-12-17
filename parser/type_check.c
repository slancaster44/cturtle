
#include "ast.h"

#include <string.h>
#include <stdlib.h>

typedef int bool;

#define true 1
#define false 0
#define panic(MSG) { \
    printf(MSG); \
    exit(1); \
}

bool in(char* val, char** values, int num_values) {
    for (int i = 0; i < num_values; i++) {
        if (!strcmp(val, values[i])) {
            return true;
        }
    }

    return false;
}

enum ReturnType CheckTypeBinOp(char* op, enum ReturnType lhs_type, enum ReturnType rhs_type) {
    char* mathOps[4] = {"+", "-", "*", "/"};
    
    if (!strcmp(op, "==") || !strcmp(op, "!=")) {
        if (lhs_type != rhs_type) {
            panic("Cannot compair dissimilar types\n");
        }

        return BOOL_RT;
    } else if (!strcmp(op, "||") || !strcmp(op, "&&")) {
        if (lhs_type != BOOL_RT && rhs_type != BOOL_RT) {
            panic("Cannot use '||' or '&&' on non-boolean types\n");
        }

        return BOOL_RT;
    } else if (in(op, mathOps, 4)) {
        if (lhs_type != rhs_type) {
            panic("Cannot apply arithmetic operator to dissimilar types\n");
        } else if (lhs_type != INT_RT && lhs_type != FLT_RT && lhs_type != CHR_RT) {
            panic("Invalid type on arithmetic operator\n");
        }

        return lhs_type;
    }

    panic("Could not determine type\n");
}