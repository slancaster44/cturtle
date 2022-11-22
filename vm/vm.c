#include "vm.h"
#include "mem_mac.h"
#include "instructions.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Debugging Functions */
qword getA() { return REG_A; }
qword getB() { return REG_B; }
qword peakBp(qword off) { return BP[off]; }
void freeBP() { free(BP); }

#define decodeFirstReg(VAL) VAL >> 4
#define decodeSecondReg(VAL) VAL & 0b00001111
#define INITIAL_stack_limit 2

#define getByteImm() *(PC++)
static inline word getWordImm();
static inline qword getQwordImm();

void Execute(byte* code, int code_length) {
    CB = new_array(byte, code_length);
    memcpy(CB, code, code_length);

    SB = new_array(qword, INITIAL_stack_limit);
    SP = SB;
    stack_limit = INITIAL_stack_limit;

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
            case LDM_BPOFF_A:
                Ldm_BpOff_A(getQwordImm());
                break;
            case LDM_BPOFF_B:
                Ldm_BpOff_B(getQwordImm());
                break;
            case LDA_BP_OFF:
                Lda_BpOff(getQwordImm());
                break;
            case LDB_BP_OFF:
                Ldb_BpOff(getQwordImm());
                break;
            case LDA_BPOFF_B:
                Lda_BpOffB();
                break;
            case LDB_BPOFF_A:
                Ldb_BpOffA();
                break;
            case LDM_BPOFFA_B:
                Ldm_BpOffA_B();
                break;
            case LDM_BPOFFB_A:
                Ldm_BpOffB_A();
                break;
            case PUSH_IMM:
                Push_Imm(getQwordImm());
                break;
            case POP_A:
                Pop_A();
                break;
            case POP_B:
                Pop_B();
                break;
            case PUSH_A:
                Push_A();
                break;
            case PUSH_B:
                Push_B();
                break;
            case PUSH_BP:
                Push_Bp();
                break;
            case POP_BP:
                Pop_Bp();
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
            case SUB_A_B:
                Sub_A_B();
                break;
            case SUB_B_A:
                Sub_B_A();
                break;
            case SUB_A_IMM:
                Sub_A_Imm(getQwordImm());
                break;
            case SUB_B_IMM:
                Sub_B_Imm(getQwordImm());
                break;
            case MUL_A_B:
                Mul_A_B();
                break;
            case MUL_B_A:
                Mul_B_A();
                break;
            case MUL_A_IMM:
                Mul_A_Imm(getQwordImm());
                break;
            case MUL_B_IMM:
                Mul_B_Imm(getQwordImm());
                break;
            case DIV_A_B:
                Div_A_B();
                break;
            case DIV_B_A:
                Div_B_A();
                break;
            case DIV_A_IMM:
                Div_A_Imm(getQwordImm());
                break;
            case DIV_B_IMM:
                Div_B_Imm(getQwordImm());
                break;
            case POW_A_B:
                Pow_A_B();
                break;
            case POW_B_A:
                Pow_B_A();
                break;
            case POW_A_IMM:
                Pow_A_Imm(getQwordImm());
                break;
            case POW_B_IMM:
                Pow_B_Imm(getQwordImm());
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
            case OR_A_B:
                Or_A_B();
                break;
            case OR_B_A:
                Or_B_A();
                break;
            case OR_A_IMM:
                Or_A_Imm(getQwordImm());
                break;
            case OR_B_IMM:
                Or_B_Imm(getQwordImm());
                break;
            case XOR_A_B:
                Xor_A_B();
                break;
            case XOR_B_A:
                Xor_B_A();
                break;
            case XOR_A_IMM:
                Xor_A_Imm(getQwordImm());
                break;
            case XOR_B_IMM:
                Xor_B_Imm(getQwordImm());
                break;
            case EQ_A_B:
                Eq_A_B();
                break;
            case EQ_B_A:
                Eq_B_A();
                break;
            case EQ_A_IMM:
                Eq_A_Imm(getQwordImm());
                break;
            case EQ_B_IMM:
                Eq_B_Imm(getQwordImm());
                break;
            case NE_A_B:
                Ne_A_B();
                break;
            case NE_B_A:
                Ne_B_A();
                break;
            case NE_A_IMM:
                Ne_A_Imm(getQwordImm());
                break;
            case NE_B_IMM:
                Ne_B_Imm(getQwordImm());
                break;
            case GT_A_B:
                Gt_A_B();
                break;
            case GT_B_A:
                Gt_B_A();
                break;
            case GT_A_IMM:
                Gt_A_Imm(getQwordImm());
                break;
            case GT_B_IMM:
                Gt_B_Imm(getQwordImm());
                break;
            case LT_A_B:
                Lt_A_B();
                break;
            case LT_B_A:
                Lt_B_A();
                break;
            case LT_A_IMM:
                Lt_A_Imm(getQwordImm());
                break;
            case LT_B_IMM:
                Lt_B_Imm(getQwordImm());
                break;
            case GE_A_B:
                Ge_A_B();
                break;
            case GE_B_A:
                Ge_B_A();
                break;
            case GE_A_IMM:
                Ge_A_Imm(getQwordImm());
                break;
            case GE_B_IMM:
                Ge_B_Imm(getQwordImm());
                break;
            case LE_A_B:
                Le_A_B();
                break;
            case LE_B_A:
                Le_B_A();
                break;
            case LE_A_IMM:
                Le_A_Imm(getQwordImm());
                break;
            case LE_B_IMM:
                Le_B_Imm(getQwordImm());
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
            case JPB_OFF:
                Jpb_Offset(getQwordImm());
                break;
            case JPA_NZ_OFF:
                Jpa_Nz_Offset(getQwordImm());
                break;
            case JPB_NZ_OFF:
                Jpb_Nz_Offset(getQwordImm());
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

void ensureStackAccomodations(int numQwords) {
    int diff = SP - SB;
    if ((diff >> 3) + numQwords > stack_limit) {
        resize_array(qword, SB, stack_limit, stack_limit+=stack_limit);
        SP = SB + diff;
    } else if ((diff >> 3) < (stack_limit >> 2) && 
        stack_limit > INITIAL_stack_limit) {

        resize_array(qword, SB, stack_limit, stack_limit >>= 1);
        SP = SB + diff;
    }
}

static inline void Push_Imm(qword imm) {
    ensureStackAccomodations(1);
    *SP = imm;
    SP += sizeof(qword);
}

static inline void Push_A() {
    ensureStackAccomodations(1);
    *SP = REG_A;
    SP += sizeof(qword);
}

static inline void Push_B() {
    ensureStackAccomodations(1);
    *SP = REG_B;
    SP += sizeof(qword);
}

static inline void Pop_A() {
    SP -= sizeof(qword);
    REG_A = *SP;
}

static inline void Pop_B() {
    SP -= sizeof(qword);
    REG_B = *SP;
}

static inline void Push_Bp() {
    ensureStackAccomodations(1);
    *SP = (qword) BP;
    SP += sizeof(qword);
}

static inline void Pop_Bp() {
    SP -= sizeof(qword);
    BP = (qword*) *SP;
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

static inline void Ldm_BpOff_A(qword offset) {
    BP[offset] = REG_A;
}

static inline void Ldm_BpOff_B(qword offset) {
    BP[offset] = REG_B;
}

static inline void Lda_BpOff(qword offset) {
    REG_A = BP[offset];
}

static inline void Ldb_BpOff(qword offset) {
    REG_B = BP[offset];
}

static inline void Lda_BpOffB() {
    REG_A = BP[REG_B];
}

static inline void Ldb_BpOffA() {
    REG_B = BP[REG_A];
}

static inline void Ldm_BpOffA_B() {
    BP[REG_A] = REG_B;
}

static inline void Ldm_BpOffB_A() {
    BP[REG_B] = REG_A;
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

static inline void Sub_A_B() {
    REG_A -= REG_B;
}

static inline void Sub_B_A() {
    REG_B -= REG_A;
}

static inline void Sub_A_Imm(qword imm) {
    REG_A -= imm;
}

static inline void Sub_B_Imm(qword imm) {
    REG_B -= imm;
}

static inline void Mul_A_B() {
    REG_A *= REG_B;
}

static inline void Mul_B_A() {
    REG_B *= REG_A;
}

static inline void Mul_A_Imm(qword imm) {
    REG_A *= imm;
}

static inline void Mul_B_Imm(qword imm) {
    REG_B *= imm;
}

static inline void Div_A_B() {
    REG_A /= REG_B;
}

static inline void Div_B_A() {
    REG_B /= REG_A;
}

static inline void Div_A_Imm(qword imm) {
    REG_A /= imm;
}

static inline void Div_B_Imm(qword imm) {
    REG_B /= imm;
}

static inline void Pow_A_B() {
    REG_A = (qword) pow((double) REG_A, (double) REG_B);
}

static inline void Pow_B_A() {
    REG_B = (qword) pow((double) REG_B, (double) REG_A);
}

static inline void Pow_A_Imm(qword imm) {
    REG_A = (qword) pow((double) REG_A, (double) imm);
}

static inline void Pow_B_Imm(qword imm) {
    REG_B = (qword) pow((double) REG_B, (double) imm);
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

static inline void Or_A_B() {
    REG_A = REG_A | REG_B;
}

static inline void Or_B_A() {
    REG_B = REG_B | REG_A;
}

static inline void Or_A_Imm(qword imm) {
    REG_A = REG_A | imm;
}

static inline void Or_B_Imm(qword imm) {
    REG_B = REG_B | imm;
}

static inline void Xor_A_B() {
    REG_A = REG_A ^ REG_B;
}

static inline void Xor_B_A() {
    REG_B = REG_B ^ REG_A;
}

static inline void Xor_A_Imm(qword imm) {
    REG_A = REG_A ^ imm;
}

static inline void Xor_B_Imm(qword imm) {
    REG_B = REG_B ^ imm;
}

static inline void Eq_A_B() {
    REG_A = REG_A == REG_B;
}

static inline void Eq_B_A() {
    REG_B = REG_B == REG_A;
}

static inline void Eq_A_Imm(qword imm) {
    REG_A = REG_A == imm;
}

static inline void Eq_B_Imm(qword imm) {
    REG_B = REG_B == imm;
}

static inline void Ne_A_B() {
    REG_A = REG_A != REG_B;
}

static inline void Ne_B_A() {
    REG_B = REG_B != REG_A;
}

static inline void Ne_A_Imm(qword imm) {
    REG_A = REG_A != imm;
}

static inline void Ne_B_Imm(qword imm) {
    REG_B = REG_B != imm;
}

static inline void Gt_A_B() {
    REG_A = REG_A > REG_B;
}

static inline void Gt_B_A() {
    REG_B = REG_B > REG_A;
}

static inline void Gt_A_Imm(qword imm) {
    REG_A = REG_A > imm;
}

static inline void Gt_B_Imm(qword imm) {
    REG_B = REG_B > imm;
}

static inline void Lt_A_B() {
    REG_A = REG_A < REG_B;
}

static inline void Lt_B_A() {
    REG_B = REG_B < REG_A;
}

static inline void Lt_A_Imm(qword imm) {
    REG_A = REG_A < imm;
}

static inline void Lt_B_Imm(qword imm) {
    REG_B = REG_B < imm;
}

static inline void Ge_A_B() {
    REG_A = REG_A >= REG_B;
}

static inline void Ge_B_A() {
    REG_B = REG_B >= REG_A;
}

static inline void Ge_A_Imm(qword imm) {
    REG_A = REG_A >= imm;
}

static inline void Ge_B_Imm(qword imm) {
    REG_B = REG_B >= imm;
}

static inline void Le_A_B() {
    REG_A = REG_A <= REG_B;
}

static inline void Le_B_A() {
    REG_B = REG_B <= REG_A;
}

static inline void Le_A_Imm(qword imm) {
    REG_A = REG_A <= imm;
}

static inline void Le_B_Imm(qword imm) {
    REG_B = REG_B <= imm;
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

static inline void Jpb_Offset(qword off) {
    if (REG_B)
        PC = CB + off;
}

static inline void Jpa_Nz_Offset(qword off) {
    if (REG_A != 0)
        PC = CB + off;
}

static inline void Jpb_Nz_Offset(qword off) {
    if (REG_B != 0)
        PC = CB + off;
}

static inline void Alloc(qword size) {
    BP = new_array(qword, size);
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