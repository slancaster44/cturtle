#include "tbin.h"
#include "mem_mac.h"

#include <string.h>

struct TurtleBinary* newBinary() {
    struct TurtleBinary* tb = new(struct TurtleBinary);
    
    tb->MagicNumber = new_array(byte, MAGIC_NUMBER_WIDTH);
    memcpy(tb->MagicNumber, (byte*) MAGIC_NUMBER, MAGIC_NUMBER_WIDTH);

    tb->codelen = 0;
    tb->code = NULL;

    return tb;
}

void deleteTurtleBinary(struct TurtleBinary* tb) {
    free(tb->MagicNumber);
    free(tb->code);
    free(tb);
}

void writeCode(struct TurtleBinary* tb, byte* code, int lengthOfNewCode) {
    int oldCodelen = tb->codelen;
    tb->codelen += lengthOfNewCode;

    if (tb->code == NULL) {
        tb->code = new_array(byte, tb->codelen);
    } else {
        expand_array(byte, tb->code, oldCodelen, tb->codelen);
    }

    memcpy(tb->code + oldCodelen, code, lengthOfNewCode);
}