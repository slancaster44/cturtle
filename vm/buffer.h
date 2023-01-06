#ifndef BUFFER_H
#define BUFFER_H

/* To facilitate garbage collection in the vm
 * there has to be a tight speration of where
 * dynamically allocated memories are stored
 * and where other variables are stored
 * 
 * This header includes the necessary
 * infrastructure to do so.
 * 
 * The Buffer stack will store the root
 * objects. Any buffers kept during garbage
 * collection must either be on the buffer
 * stack, or refered to by buffers on this
 * stack. All other buffers will be deleted
 */

#include <inttypes.h>
#include "common_types.h"

struct Buffer {
    int numQwords;
    qword* QwordBuffer;

    int numReferences;
    struct Buffer** References;
};

/* The user will place buffers on this
 * stack
 */
static int bufferStackLimit;
static struct Buffer** BufferStack; 
static qword BSP; /* Buffer Stack Pointer */

/* All buffers will be added to this list
 * when an alloc instruction is executed
 * When garbage collection occurs, any
 * buffers in this list, but not on
 * the buffer stack will be deleted
 */
static int numAllocatedBuffers;
static struct Buffer* AllBuffers;

/* So the user can manipulate stack frames
 * easily, the frame offset register 
 * will point to the current frame
 */

static qword BSFO;

/* The user may manipulate buffers
 * placed in these buffer pointer
 * registers
 */

static struct Buffer* BPA;
static struct Buffer* BPB;

/* When a user resizes or allocates
 * a buffer, the user must indicate
 * the sizes. These Buffer size registers
 * will be used to indicate that
 * 
 * RBS -- Will be used to indicate
 * the refrence buffer size
 * 
 * QBS -- Will be used to indicate
 * the qword buffer size
 */

static qword RBS;
static qword QBS;

/* Debug */
qword getQBS();
qword getRBS();
struct Buffer* getBPA();
struct Buffer* getBPB();
void freeBPA();
void freeBPB();
void printBuffer(struct Buffer* b);

/* Buffer Size Instructions */

static inline void Ldrbs_Imm(qword imm); //RBS <- Imm64
static inline void Ldrbs_A();  //RBS <- A
static inline void Ldrbs_B();  //RBS <- B

static inline void Ldqbs_Imm(qword imm); //QBS <- Imm64
static inline void Ldqbs_A(); // QBS <- A
static inline void Ldqbs_B(); //QBS <- B

/* Buffer Manipulation Instructions */
static inline void Ldm_BpaOffImm_Imm(qword offset, qword imm); //BPA.QwordBuffer[Offset] <- Imm64
static inline void Ldm_BpaOffImm_A(qword offset); //BPA.QwordBuffer[Offset] <- A
static inline void Ldm_BpaOffImm_B(qword offset); //BPA.QwordBuffer[Offset] <- B

static inline void Ldm_BpbOffImm_Imm(qword offset, qword imm); //BPB.QwordBuffer[Offset] <- Imm64
static inline void Ldm_BpbOffImm_A(qword offset); //BPB.QwordBuffer[Offset] <- A
static inline void Ldm_BpbOffImm_B(qword offset); //BPB.QwordBuffer[Offset] <- B

static inline void Ldm_BpaOffA_B();
static inline void Ldm_BpaOffB_A();

static inline void Ldm_BpbOffA_B();
static inline void Ldm_BpbOffB_A();

static inline void Ldm_BpaOffA_Imm(qword imm);
static inline void Ldm_BpaOffB_Imm(qword imm);

static inline void Ldm_BpbOffA_Imm(qword imm);
static inline void Ldm_BpbOffB_Imm(qword imm);

static inline void Lda_BpaOffImm(qword offset); //A <- BPA.QwordBuffer[Offset]
static inline void Ldb_BpaOffImm(qword offset); //B <- BPA.QwordBuffer[Offset]

static inline void Lda_BpbOffImm(qword offset); //A <- BPB.QwordBuffer[Offset]
static inline void Ldb_BpbOffImm(qword offset); //B <- BPB.QwordBuffer[Offset]

static inline void Lda_BpaOffB(); //A <- BPA.QwordBuffer[Offset]
static inline void Ldb_BpaOffA(); //B <- BPA.QwordBuffer[Offset]

