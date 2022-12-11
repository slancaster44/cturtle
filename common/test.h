#ifndef TEST_H
#define TEST_H

void assert(int condition, char* testName);
#define assertArrayEquals(ARR1, ARR2, ARRLEN, TEST_NAME) { \
    for (int i = 0; i < ARRLEN; i++) { \
        if (ARR1[i] != ARR2[i]) { \
            printf("%s Failed\n", TEST_NAME); \
            return; \
        } \
    } \
 \
    printf("%s Passed\n", TEST_NAME); \
}
#endif
