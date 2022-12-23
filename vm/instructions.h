#ifndef INS_H
#define INS_H


#define list8bitImm(VAL) \
    VAL, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

enum Opcode {
    NIL,
    NOP,
          
    LDA_IMM,        /* A <- Immediate64 */
    LDB_IMM,        /* B <- Immediate64 */
    LDA_B,          /* A <- B */
    LDB_A,          /* B <- A */

    ENSURE_STACK_SIZE, /* Expand stack to be that size */
    SHRINK_STACK_SIZE, /* Shrink stack down to that size */
    INSERT_STACK_IMM_A, /* Stack[IMM] = A */
    LDA_STACK_IMM, /* A = Stack[IMM] */
    
    PUSH_IMM,        /* (SP) <- Immediate64; SP++ */
    PUSH_A,          /* (SP) <- A; SP++  */
    PUSH_B,          /* (SP) <- B; SP++  */
    POP_A,           /* SP--; A <- (SP)  */
    POP_B,           /* SP--; B <- (SP)  */
       
    AND_A_B,        /* A <- A & B */
    AND_B_A,        /* B <- A & B */
    AND_A_IMM,      /* A <- A & Immediate64 */
    AND_B_IMM,      /* B <- B & Immediate64 */

    SL_A_IMM,      /* A <- A << Immediate8 */
    SL_B_IMM,      /* B <- B << Immediate8 */
    SR_A_IMM,      /* A <- A >> Immediate8 */
    SR_B_IMM,      /* B <- A >> Immediate8 */

    INVA,           /* A <- !A */
    INVB,           /* B <- !B */

    LT_A_B,     /* A <- A < B */
    LT_B_A,     /* B <- B < A */
    LT_A_IMM,   /* A <- A < Immediate64 */
    LT_B_IMM,   /* B <- B < Immediate64 */
    
    GT_A_B,     /* A <- A > B */
    GT_B_A,     /* B <- B > A */
    GT_A_IMM,   /* A <- A > Immediate64 */
    GT_B_IMM,   /* B <- B > Immediate64 */

    LE_A_B,
    LE_B_A,
    LE_A_IMM,
    LE_B_IMM,

    GE_A_B,
    GE_B_A,
    GE_A_IMM,
    GE_B_IMM,

    EQ_A_B,
    EQ_B_A,
    EQ_A_IMM,
    EQ_B_IMM,

    NE_A_B,
    NE_B_A,
    NE_A_IMM,
    NE_B_IMM,

    OR_A_B,
    OR_B_A,
    OR_A_IMM,
    OR_B_IMM,

    XOR_A_B,
    XOR_B_A,
    XOR_A_IMM,
    XOR_B_IMM,

    ADD_A_B,        /* A <- A + B */
    ADD_B_A,        /* B <- B + A */
    ADD_A_IMM,      /* A <- A + Immediate64 */
    ADD_B_IMM,      /* B <- B + Immediate64 */

    SUB_A_B,
    SUB_B_A,
    SUB_A_IMM,
    SUB_B_IMM,

    MUL_A_B,
    MUL_B_A,
    MUL_A_IMM,
    MUL_B_IMM,

    POW_A_B,
    POW_B_A,
    POW_A_IMM,
    POW_B_IMM,

    DIV_A_B,
    DIV_B_A,
    DIV_A_IMM,
    DIV_B_IMM,


    JPA_OFF,       /* if (A != 0): PC <- CB + Offset64 */
    JPB_OFF,       /* if (B != 0): PC <- CB + Offset64 */
    JPB_Z_OFF,
    JPA_Z_OFF,
    JP_OFF,        /* PC <- CB + Offset64 */
    CALL_OFF,      /* push PC; PC <- CB + Offset64 */
    RET,           /* pop PC */

    BUILTIN,        /* Execute Builtin Call #Immediate8 */

    /* Unimplemented */
    LDRBS_IMM, //FI SI
    LDRBS_A, //FI SI
    LDRBS_B, //FI SI

