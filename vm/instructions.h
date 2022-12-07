#ifndef INS_H
#define INS_H

enum Opcode {
    NIL,
    NOP,
          
    LDA_IMM,        /* A <- Immediate64 */
    LDB_IMM,        /* B <- Immediate64 */
    LDA_B,          /* A <- B */
    LDB_A,          /* B <- A */

    LDA_SP_NOFF,     /* A <- (SP - Offset64) */
    LDA_FO_OFF,      /* A <- (SB + FO + Offset64) */
    LDA_FO_NOFF,     /* A <- (SB + FO - Offset64) */

    LDB_SP_NOFF,     /* B <- (SP - Offset64) */
    LDB_FO_OFF,      /* B <- (SB + FO + Offset64) */
    LDB_FO_NOFF,     /* B <- (SB + FO - Offset64) */

    LDM_SPNOFF_A,   /* (SP - Offset64) <- A */
    LDM_FOOFF_A,    /* (SP + FO + Offset64) <- A */
    LDM_FONOFF_A,   /* (SP - FO + Offset64) <- A */

    LDM_SPNOFF_B,   /* (SP - Offset64) <- B */
    LDM_FOOFF_B,    /* (SP + FO + Offset64) <- B */
    LDM_FONOFF_B,   /* (SP - FO + Offset64) <- B */

    LDFO,            /* FO <- SP - SB */

    SUB_SP_IMM,      /* SP <- SP - Immediate64 */
    SUB_SP_B,        /* SP <- SP - B */
    SUB_SP_A,        /* SP <- SP - A */

    ADD_SP_IMM,     /* SP <- SP + Immediate64 */
    ADD_SP_B,       /* SP <- SP + B */
    ADD_SP_A,       /* SP <- SP + A */
    
    PUSH_FO,         /* (SP) <- FO; SP++ */
    PUSH_IMM,        /* (SP) <- Immediate64; SP++ */
    PUSH_A,          /* (SP) <- A; SP++  */
    PUSH_B,          /* (SP) <- B; SP++  */
    POP_A,           /* SP--; A <- (SP)  */
    POP_B,           /* SP--; B <- (SP)  */
    POP_FO,          /* SP--; FO <- (SP) */
    
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
    JPA_NZ_OFF,
    JPB_NZ_OFF,
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

    LDM_BPAOFFA_B, //FI 
    LDM_BPAOFFB_A, //FI

    LDM_BPAOFFA_IMM, //FI SI
    LDM_BPAOFFB_IMM, //FI SI

    LDM_BPBOFFA_B, //FI
    LDM_BPBOFFB_A, //FI

    LDA_BPAOFFIMM, //FI SI
    LDB_BPAOFFIMM, //FI SI

    LDA_BPBOFFIMM, //FI SI 
    LDB_BPBOFFIMM, //FI SI

    LDA_BPAOFFB, //FI SI
    LDB_BPAOFFA, //FI

    LDA_BPBOFFB, //FI SI
    LDB_BPBOFFA, //FI SI

    LDM_BPAOFFIMM_BPA, //FI
    LDM_BPAOFFA_BPA, //FI
    LDM_BPAOFFB_BPA, //FI

    LDM_BPAOFFIMM_BPB, //FI SI
    LDM_BPAOFFA_BPB, //FI SI
    LDM_BPAOFFB_BPB, //FI SI

    LDM_BPBOFFIMM_BPA, //FI SI
    LDM_BPBOFFA_BPA, //FI SI
    LDM_BPBOFFB_BPA, //FI SI

    LDM_BPBOFFIMM_BPB, //FI 
    LDM_BPBOFFA_BPB, //FI
    LDM_BPBOFFB_BPB, //FI

    LDBPA_BPAOFFIMM, //FI
    LDBPA_BPAOFFA, //FI
    LDBPA_BPAOFFB, //FI

    LDBPA_BPBOFFIMM, //FI SI
    LDBPA_BPBOFFA, //FI SI
    LDBPA_BPBOFFB, //FI SI

    LDBPB_BPAOFFIMM, //FI SI
    LDBPB_BPAOFFA, //FI SI
    LDBPB_BPAOFFB, //FI SI

    LDBPB_BPBOFFIMM, //FI
    LDBPB_BPBOFFA, //FI
    LDBPB_BPBOFFB, //FI

    LDBSFO,
    PUSH_BSFO,
    POP_BSFO,

    ADD_BSP_IMM,
    ADD_BSP_A,
    ADD_BSP_B,

    SUB_BSP_IMM,
    SUB_BSP_A,
    SUB_BSP_B,

    LDBPA_BFOOFF_IMM,
    LDBPA_BFONOFF_IMM,
    LDBPA_BSPNOFF_IMM,
    LDBPA_BFOOFF_A,
    LDBPA_BFONOFF_A,
    LDBPA_BSPNOFF_A,
    LDBPA_BFOOFF_B,
    LDBPA_BFONOFF_B,
    LDBPA_BSPNOFF_B,

    LDBPB_BFOOFF_IMM,
    LDBPB_BFONOFF_IMM,
    LDBPB_BSPNOFF_IMM,
    LDBPB_BFOOFF_A,
    LDBPB_BFONOFF_A,
    LDBPB_BSPNOFF_A,
    LDBPB_BFOOFF_B,
    LDBPB_BFONOFF_B,
    LDBPB_BSPNOFF_B,

    PUSH_BPA,
    PUSH_BPB,
    
    POP_BPA,
    POP_BPB,

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