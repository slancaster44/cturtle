#include "vm.h"
#include "instructions.h"

#include <stdio.h>

#define runTest(fn) \
    if (fn() != 0) \
        printf("Test " #fn " failed: %d", fn());


int testSub() {
    return 0;
}

int main() {
    runTest(testSub);
    

    return 0;
}