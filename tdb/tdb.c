#include "tdb.h"
#include "vm.h"
#include "instructions.h"

#include <stdio.h>
#include <stdlib.h>

void debugPrint(byte* code, int codelen);

void DebugRawByteCode(byte* code, int codelen) {
    if (codelen == 0) {
        return;
    }
    
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
        printf("0x%04X ", i);
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
        case SUB_A_IMM:
            printf("sub a, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case EQ_A_B:
            printf("eq a, b\n");
            break;
        case NE_A_B:
            printf("ne a, b\n");
            break;
        case OR_A_B:
            printf("or a, b\n");
            break;
        case AND_A_B:
            printf("and a, b\n");
            break;
        case EQ_A_IMM:
            printf("eq a, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case NE_A_IMM:
            printf("ne a, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case AND_A_IMM:
            printf("and a, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case OR_A_IMM:
            printf("or a, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case JPA_Z_OFF:
            printf("jpa z, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case JP_OFF:
            printf("jp 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case JPA_OFF:
            printf("jpa nz, 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case EXIT:
            printf("exit\n");
            break;
        case ENSURE_STACK_SIZE:
            printf("ess 0x%hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        case INSERT_STACK_IMM_A:
            printf("ldm stack[0x%hhX], a\n", code[i+1]);
            i += sizeof(qword);
            break;
        case LDA_STACK_IMM:
            printf("lda stack[0x%hhX]\n", code[i+1]);
            i += sizeof(qword);
            break;
        case SHRINK_STACK_SIZE:
            printf("sss %hhX\n", code[i+1]);
            i += sizeof(qword);
            break;
        default:
            printf("Cannot decompile opcode '%d'\n", code[i]);
            exit(1);
        }
    }
}