#ifndef MEM_MACS
#define MEM_MACS

#include <stdlib.h>

#define new(type) (type*) malloc(sizeof(type));

#define new_array(type, size) (type*) malloc(sizeof(type[size]));

#define resize_array(TYPE, OLDARR, OLD_SIZE, NEW_SIZE) {    \
    TYPE* newContainer= new_array(TYPE, NEW_SIZE);          \
    memcpy(newContainer, OLDARR, OLD_SIZE * sizeof(TYPE));  \
    free(OLDARR);                                           \
    OLDARR = newContainer;                                  \
}

#endif