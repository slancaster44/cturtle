#ifndef VM_H
#define VM_H

#include <stdint.h>

#define byte uint8_t
#define word uint16_t
#define qword uint64_t

/*Reserve r14, r15, r13 for use as register a, b & pc */
register qword REG_A    asm("r14");
register qword REG_B    asm("r15");
register byte* PC       asm("r13");

struct VM {
    byte* sp; /* Pointer to Top Of Stack */
    byte* sb; /* Pointer to base of stack */
    qword fo; /* Offset from base of stack of a frame */
    byte* bp; /* Pointer to current buffer we are working on */

    int codelen;
    byte* code;
    
    byte* stack;
};

void Execute(byte* code, int code_length);
void printRegs(struct VM* vm);

static inline void Lda_Imm(qword imm);
static inline void Ldb_Imm(qword imm);
static inline void Lda_B();
static inline void Ldb_A();

static inline void Add_A_B();
static inline void Add_B_A();
static inline void Add_A_A();
static inline void Add_B_B();
static inline void Add_A_Imm(qword imm);
static inline void Add_B_Imm(qword imm);

static inline void And_A_B();
static inline void And_B_A();
static inline void And_A_Imm(qword imm);
static inline void And_B_Imm(qword imm);

static inline void Inv_A();
static inline void Inv_B();

static inline void Asl_A_Imm(byte imm);
static inline void Asl_B_Imm(byte imm);
static inline void Asr_A_Imm(byte imm);
static inline void Asr_B_Imm(byte imm);

static inline void Jp_Ne_Imm(struct VM* vm, qword imm);
static inline void Jp_Eq_Imm(struct VM* vm, qword imm);
static inline void Jp_Gt_Imm(struct VM* vm, qword imm);
static inline void Jp_Lt_Imm(struct VM* vm, qword imm);
static inline void Jp_Ge_Imm(struct VM* vm, qword imm);
static inline void Jp_Le_Imm(struct VM* vm, qword imm);

static inline void Alloc(struct VM* vm, qword size);

static inline void HandleBuiltin(struct VM* vm, byte imm);
#endif