#include "tbin.h"
#include "mem_mac.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

struct TurtleBinary* newBinary() {
    struct TurtleBinary* tb = new(struct TurtleBinary);
    
    tb->MagicNumber = new_array(byte, MAGIC_NUMBER_WIDTH);
    memcpy(tb->MagicNumber, MAGIC_NUMBER, MAGIC_NUMBER_WIDTH);

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

void writeTurtleFile(struct TurtleBinary* tb, char* filename) {
    FILE* fp = fopen(filename, "w");

    for (int i = 0; i < MAGIC_NUMBER_WIDTH; i ++) {
        fputc(tb->MagicNumber[i], fp);
    }
    
    byte* value = (byte*) &tb->codelen;
    for (int i = 0; i < sizeof(qword); i ++) {
        fputc(value[i], fp);
    }

    for (int i = 0; i < tb->codelen; i++) {
        fputc(tb->code[i], fp);
    }

    fclose(fp);
}

struct TurtleBinary* readTurtleFile(char* filename) {
    struct TurtleBinary* retVal = newBinary();

    FILE* fp = fopen(filename, "r");
    char magic[MAGIC_NUMBER_WIDTH+1];

    for (int i = 0; i < MAGIC_NUMBER_WIDTH; i ++) {
        magic[i] = getc(fp);
    }
    magic[MAGIC_NUMBER_WIDTH] = '\0';

    if (strcmp(magic, MAGIC_NUMBER) != 0) {
        printf("Invalid Magic Number\n");
        exit(1);
    }

    for (int i = 0; i < sizeof(qword); i++) {
        retVal->codelen += getc(fp) * ((qword) powl((double) 256, (double) i));
    }

    retVal->code = new_array(byte, retVal->codelen);
    for (int i = 0; i < retVal->codelen; i++) {
        retVal->code[i] = getc(fp);
    }

    fclose(fp);
    return retVal;
}