#include <stdio.h>

#include "hash.h"

void assert(int expr, char* name) {
    if (expr)
        printf("%s Passed\n", name);
    else {
        printf("%s Failed\n", name);
    }
}

void test() {
    struct Map* m = newMap();
    setPair(m, "Hello", 10);
    setPair(m, "Banana", 11);
    setPair(m, "Onion", 12);
    setPair(m, "Monkey", 13);

    assert(lookup(m, "Hello") == 10, "Test 1");
    assert(lookup(m, "Banana") == 11, "Test 2");
    assert(lookup(m, "Onion") == 12, "Test 3");
    assert(lookup(m, "Monkey") == 13, "Test 4");

    deleteMap(m);

    m = newMap();
    setPairInt(m, 10, 11);
    setPairInt(m, 11, 12);
    setPairInt(m, 13, 14);
    setPairInt(m, 14, 15);

    assert(lookupInt(m, 10) == 11, "Test 5");
    assert(lookupInt(m, 11) == 12, "Test 6");
    assert(lookupInt(m, 14) == 15, "Test 7");
    assert(lookupInt(m, 13) == 14, "Test 8");
    deleteMap(m);
}



int main() {
    test();
    return 0;
}