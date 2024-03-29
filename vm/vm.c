#include "vm.h"
#include "mem_mac.h"
#include "buffer.h"
#include "instructions.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Debugging Functions */
qword getA() { return REG_A; }
qword getB() { return REG_B; }

qword getQBS() { return QBS; }
qword getRBS() { return RBS; }
struct Buffer* getBPA() { return BPA; }
struct Buffer* getBPB() { return BPB; }
void freeBPA() { Dealloc_Bpa(); }
void freeBPB() { Dealloc_Bpb(); }
bool getExitFlag() { return ExitFlag; }
byte* getPC() { return PC; }
int getCodeLen() { return codelen; }
byte* getCB() { return CB; }

void printBuffer(struct Buffer* b) {
    if (b == NULL)
        return;

    printf("[");
    for (int i = 0; i < b->numQwords; i ++) {
        printf("%lu", b->QwordBuffer[i]);
        if (i != b->numQwords-1)
            printf(", ");
    }

    for (int i = 0; i < b->numReferences; i++) {
        printBuffer(b->References[i]);
        if (i != b->numReferences-1)
            printf(", ");
    }

    printf("]");
}

/* Helper macros & functions */
#define decodeFirstReg(VAL) VAL >> 4
#define decodeSecondReg(VAL) VAL & 0b00001111
#define INITIAL_STACK_LIMIT 2

#define getByteImm() *(PC++)
static inline word getWordImm();
static inline qword getQwordImm();

void initVM(byte* code, int code_length) {
    CB = new_array(byte, code_length);
    memcpy(CB, code, code_length);

    SB = new_array(qword, INITIAL_STACK_LIMIT);
    SP = 0;
    stack_limit = INITIAL_STACK_LIMIT;

    BufferStack = new_array(struct Buffer*, INITIAL_STACK_LIMIT);
    BSP = 0;
    bufferStackLimit = INITIAL_STACK_LIMIT;

    for (int i = 0; i < INITIAL_STACK_LIMIT; i ++) {
        BufferStack[i] = NULL;
    }

    if (sizeof(qword) != sizeof(qword*)) {
        printf("Word Size Misalignment");
        exit(1);
    }

    codelen = code_length;

    PC = CB;

    REG_A = 0;
    REG_B = 0;
}

