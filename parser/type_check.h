#ifndef TYPE_CHECK_H
#define TYPE_CHECK_H

#include "ast.h"

enum ReturnType CheckTypeBinOp(char* op, enum ReturnType lhs_type, enum ReturnType rhs_type);

#endif