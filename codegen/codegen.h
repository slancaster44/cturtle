
#include "common_types.h"
#include "parser.h"
#include "hash.h"

struct CodeGenerator {
    byte* OutputCode;
    int OutputCodeLen;

    struct Parser parser;

    struct Map* OpAImm;
    struct Map* OpAB;
};

struct CodeObj {
    byte* code;
    int codelen;
};

struct CodeObj Compile(char* filename);