    LDQBS_IMM, //FI SI
    LDQBS_A, //FI SI
    LDQBS_B, //FI SI

    LDM_BPAOFFIMM_IMM, //FI SI
    LDM_BPAOFFIMM_A, //FI SI
    LDM_BPAOFFIMM_B, //FI SI

    LDM_BPBOFFIMM_IMM, //FI SI
    LDM_BPBOFFIMM_A, //FI SI
    LDM_BPBOFFIMM_B, //FI SI

    LDM_BPBOFFA_IMM, //FI SI
    LDM_BPBOFFB_IMM, //FI SI

    LDM_BPAOFFA_B, //FI SI
    LDM_BPAOFFB_A, //FI SI

    LDM_BPAOFFA_IMM, //FI SI
    LDM_BPAOFFB_IMM, //FI SI

    LDM_BPBOFFA_B, //FI SI
    LDM_BPBOFFB_A, //FI SI

    LDA_BPAOFFIMM, //FI SI
    LDB_BPAOFFIMM, //FI SI

    LDA_BPBOFFIMM, //FI SI 
    LDB_BPBOFFIMM, //FI SI

    LDA_BPAOFFB, //FI SI
    LDB_BPAOFFA, //FI SI

    LDA_BPBOFFB, //FI SI
    LDB_BPBOFFA, //FI SI

    LDM_BPAOFFIMM_BPB, //FI SI
    LDM_BPAOFFA_BPB, //FI SI
    LDM_BPAOFFB_BPB, //FI SI

    LDM_BPBOFFIMM_BPA, //FI SI
    LDM_BPBOFFA_BPA, //FI SI
    LDM_BPBOFFB_BPA, //FI SI

    LDBPA_BPBOFFIMM, //FI SI
    LDBPA_BPBOFFA, //FI SI
    LDBPA_BPBOFFB, //FI SI

    LDBPB_BPAOFFIMM, //FI SI
    LDBPB_BPAOFFA, //FI SI
    LDBPB_BPAOFFB, //FI SI

    LDBSFO,
    PUSH_BSFO,
    POP_BSFO,

    ADD_BSP_IMM,
    ADD_BSP_A,
    ADD_BSP_B,

    SUB_BSP_IMM,
    SUB_BSP_A,
    SUB_BSP_B,

    LDBPA_BFOOFFIMM,
    LDBPA_BFONOFFIMM,
    LDBPA_BSPNOFFIMM,
    LDBPA_BFOOFFA,
    LDBPA_BFONOFFA,
    LDBPA_BSPNOFFA,
    LDBPA_BFOOFFB,
    LDBPA_BFONOFFB,
    LDBPA_BSPNOFFB,

    LDBPB_BFOOFFIMM,
    LDBPB_BFONOFFIMM,
    LDBPB_BSPNOFFIMM,
    LDBPB_BFOOFFA,
    LDBPB_BFONOFFA,
    LDBPB_BSPNOFFA,
    LDBPB_BFOOFFB,
    LDBPB_BFONOFFB,
    LDBPB_BSPNOFFB,

    PUSH_BPA, //FI SI
    PUSH_BPB, //FI SI
    
    POP_BPA, //FI SI
    POP_BPB, //FI SI

    ALLOC_BPA, //FI SI
    ALLOC_BPB, //FI SI

    RESIZE_BPA,
    RESIZE_BPB,

    DEALLOC_BPA, //FI SI
    DEALLOC_BPB, //FI SI

    UNION_BPA_BPB,
    UNION_BPB_BPA,

    APPENDBPA_IMM,
    APPENDBPA_A,
    APPENDBPA_B,

    APPENDBDB_IMM,
    APPENDBPB_A,
    APPENDBPB_B,

    /* ^^^ Unimplemented ^^^^ */

    EXIT,           /* Kill Process */
};

enum Builtins {
    PRINT_ASCII_A,
    PRINT_HEX_A,
    PRINT_ASCII_B,
    PRINT_HEX_B,
    DUMP_REGS,
};

#endif