int executeInstruction() {
    enum Opcode curOpcode = getByteImm();
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
        case AND_B_IMM:
            And_B_Imm(getQwordImm());
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
        case INVA:
            Inv_A();
            break;
        case INVB:
            Inv_B();
            break;
        case JP_OFF:
            Jp_Offset(getQwordImm());
            break;
        case JPB_OFF:
            Jpb_Offset(getQwordImm());
            break;
        case JPA_OFF:
            Jpa_Offset(getQwordImm());
            break;
        case JPA_Z_OFF:
            Jpa_Z_Offset(getQwordImm());
            break;
        case JPB_Z_OFF:
            Jpb_Z_Offset(getQwordImm());
            break;
        case CALL_OFF:
            Call_Offset(getQwordImm());
            break;
        case RET:
            Ret();
            break;
        case LDRBS_IMM:
            Ldrbs_Imm(getQwordImm());
            break;
        case LDRBS_A:
            Ldrbs_A();
            break;
        case LDRBS_B:
            Ldrbs_B();
            break;
        case LDQBS_IMM:
            Ldqbs_Imm(getQwordImm());
            break;
        case LDQBS_A:
            Ldqbs_A();
            break;
        case LDQBS_B:
            Ldqbs_B();
            break;
        case ALLOC_BPA:
            Alloc_Bpa();
            break;
        case ALLOC_BPB:
            Alloc_Bpb();
            break;
        case DEALLOC_BPA:
            Dealloc_Bpa();
            break;
        case DEALLOC_BPB:
            Dealloc_Bpb();
            break;
        case LDM_BPAOFFIMM_IMM:
            Ldm_BpaOffImm_Imm(getQwordImm(), getQwordImm());
            break;
        case LDM_BPAOFFIMM_A:
            Ldm_BpaOffImm_A(getQwordImm());
            break;
        case LDM_BPAOFFIMM_B:
            Ldm_BpaOffImm_B(getQwordImm());
            break;
        case LDA_BPAOFFIMM:
            Lda_BpaOffImm(getQwordImm());
            break;
        case LDB_BPAOFFIMM:
            Ldb_BpaOffImm(getQwordImm());
            break;
        case LDM_BPBOFFIMM_IMM:
            Ldm_BpbOffImm_Imm(getQwordImm(), getQwordImm());
            break;
        case LDM_BPBOFFIMM_A:
            Ldm_BpbOffImm_A(getQwordImm());
            break;
        case LDM_BPBOFFIMM_B:
            Ldm_BpbOffImm_B(getQwordImm());
            break;
        case LDA_BPBOFFIMM:
            Lda_BpbOffImm(getQwordImm());
            break;
        case LDB_BPBOFFIMM:
            Ldb_BpbOffImm(getQwordImm());
            break;
        case LDM_BPAOFFIMM_BPB:
            Ldm_BpaOffImm_Bpb(getQwordImm());
            break;
        case LDBPB_BPAOFFIMM:
            Ldbpb_BpaOffImm(getQwordImm());
            break;
        case LDM_BPBOFFIMM_BPA:
            Ldm_BpbOffImm_Bpa(getQwordImm());
            break;
        case LDBPA_BPBOFFIMM:
            Ldbpa_BpbOffImm(getQwordImm());
            break;
        case LDM_BPAOFFA_IMM:
            Ldm_BpaOffA_Imm(getQwordImm());
            break;
        case LDB_BPAOFFA:
            Ldb_BpaOffA();
            break;
        case LDM_BPBOFFB_IMM:
            Ldm_BpbOffB_Imm(getQwordImm());
            break;
        case LDA_BPBOFFB:
            Lda_BpbOffB();
            break;
        case LDM_BPBOFFA_IMM:
            Ldm_BpbOffA_Imm(getQwordImm());
            break;
        case LDM_BPAOFFA_BPB:
            Ldm_BpaOffA_Bpb();
            break;
        case LDBPB_BPAOFFA:
            Ldbpb_BpaOffA();
            break;
        case LDB_BPBOFFA:
            Ldb_BpbOffA();
            break;
        case LDM_BPAOFFB_BPB:
            Ldm_BpaOffB_Bpb();
            break;
        case LDBPB_BPAOFFB:
            Ldbpb_BpaOffB();
            break;
        case LDM_BPBOFFA_BPA:
            Ldm_BpbOffA_Bpa();
            break;
        case LDBPA_BPBOFFA:
            Ldbpa_BpbOffA();
            break;
        case LDM_BPAOFFB_IMM:
            Ldm_BpaOffB_Imm(getQwordImm());
            break;
        case LDM_BPBOFFB_BPA:
            Ldm_BpbOffB_Bpa();
            break;
        case LDBPA_BPBOFFB:
            Ldbpa_BpbOffB();
            break;
        case LDA_BPAOFFB:
            Lda_BpaOffB();
            break;
        case LDM_BPAOFFB_A:
            Ldm_BpaOffB_A();
            break;
        case LDM_BPBOFFB_A:
            Ldm_BpbOffB_A();
            break;
        case LDM_BPBOFFA_B:
            Ldm_BpbOffA_B();
            break;
        case LDM_BPAOFFA_B:
            Ldm_BpaOffA_B();
            break;
        case PUSH_BPA:
            Push_Bpa();
            break;
        case PUSH_BPB:
            Push_Bpb();
            break;
        case POP_BPA:
            Pop_Bpa();
            break;
        case POP_BPB:
            Pop_Bpb();
            break;
        case ENSURE_STACK_SIZE:
            Ensure_Stack_Size(getQwordImm());
            break;
        case INSERT_STACK_IMM_A:
            Insert_Stack_Imm_A(getQwordImm());
            break;
        case LDA_STACK_IMM:
            Lda_Stack_Imm(getQwordImm());
            break;
        case LDBPB_BPA:
            BPB = BPA;
            break;
        case LDBPA_BPB:
            BPA = BPB;
            break;
        case INSERT_BUFFER_STACK_IMM_BPA:
            Insert_Buffer_Stack_Imm_BPA(getQwordImm());
            break;
        case ENSURE_BUFFER_STACK_SIZE:
            Ensure_Buffer_Stack_Size(getQwordImm());
            break;
        case LDBPA_STACK_IMM:
            Ldbpa_Stack_Imm(getQwordImm());
            break;
        case SHRINK_BUFFER_STACK_SIZE:
            BSP -= getQwordImm();
            break;
        case SHRINK_STACK_SIZE:
            SP -= getQwordImm();
            break;
        case BUILTIN:
            HandleBuiltin(getByteImm());
            break;
        case EXIT:
            goto exit;
        default:
            printf("Error: Unrecognized opcode: 0x%X (%d)\n", curOpcode, curOpcode);
            exit(1);
    }

    return 1;

    exit:
        ExitFlag = true;
        free(CB);
        free(SB);

    return 0;
}

