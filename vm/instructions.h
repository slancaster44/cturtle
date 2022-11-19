#ifndef INS_H
#define INS_H

enum Opcode {
    NIL,
    NOP,
          
    LDA_IMM,        /* A <- Immediate64 */
    LDB_IMM,        /* B <- Immediate64 */
    LDA_B,          /* A <- B */
    LDB_A,          /* B <- A */

    /* ==== Unimplemented ==== */
    LDA_BP_OFF,      /* A <- (BP + Offset64) */
    LDA_SP_OFF ,     /* A <- (SP + Offset64) */
    LDA_SP_NOFF,     /* A <- (SP - Offset64) */
    LDA_FO_OFF,      /* A <- (SB + FO + Offset64) */
    LDA_FO_NOFF,     /* A <- (SB + FO - Offset64) */

    LDB_BP_OFF,      /* B <- (BP + Offset64) */
    LDB_SP_OFF,      /* B <- (SP + Offset64) */
    LDB_SP_NOFF,     /* B <- (SP - Offset64) */
    LDB_FO_OFF,      /* B <- (SB + FO + Offset64) */
    LDB_FO_NOFF,     /* B <- (SB + FO - Offset64) */

    LD_BP_OFF_A,    /* (BP + Offset64) <- A */
    LD_SP_OFF_A,    /* (SP + Offset64) <- A */
    LD_SP_NOFF_A,   /* (SP - Offset64) <- A */
    LD_FO_OFF_A,    /* (SP + FO + Offset64) <- A */
    LD_FO_NOFF_A,   /* (SP - FO + Offset64) <- A */

    LD_BP_OFF_B,    /* (BP + Offset64) <- B */
    LD_SP_OFF_B,    /* (SP + Offset64) <- B */
    LD_SP_NOFF_B,   /* (SP - Offset64) <- B */
    LD_FO_OFF_B,    /* (SP + FO + Offset64) <- B */
    LD_FO_NOFF_B,   /* (SP - FO + Offset64) <- B */

    LDBP_FO_OFF,     /* BP <- (SB + FO + Offset64) */
    LDBP_FO_NOFF,    /* BP <- (SB + FO - Offset64) */
    LDBP_SP_OFF ,    /* BP <- (SP + Offset64) */
    LDBP_SP_NOFF,    /* BP <- (SP - Offset64) */

    LDFO,            /* FO <- SP - SB */

    SUB_SP_IMM,      /* SP <- SP - Immediate64 */
    SUB_SP_B,        /* SP <- SP - B */
    SUB_SP_A,        /* SP <- SP - A */
    
    PUSH_FO,         /* (SP) <- FO; SP++ */
    PUSH_IMM,        /* (SP) <- Immediate64; SP++ */
    PUSH_A,          /* (SP) <- A; SP++  */
    PUSH_B,          /* (SP) <- B; SP++  */
    PUSH_BP,         /* (SP) <- BP; SP++ */
    POP_A,           /* SP--; A <- (SP)  */
    POP_B,           /* SP--; B <- (SP)  */
    POP_BP,          /* SP--; BP <- (SP) */
    POP_FO,          /* SP--; FO <- (SP) */
    /* ^^^^^ Unimplemented ^^^^^ */
    
    AND_A_B,        /* A <- A & B */
    AND_B_A,        /* B <- A & B */
    AND_A_IMM,      /* A <- A & Immediate64 */
    AND_B_IMM,      /* B <- B & Immediate64 */

    SL_A_IMM,      /* A <- A << Immediate8 */
    SL_B_IMM,      /* B <- B << Immediate8 */
    SR_A_IMM,      /* A <- A >> Immediate8 */
    SR_B_IMM,      /* B <- A >> Immediate8 */
    RL_A_IMM,
    RL_B_IMM,
    RR_A_IMM,
    RR_B_IMM,

    INVA,           /* A <- !A */
    INVB,           /* B <- !B */

    LT_A_B,
    LT_B_A,
    LT_A_IMM,
    LT_B_IMM,
    
    GT_A_B,
    GT_B_A,
    GT_A_IMM,
    GT_B_IMM,

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

    SQR_A_B,
    SQR_B_A,
    SQR_A_IMM,
    SQR_B_IMM,

    DIV_A_B,
    DIV_B_A,
    DIV_A_IMM,
    DIV_B_IMM,


    JPA_ADDR,       /* if (A != 0): PC <- Address64 */
    JPB_ADDR,       /* if (B != 0): PC <- Address64 */
    JP_ADDR,        /* PC <- Address64 */
    CALL_ADDR,      /* push PC; PC <- Address64 */
    RET,            /* pop PC */

    BUILTIN,        /* Execute Builtin Call #Immediate8 */

    ALLOC_IMM,      /* BP <- malloc(sizeof(qword) * immediate64)*/
    ALLOC_A,        /* BP <- malloc(sizeof(qword) * a) */
    ALLOC_B,        /* BP <- malloc(sizeof(qword) * b) */
    DEALLOC,        /* free(BP) */
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