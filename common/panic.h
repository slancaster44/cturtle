#ifndef PANIC_H
#define PANIC_H

#include <stdarg.h>


#define panic(LINE, COLUMN, FILENAME, ...) { \
    printf("%s: (%d:%d) -- ", FILENAME, LINE, COLUMN); \
    printf(__VA_ARGS__); \
    exit(1); \
}

#endif