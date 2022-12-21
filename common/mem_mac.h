#ifndef MEM_MACS
#define MEM_MACS

#include <stdlib.h>
#include <string.h>

#define new(type) (type*) malloc(sizeof(type));

#define new_array(TYPE, SIZE) (TYPE*) malloc(SIZE * sizeof(TYPE));

#define expand_array(TYPE, OLDARR, OLD_SIZE, NEW_SIZE) {    \
    TYPE* newContainer = (TYPE*) malloc(NEW_SIZE * sizeof(TYPE));\
    memcpy(newContainer, OLDARR, OLD_SIZE * sizeof(TYPE));  \
    free(OLDARR);                                           \
    OLDARR = newContainer;                                  \
}

#define shrink_array(TYPE, OLDARR, OLD_SIZE, NEW_SIZE) {    \
    TYPE* newContainer = new_array(TYPE, NEW_SIZE);         \
    memcpy(newContainer, OLDARR, NEW_SIZE * sizeof(TYPE));  \
    free(OLDARR);                                           \
    OLDARR = newContainer;                                  \
}


#endif