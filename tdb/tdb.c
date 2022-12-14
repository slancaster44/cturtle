#include "tdb.h"
#include "vm.h"
#include "instructions.h"

#include <stdio.h>
#include <stdlib.h>

void debugPrint(byte* code, int codelen);

void DebugRawByteCode(byte* code, int codelen) {
    initVM(code, codelen);

    char command;
    while (!getExitFlag()) {
        printf(">> ");
        command = getchar();

        switch (command) {
        case 's':
            executeInstruction();
            break;
        case 'p':
            debugPrint(code, codelen);
            break;
        case 'q':
            return;
            break;
        default:
            printf("Unknown command\n");
            break;
        }

        getchar(); //Skip over '\n'
    }

    printf("Exit Reached\n");
}

void debugPrint(byte* code, int codelen) {

    char command;
    scanf("%c", &command);

    switch (command) {
    case 'r':
        printRegs();
        break;
    case 's':
        printStack();
        break;
    case 'l':
        Decompile(code, codelen);
        break;
    default:
        printf("Unknown command\n");
    }
}

void Decompile(byte* code, int codelen) {
    for (int i = 0; i < codelen; i ++) {
        if (getPC() - getCB() == i)
            printf("> ");
        else
            printf("  ");


        switch (code[i]) {
        case LDA_IMM:
            printf("lda 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case PUSH_A:
            printf("push a\n");
            break;
        case LDB_A:
            printf("ldb a\n");
            break;
        case POP_A:
            printf("pop a\n");
            break;
        case MUL_A_B:
            printf("mul a, b\n");
            break;
        case ADD_A_B:
            printf("add a, b\n");
            break;
        case ADD_A_IMM:
            printf("add a, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case MUL_A_IMM:
            printf("mul a, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        default:
            printf("Cannot decompile opcode '%d'\n", code[i]);
            exit(1);
        }
    }
}