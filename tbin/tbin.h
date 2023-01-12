#ifndef TBIN_H
#define TBIN_H

/* A library used to read, write and manipulate turtle binaries */

/* File Specification
 * Bytes 0 - 11: THE MAGIC NUMBER!!
 * Bytes 12: Size of the code
 * Bytes 13 - end: The code
 */

#include <stdint.h>

#include "common_types.h"

#define MAGIC_NUMBER_WIDTH 10
#define MAGIC_NUMBER "#!/bin/tvm"

struct TurtleBinary {
    byte* MagicNumber;
    qword codelen;
    byte* code;
};

struct TurtleBinary* newBinary();
void deleteTurtleBinary(struct TurtleBinary* tb);
void writeCode(struct TurtleBinary* tb, byte* code, int lengthOfNewCode);

struct TurtleBinary* readTurtleFile(char* filename);
void writeTurtleFile(struct TurtleBinary* tb, char* filename);

#endif