#ifndef VM_H
#define VM_H

#include <stdint.h>

#define byte uint8_t
#define word uint16_t
#define qword uint64_t


qword REG_A;
qword REG_B;
byte* PC;
byte* SP; /* Pointer to Top Of Stack */
qword FO; /* Offset from base of stack of the current stack frame */
byte* BP; /* Pointer to current buffer we are working on */

int codelen;
byte* CB; /* Starting address of code */

int stack_size;
byte* SB; /* Starting address of stack */

/* Utility */
void Execute(byte* code, int code_length);
void printRegs();

/* Basic Load Instructions */
static inline void Lda_Imm(qword imm); //FI SI
static inline void Ldb_Imm(qword imm); //FI SI
static inline void Lda_B(); //FI SI
static inline void Ldb_A(); //FI SI

/* Memory Management Instructions */
static inline void Lda_BpOff(qword offset);
static inline void Lda_SpOff(qword offset);
static inline void Lda_SpNoff(qword negative_offset);
static inline void Lda_FoOff(qword offset);
static inline void Lda_FoNoff(qword negative_offset);

static inline void Ldb_BpOff(qword offset);
static inline void Ldb_SpOff(qword offset);
static inline void Ldb_SpNoff(qword negative_offset);
static inline void Ldb_FoOff(qword offset);
static inline void Ldb_FoNoff(qword negative_offset);

static inline void Ldm_BpOff_A(qword offset);
static inline void Ldm_SpOff_A(qword offset);
static inline void Ldm_SpNoff_A(qword offset);
static inline void Ldm_FoOff_A(qword offset);
static inline void Ldm_FoNoff_A(qword offset);

static inline void Ldm_BpOff_B(qword offset);
static inline void Ldm_SpOff_B(qword offset);
static inline void Ldm_SpNoff_B(qword offset);
static inline void Ldm_FoOff_B(qword offset);
static inline void Ldm_FoNoff_B(qword offset);

static inline void Ldbp_FoOff(qword offset);
static inline void Ldbp_FoNoff(qword offset);
static inline void Ldbp_SpOff(qword offset);
static inline void Ldbp_SpNoff(qword offset);

static inline void Ldfo();

static inline void Sub_Sp_Imm(qword imm);
static inline void Sub_Sp_A();
static inline void Sub_Sp_B();

static inline void Sub_Sp_Imm(qword imm);
static inline void Sub_Sp_A();
static inline void Sub_Sp_B();

static inline void Add_Sp_Imm(qword imm);
static inline void Add_Sp_A();
static inline void Add_Sp_B();

static inline void Push_Imm(qword imm);
static inline void Push_Fo();
static inline void Push_Bp();
static inline void Push_A();
static inline void Push_A();

static inline void Pop_Fo();
static inline void Pop_Sp();
static inline void Pop_A();
static inline void Pop_B();

static inline void Alloc(qword size); //FI SI
static inline void Dealloc(); //FI SI

/* Arithmetic Instructions */
static inline void Add_A_B(); //FI SI
static inline void Add_B_A(); //FI SI
static inline void Add_A_Imm(qword imm); //FI SI
static inline void Add_B_Imm(qword imm); //FI SI

static inline void Sub_A_B();
static inline void Sub_B_A();
static inline void Sub_A_Imm(qword imm);
static inline void Sub_B_Imm(qword imm);

static inline void Mul_A_B();
static inline void Mul_B_A();
static inline void Mul_A_Imm(qword imm);
static inline void Mul_B_Imm(qword imm);

static inline void Div_A_B();
static inline void Div_B_A();
static inline void Div_A_Imm(qword imm);
static inline void Div_B_Imm(qword imm);

static inline void Pow_A_B();
static inline void Pow_B_A();
static inline void Pow_A_Imm(qword imm);
static inline void Pow_B_Imm(qword imm);

static inline void Sl_A_Imm(byte imm); //FI SI
static inline void Sl_B_Imm(byte imm); //FI SI

static inline void Sr_A_Imm(byte imm); //FI SI
static inline void Sr_B_Imm(byte imm); //FI SI

static inline void Rl_A_Imm(byte imm);
static inline void Rl_B_Imm(byte imm);

static inline void Rr_A_Imm(byte imm);
static inline void Rr_B_Imm(byte imm);

static inline void Inv_A(); //FI
static inline void Inv_B(); //FI

static inline void And_A_B(); //FI SI
static inline void And_B_A(); //FI SI
static inline void And_A_Imm(qword imm); //FI SI
static inline void And_B_Imm(qword imm); //FI SI

static inline void Or_A_B();
static inline void Or_B_A();
static inline void Or_A_Imm(qword imm);
static inline void Or_B_Imm(qword imm);

static inline void Xor_A_B();
static inline void Xor_B_A();
static inline void Xor_A_Imm(qword imm);
static inline void Xor_B_Imm(qword imm);

static inline void Eq_A_B();
static inline void Eq_B_A();
static inline void Eq_A_Imm(qword imm);
static inline void Eq_B_Imm(qword imm);

static inline void Ne_A_B();
static inline void Ne_B_A();
static inline void Ne_A_Imm(qword imm);
static inline void Ne_B_Imm(qword imm);

static inline void Gt_A_B();
static inline void Gt_B_A();
static inline void Gt_A_Imm(qword imm);
static inline void Gt_B_Imm(qword imm);

static inline void Lt_A_B();
static inline void Lt_B_A();
static inline void Lt_A_Imm(qword imm);
static inline void Lt_B_Imm(qword imm);

static inline void Ge_A_B();
static inline void Ge_B_A();
static inline void Ge_A_Imm(qword imm);
static inline void Ge_B_Imm(qword imm);

static inline void Le_A_B();
static inline void Le_B_A();
static inline void Le_A_Imm(qword imm);
static inline void Le_B_Imm(qword imm);

/* Control Flow Instructions */
static inline void Jpa_Offset(qword off);
static inline void Jpb_Offset(qword off);
static inline void Jp_Offset(qword off);

static inline void Call_Offset(qword off);
static inline void Ret();

/* Misc */
static inline void HandleBuiltin(byte imm); //FI SI
#endif