
#include "codegen.h"
#include "mem_mac.h"
#include "instructions.h"
#include "tdb.h"
#include "tbin.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define panic(MSG) \
    printf("%s", MSG); \
    exit(1);

#define checkFilename \
    if (argc < 3) { panic("No filename provided\n"); } 


void printHelp();
void executeBin(char* filename);
void debug(char* filename);
void compile(char* filename);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("No Arguments\n");
        printHelp();
        return 1;
    }

    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
        printHelp();
        return 0;
    } else if (!strcmp(argv[1], "--compile") || !strcmp(argv[1], "-c")) {
        checkFilename;
        compile(argv[2]);
        return 0;
    } else if (!strcmp(argv[1], "--debug") || !strcmp(argv[1], "-d")) {
        checkFilename;
        debug(argv[2]);
        return 0;
    } else {
        printf("Invalid Command Line Argument");
        printHelp();
        return 1;
    }

    return 0;
}

void printHelp() {
    printf("--help    | -h :: Print help to screen\n");
    printf("--compile | -c :: Compile file and generate bytecode binary\n");
    printf("--run     | -r :: Run compiled bytecode binary\n");
    printf("--debug   | -d :: Run bytecode binary in debugger\n");
}

void compile(char* filename) {
    struct CodeGenerator* cg = newCodeGenerator(filename);

    /*Compile program statement by statement*/
    int outputCodeSize = 0;
    uint8_t* outputCode = new_array(uint8_t, 1);
    
    while (cg->currentStatementCode == NULL || cg->parser->curTok->Type != EOF_TT) {
        compileCurrentStatement(cg);

        expand_array(uint8_t, outputCode, outputCodeSize, outputCodeSize + cg->codeSize);
        memcpy(outputCode + outputCodeSize, cg->currentStatementCode, cg->codeSize);
        outputCodeSize += cg->codeSize;
    }
    deleteCodeGenerator(cg);
    cg = NULL;

    /* Append Exit Instruction */
    expand_array(uint8_t, outputCode, outputCodeSize, outputCodeSize + 1);
    outputCode[outputCodeSize++] = EXIT;

    /* Write out program */
    struct TurtleBinary* outputBin = newBinary();
    writeCode(outputBin, outputCode, outputCodeSize);
    writeTurtleFile(outputBin, "output.tbin");
    deleteTurtleBinary(outputBin);
}

void debug(char* filename) {
    struct TurtleBinary* tb = readTurtleFile(filename);
    DebugRawByteCode(tb->code, tb->codelen);
    deleteTurtleBinary(tb);
}

void executeBin(char* filename) {
    struct TurtleBinary* tb = readTurtleFile(filename);
    Execute(tb->code, tb->codelen);
    deleteTurtleBinary(tb);
}