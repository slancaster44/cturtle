#include <stdio.h>
int main() {
    int a = 15;

    while (a != 251658225) {
        a += 15;
    }
    printf("%d", a);
    return 0;
}