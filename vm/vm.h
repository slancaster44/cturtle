#ifndef VM_H
#define VM_H

#include <stdint.h>

#include "common_types.h"

static qword REG_A;    /* General purpose register */
static qword REG_B;    /* General purpose register */
static byte* PC;       /* Pointer to current instruction*/
static qword FO;       /* Offset from base of stack of the current stack frame */

static int codelen;
static byte* CB; /* Starting address of code */

static int SP;
static int stack_limit; //In qwords
static qword* SB; /* Starting address of stack */

static bool ExitFlag;

void Execute(byte* code, int code_length);
void initVM(byte* code, int code_length);
int executeInstruction(); /* Returns 0 if no more to execute */

/* Debug Utility */
void printRegs();
void printStack();
byte* getPC();
byte* getCB();
qword getA();
qword getB();
qword peakBp(qword off);
void freeBP();
bool getExitFlag();

/* Expands stack if there is not enough space
 * to add a value of a given size 
 */
void ensureStackAccomodations(int numQwords);

/* Basic Load Instructions */
static inline void Lda_Imm(qword imm); //FI SI
static inline void Ldb_Imm(qword imm); //FI SI
static inline void Lda_B(); //FI SI
static inline void Ldb_A(); //FI SI

/* Memory Management Instructions */


/* Simple Stack Instructions */
static inline void Push_Imm(qword imm); //FI SI
static inline void Push_A(); //FI SI
static inline void Push_B(); //FI SI
static inline void Pop_A(); //FI SI
static inline void Pop_B(); //FI SI

/* Arithmetic Instructions */
static inline void Add_A_B(); //FI SI
static inline void Add_B_A(); //FI SI
static inline void Add_A_Imm(qword imm); //FI SI
static inline void Add_B_Imm(qword imm); //FI SI

static inline void Sub_A_B(); //FI SI
static inline void Sub_B_A(); //FI SI
static inline void Sub_A_Imm(qword imm); //FI SI
static inline void Sub_B_Imm(qword imm); //FI SI

static inline void Mul_A_B(); //FI SI
static inline void Mul_B_A(); //FI SI
static inline void Mul_A_Imm(qword imm); //FI SI
static inline void Mul_B_Imm(qword imm); //FI SI

static inline void Div_A_B(); //FI SI
static inline void Div_B_A(); //FI SI
static inline void Div_A_Imm(qword imm); //FI SI
static inline void Div_B_Imm(qword imm); //FI SI

static inline void Pow_A_B(); //FI SI
static inline void Pow_B_A(); //FI SI
static inline void Pow_A_Imm(qword imm); //FI SI
static inline void Pow_B_Imm(qword imm); //FI SI

static inline void Sl_A_Imm(byte imm); //FI SI
static inline void Sl_B_Imm(byte imm); //FI SI

static inline void Sr_A_Imm(byte imm); //FI SI
static inline void Sr_B_Imm(byte imm); //FI SI

static inline void Inv_A(); //FI SI
static inline void Inv_B(); //FI SI

static inline void And_A_B(); //FI SI
static inline void And_B_A(); //FI SI
static inline void And_A_Imm(qword imm); //FI SI
static inline void And_B_Imm(qword imm); //FI SI

static inline void Or_A_B(); //FI SI
static inline void Or_B_A(); //FI SI
static inline void Or_A_Imm(qword imm); //FI SI
static inline void Or_B_Imm(qword imm); //FI SI

static inline void Xor_A_B(); //FI SI
static inline void Xor_B_A(); //FI SI
static inline void Xor_A_Imm(qword imm); //FI SI 
static inline void Xor_B_Imm(qword imm); //FI SI

static inline void Eq_A_B(); //FI SI 
static inline void Eq_B_A(); //FI SI 
static inline void Eq_A_Imm(qword imm); //FI SI 
static inline void Eq_B_Imm(qword imm); //FI SI 

static inline void Ne_A_B(); //FI SI 
static inline void Ne_B_A(); //FI SI 
static inline void Ne_A_Imm(qword imm); //FI SI 
static inline void Ne_B_Imm(qword imm); //FI SI 

static inline void Gt_A_B(); //FI SI 
static inline void Gt_B_A(); //FI SI 
static inline void Gt_A_Imm(qword imm); //FI SI 
static inline void Gt_B_Imm(qword imm); //FI SI 

static inline void Lt_A_B(); //FI SI 
static inline void Lt_B_A(); //FI SI 
static inline void Lt_A_Imm(qword imm); //FI SI 
static inline void Lt_B_Imm(qword imm); //FI SI 

static inline void Ge_A_B(); //FI SI 
static inline void Ge_B_A(); //FI SI 
static inline void Ge_A_Imm(qword imm); //FI SI 
static inline void Ge_B_Imm(qword imm); //FI SI 

static inline void Le_A_B(); //FI SI 
static inline void Le_B_A(); //FI SI 
static inline void Le_A_Imm(qword imm); //FI SI 
static inline void Le_B_Imm(qword imm); //FI SI 

/* Control Flow Instructions */
static inline void Jpa_Offset(qword off); //FI SI
static inline void Jpb_Offset(qword off); //FI SI
static inline void Jp_Offset(qword off); //FI SI
static inline void Jpb_Z_Offset(qword off);
static inline void Jpa_Z_Offset(qword off);

static inline void Call_Offset(qword off); //FI SI
static inline void Ret(); //FI SI

static inline void Insert_Stack_Imm_A(qword imm);
static inline void Ensure_Stack_Size(qword imm);
static inline void Lda_Stack_Imm(qword imm);

static inline void Insert_Buffer_Stack_Imm_BPA(qword imm);
static inline void Ensure_Buffer_Stack_Size(qword imm);
static inline void Ldbpa_Stack_Imm(qword imm);



/* Misc */
static inline void HandleBuiltin(byte imm); //FI SI
#endif