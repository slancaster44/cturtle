
#include "ast.h"

#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "panic.h"
#include "parser.h"
#include "mem_mac.h"

bool in(char* val, char** values, int num_values) {
    for (int i = 0; i < num_values; i++) {
        if (!strcmp(val, values[i])) {
            return true;
        }
    }
    return false;
}

struct ValueType* CheckTypeBinOp(struct Parser* p, char* op, struct ValueType* lhs_type, struct ValueType* rhs_type) {
    char* mathOps[4] = {"+", "-", "*", "/"};
    
    if (!strcmp(op, "==") || !strcmp(op, "!=")) {
        if (!typesMatch(lhs_type, rhs_type)) {
            parser_panic(p, "Cannot compair dissimilar types\n");
        }

        return newType(BOOL_BT);
    } else if (!strcmp(op, "||") || !strcmp(op, "&&")) {
        if (lhs_type->base_type != BOOL_BT && rhs_type->base_type != BOOL_BT) {
            parser_panic(p, "Cannot use '||' or '&&' on non-boolean types\n");
        }

        return newType(BOOL_BT);
    } else if (in(op, mathOps, 4)) {
        if (!typesMatch(lhs_type, rhs_type)) {
            parser_panic(p, "Cannot apply arithmetic operator to dissimilar types\n");
        } else if (lhs_type->base_type != INT_BT && lhs_type->base_type != FLT_BT && lhs_type->base_type != CHR_BT) {
            parser_panic(p, "Invalid type on arithmetic operator\n");
        }

        return lhs_type; /*TODO: Copy? */
    }

    parser_panic(p, "Could not determine type\n");
}

struct ValueType* newType(enum BaseType bt) {
    struct ValueType* retVal = new(struct ValueType);

    retVal->base_type = bt;
    return retVal;
}

void deleteType(struct ValueType* vt) {
    switch (vt->base_type) {
    case LIST_BT:
        deleteType(vt->subtype_info.ListEntryType);
        /*Break left out intentionally*/
    default:
        free(vt);
    }
}

bool typesMatch(struct ValueType* v1, struct ValueType* v2) {
    if (v1->base_type == LIST_BT && v2->base_type == LIST_BT) {
        return typesMatch(v1->subtype_info.ListEntryType, v2->subtype_info.ListEntryType);
    } else {
        return v1->base_type == v2->base_type;
    }
}

bool isTypeComplex(struct ValueType* v1) {
    switch (v1->base_type) {
    case LIST_BT:
        return true;
    default:
        return false;
    }
}