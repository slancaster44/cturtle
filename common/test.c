#include "test.h"

#include <stdio.h>

void assert(int expr, char* name) {
    if (expr)
        printf("%s Passed\n", name);
    else {
        printf("%s Failed\n", name);
    }
}