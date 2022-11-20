#include "vm.h"
#include "mem_mac.h"
#include "instructions.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define decodeFirstReg(VAL) VAL >> 4
#define decodeSecondReg(VAL) VAL & 0b00001111

#define getByteImm() *(PC++)
static inline word getWordImm();
static inline qword getQwordImm();

void Execute(byte* code, int code_length) {
    CB = new_array(byte, code_length);
    memcpy(CB, code, code_length);

    SB = new_array(byte, 0);

    codelen = code_length;

    PC = CB;

    enum Opcode curOpcode;
    while (1) {
        curOpcode = getByteImm();
        switch (curOpcode) {
            case LDA_IMM:
                Lda_Imm(getQwordImm());
                break;
            case LDB_IMM:
                Ldb_Imm(getQwordImm());
                break;
            case LDA_B:
                Lda_B();
                break;
            case LDB_A:
                Ldb_A();
                break;
            case ADD_A_B:
                Add_A_B();
                break;
            case ADD_B_A:
                Add_B_A();
                break;
            case ADD_A_IMM:
                Add_A_Imm(getQwordImm());
                break;
            case ADD_B_IMM:
                Add_B_Imm(getQwordImm());
                break;
            case AND_A_B:
                And_A_B();
                break;
            case AND_B_A:
                And_B_A();
                break;
            case AND_A_IMM:
                And_A_Imm(getQwordImm());
                break;
            case AND_B_IMM:
                And_B_Imm(getQwordImm());
                break;
            case SL_A_IMM:
                Sl_A_Imm(getByteImm());
                break;
            case SL_B_IMM:
                Sl_B_Imm(getByteImm());
                break;
            case SR_A_IMM:
                Sr_A_Imm(getByteImm());
                break;
            case SR_B_IMM:
                Sr_B_Imm(getByteImm());
                break;
            case BUILTIN:
                HandleBuiltin(getByteImm());
                break;
            case ALLOC_IMM:
                Alloc(getQwordImm());
                break;
            case ALLOC_A:
                Alloc(REG_A);
                break;
            case ALLOC_B:
                Alloc(REG_B);
                break;
            case DEALLOC:
                free(BP);
                break;
            case EXIT:
                goto exit;
            default:
                printf("Error: Unrecognized opcode: 0x%X\n", curOpcode);
                exit(1);
        }
    }

    exit:
        free(CB);
        free(SB);
}

static inline word getWordImm() {
    word* val = (word*) PC;
    PC += sizeof(word);
    return *val;
}

static inline qword getQwordImm() {
    qword* val = (qword*) PC;
    PC += sizeof(qword);
    return *val;
}

static inline void Lda_Imm(qword imm) {
    REG_A = imm;
}

static inline void Ldb_Imm(qword imm) {
    REG_B = imm;
}

static inline void Lda_B() {
    REG_A = REG_B;
}

static inline void Ldb_A() {
    REG_B = REG_A;
}

static inline void Add_A_B() {
    REG_A += REG_B;
}

static inline void Add_B_A() {
    REG_B += REG_A;
}

static inline void Add_A_Imm(qword imm) {
    REG_A += imm;
}

static inline void Add_B_Imm(qword imm) {
    REG_B += imm;
}

static inline void And_A_B() {
    REG_A = REG_A & REG_B;
}

static inline void And_B_A() {
    REG_B = REG_B & REG_A;
}

static inline void And_A_Imm(qword imm) {
    REG_A = REG_A & imm;
}

static inline void And_B_Imm(qword imm) {
    REG_B = REG_B & imm;
}

static inline void Inv_A() {
    REG_A = ~REG_A;
}

static inline void Inv_B() {
    REG_B = ~REG_B;
}

static inline void Sl_A_Imm(byte imm) {
    REG_A = REG_A << imm;
}

static inline void Sl_B_Imm(byte imm) {
    REG_B = REG_B << imm;
}

static inline void Sr_A_Imm(byte imm) {
    REG_A = REG_A >> imm;
}

static inline void Sr_B_Imm(byte imm) {
    REG_B = REG_B >> imm;
}

static inline void Alloc(qword size) {
    BP = new_array(byte, size);
}

static inline void HandleBuiltin(byte imm) {
    switch (imm) {
    case PRINT_ASCII_A:
        printf("%c", (char) REG_A);
        break;
    case PRINT_HEX_A:
        printf("0x%llX", (qword) REG_A);
        break;
    case PRINT_ASCII_B:
        printf("%c", (char) REG_B);
        break;
    case PRINT_HEX_B:
        printf("0x%llX", (qword) REG_B);
        break;
    case DUMP_REGS:
        printRegs();
        break;
    default:
        printf("Unrecognized Builtin Call %d", imm);
        exit(1);
    }
}

void printRegs() {
    printf("\nRegisters:\n");
    printf("A:   0x%llx\nB:   0x%llx\n", REG_A, REG_B);
    printf("SP:  0x%llx\nPC:  0x%llx\n", (qword) SP, (qword) PC);
    printf("FO:  0x%llx\nBP:  0x%llx\n", (qword) FO, (qword) BP);
}