
#include "codegen.h"
#include "mem_mac.h"
#include "instructions.h"
#include "tdb.h"
#include "vm.h"
#include "panic.h"
#include "tbin.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define checkFilename \
    if (argc < 3) { panic(0, 0, "unknown.trtl", "Filename not provided"); } 


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
    } else if (!strcmp(argv[1], "--run") || !strcmp(argv[1], "-r")) {
        checkFilename;
        executeBin(argv[2]);
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
    struct CodeObj code = Compile(filename);

    /* Write out program */
    struct TurtleBinary* outputBin = newBinary();
    writeCode(outputBin, code.code, code.codelen);
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