static inline void Lda_BpbOffB(); //A <- BPB.QwordBuffer[Offset]
static inline void Ldb_BpbOffA(); //B <- BPB.QwordBuffer[Offset]


static inline void Ldm_BpaOffImm_Bpa(qword offset); //BPA.References[Offset] <- BPA
static inline void Ldm_BpaOffImm_Bpb(qword offset); //BPA.References[Offset] <- BPB

static inline void Ldm_BpbOffImm_Bpa(qword offset); //BPA.References[Offset] <- BPA
static inline void Ldm_BpbOffImm_Bpb(qword offset); //BPA.References[Offset] <- BPB

static inline void Ldbpb_BpaOffImm(qword offset);
static inline void Ldbpa_BpbOffImm(qword offset);

static inline void Ldm_BpaOffA_Bpb();
static inline void Ldm_BpaOffB_Bpb();
static inline void Ldm_BpbOffA_Bpa();
static inline void Ldm_BpbOffB_Bpa();

static inline void Ldbpb_BpaOffA();
static inline void Ldbpb_BpaOffB();
static inline void Ldbpa_BpbOffA();
static inline void Ldbpa_BpbOffB();

static inline void Ldbpa_BpbOff(qword offset); //BPA <- BPB.References[Offset]

static inline void Ldbpb_BpaOff(qword offset); //BPB <- BPA.References[Offset]
/* Buffer Stack Instructions */
static inline void Ldbfo(); //BSFO <- BSP

static inline void Add_Bsp_Imm(qword imm); //BSP <- BSP + Imm64
static inline void Add_Bsp_A(); // BSP <- BSP + A
static inline void Add_Bsp_B(); // BSP <- BSP + B

static inline void Sub_Bsp_Imm(qword imm); //BSP <- BSP - Imm64
static inline void Sub_Bsp_A(); //BSP <- BSP - A
static inline void Sub_Bsp_B(); //BSP <- BSP - B

static inline void Ldbpa_BfoOff_Imm(qword offset);
static inline void Ldbpa_BfoNoff_Imm(qword offset);
static inline void Ldbpa_BspNoff_Imm(qword offset);
static inline void Ldbpa_BfoOff_A(qword offset);
static inline void Ldbpa_BfoNoff_A(qword offset);
static inline void Ldbpa_BspNoff_A(qword offset);
static inline void Ldbpa_BfoOff_B(qword offset);
static inline void Ldbpa_BfoNoff_B(qword offset);
static inline void Ldbpa_BspNoff_B(qword offset);

static inline void Ldbpb_BfoOff_Imm(qword offset);
static inline void Ldbpb_BfoNoff_Imm(qword offset);
static inline void Ldbpb_BspNoff_Imm(qword offset);
static inline void Ldbpb_BfoOff_A(qword offset);
static inline void Ldbpb_BfoNoff_A(qword offset);
static inline void Ldbpb_BspNoff_A(qword offset);
static inline void Ldbpb_BfoOff_B(qword offset);
static inline void Ldbpb_BfoNoff_B(qword offset);
static inline void Ldbpb_BspNoff_B(qword offset);

static inline void Push_Bpa();
static inline void Push_Bpb();

static inline void Pop_Bpa();
static inline void Pop_Bpb();

/* Allocation Instructons */
static inline void Alloc_Bpa();
static inline void Alloc_Bpb();

static inline void Resize_Bpa();
static inline void Resize_Bpb();

/* Deallocation Instructions */
static inline void Dealloc_Bpa();
static inline void Dealloc_Bpb();

/* Set Math Instructions */
static inline void Union_Bpa_Bpb(); //BPA <- BPA U BPB
static inline void Union_Bpb_Bpa(); //BPB <- BPB U BPA

static inline void AppendBpa_Imm(qword imm); //BPA <- BPA + Imm
static inline void AppendBpa_A(); //BPA <- BPA + A
static inline void AppendBpa_B(); //BPA <- BPA + B

static inline void AppendBpb_Imm(qword imm); //BPB <- BPB + Imm
static inline void AppendBpb_A(); //BPB <- BPB + A
static inline void AppendBpb_B(); //BPB <- BPB + B

#endif