void Execute(byte* code, int code_length) {
    initVM(code, code_length);
    while (executeInstruction());
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
    int stack_used = SP + numQwords;

    if (stack_used >= stack_limit) {
        int old_size = SP;
        stack_limit = stack_used * 2;

        expand_array(qword, SB, old_size, stack_limit);

    } else if (stack_used <= (stack_limit / 4) && (stack_limit / 2) > INITIAL_STACK_LIMIT) {
        int old_size = SP;
        stack_limit /= 2;

        shrink_array(qword, SB, old_size, stack_limit);
    }
}

void ensureBufferStackAccomodations(int numBuffers) {
    int stack_used = BSP + numBuffers;

    if (stack_used >= bufferStackLimit) {
        int old_size = BSP;
        bufferStackLimit = stack_used * 2;

        expand_array(struct Buffer*, BufferStack, old_size, bufferStackLimit);

    } else if (stack_used <= (bufferStackLimit / 4) &&
        (bufferStackLimit / 2) > INITIAL_STACK_LIMIT) {
            int old_size = BSP;
            bufferStackLimit /= 2;

            shrink_array(struct Buffer*, BufferStack, old_size, bufferStackLimit);
    }
}

static inline void Push_Imm(qword imm) {
    ensureStackAccomodations(1);
    SB[SP] = imm;
    SP += 1;
}

static inline void Push_A() {
    ensureStackAccomodations(1);
    SB[SP] = REG_A;
    SP += 1;
}

static inline void Push_B() {
    ensureStackAccomodations(1);
    SB[SP] = REG_B;
    SP += 1;
}

static inline void Pop_A() {
    SP -= 1;
    REG_A = SB[SP];
}

static inline void Pop_B() {
    SP -= 1;
    REG_B = SB[SP];
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

static inline void Jpa_Offset(qword off) {
    if (REG_A)
        PC = CB + off;
}

static inline void Jpb_Offset(qword off) {
    if (REG_B)
        PC = CB + off;
}

static inline void Jp_Offset(qword off){
    PC = CB + off;
}

static inline void Jpa_Z_Offset(qword off) {
    if (REG_A == 0)
        PC = CB + off;
}

static inline void Jpb_Z_Offset(qword off) {
    if (REG_B == 0)
        PC = CB + off;
}

static inline void Call_Offset(qword off) {
    Push_Imm((qword) PC);
    PC = CB + off;
}

static inline void Ret() {
    SP -= 1;
    PC = (byte*) SB[SP];
}

static inline void Ldqbs_Imm(qword imm) {
    QBS = imm;
}

static inline void Ldrbs_Imm(qword imm) {
    RBS = imm;
}

static inline void Ldqbs_A(qword imm) {
    QBS = REG_A;
}

static inline void Ldrbs_A(qword imm) {
    RBS = REG_A;
}

static inline void Ldqbs_B(qword imm) {
    QBS = REG_B;
}

static inline void Ldrbs_B(qword imm) {
    RBS = REG_B;
}

static inline void Alloc_Bpa() {
    BPA = new(struct Buffer);
    
    BPA->numQwords = QBS;
    BPA->numReferences = RBS;

    if (QBS != 0)
        BPA->QwordBuffer = new_array(qword, QBS);
    if (RBS != 0)
        BPA->References = new_array(struct Buffer*, RBS);

    for (int i = 0; i < RBS; i ++) {
        BPA->References[i] = NULL;
    }
}

static inline void Alloc_Bpb() {
    BPB = new(struct Buffer);
    
    BPB->numQwords = QBS;
    BPB->numReferences = RBS;

    if (QBS != 0) 
        BPB->QwordBuffer = new_array(qword, QBS);
    
    if (RBS != 0) 
        BPB->References = new_array(struct Buffer*, RBS);

    for (int i = 0; i < RBS; i ++) {
        BPB->References[i] = NULL;
    }
}

static inline void Dealloc_Bpa() {
    free(BPA->References); 
    free(BPA->QwordBuffer); 
    free(BPA);
}

static inline void Dealloc_Bpb() {
    free(BPB->References); 
    free(BPB->QwordBuffer); 
    free(BPB);
}

static inline void Ldm_BpaOffImm_Imm(qword offset, qword imm) {
    BPA->QwordBuffer[offset] = imm;
}

static inline void Ldm_BpaOffImm_A(qword offset) {
    BPA->QwordBuffer[offset] = REG_A;
}

static inline void Ldm_BpaOffImm_B(qword offset) {
    BPA->QwordBuffer[offset] = REG_B;
}

static inline void Lda_BpaOffImm(qword offset) {
    REG_A = BPA->QwordBuffer[offset];
}

static inline void Ldb_BpaOffImm(qword offset) {
    REG_B = BPA->QwordBuffer[offset];
}

static inline void Ldm_BpbOffImm_Imm(qword offset, qword imm) {
    BPB->QwordBuffer[offset] = imm;
}

static inline void Ldm_BpbOffImm_A(qword offset) {
    BPB->QwordBuffer[offset] = REG_A;
}

static inline void Ldm_BpbOffImm_B(qword offset) {
    BPB->QwordBuffer[offset] = REG_B;
}

static inline void Lda_BpbOffImm(qword offset) {
    REG_A = BPB->QwordBuffer[offset];
}

static inline void Ldb_BpbOffImm(qword offset) {
    REG_B = BPB->QwordBuffer[offset];
}

static inline void Ldm_BpbOffA_Imm(qword imm) {
    BPB->QwordBuffer[REG_A] = imm;
}

static inline void Ldm_BpbOffB_Imm(qword imm) {
    BPB->QwordBuffer[REG_B] = imm;
}

static inline void Ldm_BpaOffA_Imm(qword imm) {
    BPA->QwordBuffer[REG_A] = imm;
}

static inline void Ldm_BpaOffB_Imm(qword imm) {
    BPA->QwordBuffer[REG_B] = imm;
}

static inline void Lda_BpaOffB() {
    REG_A = BPA->QwordBuffer[REG_B];
}

static inline void Ldb_BpaOffA() {
    REG_B = BPA->QwordBuffer[REG_A];
}

static inline void Lda_BpbOffB() {
    REG_A = BPB->QwordBuffer[REG_B];
}

static inline void Ldb_BpbOffA() {
    REG_B = BPB->QwordBuffer[REG_A];
}

static inline void Ldm_BpaOffImm_Bpa(qword offset) {
    BPA->References[offset] = BPA;
}

static inline void Ldm_BpaOffA_Bpa() {
    BPA->References[REG_A] = BPA;
}

static inline void Ldm_BpaOffB_Bpa() {
    BPA->References[REG_B] = BPA;
}

static inline void Ldm_BpaOffImm_Bpb(qword offset) {
    BPA->References[offset] = BPB;
}

static inline void Ldm_BpaOffA_Bpb() {
    BPA->References[REG_A] = BPB;
}

static inline void Ldm_BpaOffB_Bpb() {
    BPA->References[REG_B] = BPB;
}

//
static inline void Ldm_BpbOffImm_Bpa(qword offset) {
    BPB->References[offset] = BPA;
}

static inline void Ldm_BpbOffA_Bpa() {
    BPB->References[REG_A] = BPA;
}

static inline void Ldm_BpbOffB_Bpa() {
    BPB->References[REG_B] = BPA;
}

static inline void Ldm_BpbOffImm_Bpb(qword offset) {
    BPB->References[offset] = BPB;
}

static inline void Ldm_BpbOffA_Bpb() {
    BPB->References[REG_A] = BPB;
}

static inline void Ldm_BpbOffB_Bpb() {
    BPB->References[REG_B] = BPB;
}

static inline void Ldbpa_BpbOffImm(qword offset) {
    BPA = BPB->References[offset];
}

static inline void Ldbpa_BpbOffA() {
    BPA = BPB->References[REG_A];
}

static inline void Ldbpa_BpbOffB() {
    BPA = BPB->References[REG_B];
}

static inline void Ldbpb_BpaOffImm(qword offset) {
    BPB = BPA->References[offset];
}

static inline void Ldbpb_BpaOffA() {
    BPB = BPA->References[REG_A];
}

static inline void Ldbpb_BpaOffB() {
    BPB = BPA->References[REG_B];
}

static inline void Ldm_BpaOffB_A() {
    BPA->QwordBuffer[REG_B] = REG_A;
}

static inline void Ldm_BpbOffB_A() {
    BPB->QwordBuffer[REG_B] = REG_A;
}

static inline void Ldm_BpbOffA_B() {
    BPB->QwordBuffer[REG_A] = REG_B;
}

static inline void Ldm_BpaOffA_B() {
    BPA->QwordBuffer[REG_A] = REG_B;
}

static inline void Ldbpsfo() {
    BSFO = BSP;
}

static inline void Push_Bpa() {
    ensureBufferStackAccomodations(1);
    BufferStack[BSP] = BPA;
    BSP ++;
}

static inline void Pop_Bpb() {
    BSP --;
    BPB = BufferStack[BSP];
}

static inline void Push_Bpb() {
    ensureBufferStackAccomodations(1);
    BufferStack[BSP] = BPB;
    BSP ++;
}

static inline void Pop_Bpa() {
    BSP --;
    BPA = BufferStack[BSP];
}

static inline void Insert_Stack_Imm_A(qword imm) {
    SB[imm] = REG_A;
}

static inline void Ensure_Stack_Size(qword imm) {
    if (SP < imm) {
        ensureStackAccomodations(imm - SP);
        SP += imm;
    }
}

static inline void Lda_Stack_Imm(qword imm) {
    REG_A = SB[imm];
}

static inline void Insert_Buffer_Stack_Imm_BPA(qword imm) {
    BufferStack[imm] = BPA;
}

static inline void Ensure_Buffer_Stack_Size(qword imm) {
    if (BSP < imm) {
        ensureStackAccomodations(imm - BSP);
        BSP += imm;
    }
}

static inline void Ldbpa_Stack_Imm(qword imm) {
    BPA = BufferStack[imm];
}

static inline void HandleBuiltin(byte imm) {
    switch (imm) {
    case PRINT_ASCII_A:
        printf("%c", (char) REG_A);
        break;
    case PRINT_HEX_A:
        printf("0x%lX", (qword) REG_A);
        break;
    case PRINT_ASCII_B:
        printf("%c", (char) REG_B);
        break;
    case PRINT_HEX_B:
        printf("0x%lX", (qword) REG_B);
        break;
    case DUMP_REGS:
        printRegs();
        break;
    case PRINT_ASCII_BPA:
        for (int i = 0; i < BPA->numQwords; i ++) {
            printf("%c", (char) BPA->QwordBuffer[i]);
        }
        break;
    case PRINT_BPA:
        printBuffer(BPA);
        break;
    default:
        printf("Unrecognized Builtin Call %d", imm);
        exit(1);
    }
}

void printRegs() {
    printf("\nRegisters:\n");
    printf("A:   0x%lX\nB:   0x%lX\n", REG_A, REG_B);
    printf("SP:  0x%lX\nPC:  0x%lX\n", (qword) SP, (qword) PC);
    printf("FO:  0x%lX\n", (qword) FO);
}

void printStack() {
    printf("Stack:\n");
    for (int count = SP-1; count >= 0; count--) {
        printf("%lX: %lX\n", (qword) count, SB[count]);
    }
}