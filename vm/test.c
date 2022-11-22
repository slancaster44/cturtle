#include "vm.h"
#include "instructions.h"

#include <stdio.h>

#define RA 0
#define RB 1

void assert(int expr, char* name) {
    if (expr)
        printf("%s Passed\n", name);
    else {
        printRegs(); 
        printf("%s Failed\n", name);
    }
}


#define list8bitImm(VAL) \
    VAL, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

void testMaths() {
    byte codeSUB_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x02),
        LDB_IMM,
        list8bitImm(0x01),
        SUB_A_B,
        EXIT,
    };
    Execute(codeSUB_A_B, 20);
    assert(getA() == 0x01, "SUB_A_B");


    byte codeSUB_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x02),
        SUB_B_A,
        EXIT,
    };
    Execute(codeSUB_B_A, 20);
    assert(getB() == 0x01, "SUB_B_A");


    byte codeSUB_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x02),
        SUB_B_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeSUB_B_IMM, 20);
    assert(getB() == 0x01, "SUB_B_IMM");


    byte codeSUB_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x02),
        SUB_A_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeSUB_A_IMM, 20);
    assert(getA() == 0x01, "SUB_A_IMM");


    byte codeDIV_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x04),
        LDB_IMM,
        list8bitImm(0x02),
        DIV_A_B,
        EXIT,
    };
    Execute(codeDIV_A_B, 20);
    assert(getA() == 0x02, "DIV_A_B");


    byte codeDIV_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x02),
        LDB_IMM,
        list8bitImm(0x04),
        DIV_B_A,
        EXIT,
    };
    Execute(codeDIV_B_A, 20);
    assert(getB() == 0x02, "DIV_B_A");


    byte codeDIV_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x04),
        DIV_B_IMM,
        list8bitImm(0x02),
        EXIT,
    };
    Execute(codeDIV_B_IMM, 20);
    assert(getB() == 0x02, "DIV_B_IMM");


    byte codeDIV_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x04),
        DIV_A_IMM,
        list8bitImm(0x02),
        EXIT,
    };
    Execute(codeDIV_A_IMM, 20);
    assert(getA() == 0x02, "DIV_A_IMM");


    byte codeMUL_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x04),
        LDB_IMM,
        list8bitImm(0x02),
        MUL_A_B,
        EXIT,
    };
    Execute(codeMUL_A_B, 20);
    assert(getA() == 0x08, "MUL_A_B");


    byte codeMUL_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x02),
        LDB_IMM,
        list8bitImm(0x04),
        MUL_B_A,
        EXIT,
    };
    Execute(codeMUL_B_A, 20);
    assert(getB() == 0x08, "MUL_B_A");


    byte codeMUL_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x04),
        MUL_B_IMM,
        list8bitImm(0x02),
        EXIT,
    };
    Execute(codeMUL_B_IMM, 20);
    assert(getB() == 0x08, "MUL_B_IMM");


    byte codeMUL_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x04),
        MUL_A_IMM,
        list8bitImm(0x02),
        EXIT,
    };
    Execute(codeMUL_A_IMM, 20);
    assert(getA() == 0x08, "MUL_A_IMM");


    byte codeADD_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x04),
        LDB_IMM,
        list8bitImm(0x02),
        ADD_A_B,
        EXIT,
    };
    Execute(codeADD_A_B, 20);
    assert(getA() == 0x06, "ADD_A_B");


    byte codeADD_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x02),
        LDB_IMM,
        list8bitImm(0x04),
        ADD_B_A,
        EXIT,
    };
    Execute(codeADD_B_A, 20);
    assert(getB() == 0x06, "ADD_B_A");


    byte codeADD_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x04),
        ADD_B_IMM,
        list8bitImm(0x02),
        EXIT,
    };
    Execute(codeADD_B_IMM, 20);
    assert(getB() == 0x06, "ADD_B_IMM");


    byte codeADD_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x04),
        ADD_A_IMM,
        list8bitImm(0x02),
        EXIT,
    };
    Execute(codeADD_A_IMM, 20);
    assert(getA() == 0x06, "ADD_A_IMM");


    byte codePOW_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x04),
        LDB_IMM,
        list8bitImm(0x02),
        POW_A_B,
        EXIT,
    };
    Execute(codePOW_A_B, 20);
    assert(getA() == 0x10, "POW_A_B");


    byte codePOW_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x02),
        LDB_IMM,
        list8bitImm(0x04),
        POW_B_A,
        EXIT,
    };
    Execute(codePOW_B_A, 20);
    assert(getB() == 0x10, "POW_B_A");


    byte codePOW_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x04),
        POW_B_IMM,
        list8bitImm(0x02),
        EXIT,
    };
    Execute(codePOW_B_IMM, 20);
    assert(getB() == 0x10, "POW_B_IMM");


    byte codePOW_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x04),
        POW_A_IMM,
        list8bitImm(0x02),
        EXIT,
    };
    Execute(codePOW_A_IMM, 20);
    assert(getA() == 0x10, "POW_A_IMM");


    byte codeXOR_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        XOR_A_B,
        EXIT,
    };
    Execute(codeXOR_A_B, 20);
    assert(getA() == 0x01, "XOR_A_B");


    byte codeXOR_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        XOR_B_A,
        EXIT,
    };
    Execute(codeXOR_B_A, 20);
    assert(getB() == 0x01, "XOR_B_A");


    byte codeXOR_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x01),
        XOR_B_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeXOR_B_IMM, 20);
    assert(getB() == 0x00, "XOR_B_IMM");


    byte codeXOR_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        XOR_A_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeXOR_A_IMM, 20);
    assert(getA() == 0x00, "XOR_A_IMM");


    byte codeAND_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        AND_A_B,
        EXIT,
    };
    Execute(codeAND_A_B, 20);
    assert(getA() == 0x00, "AND_A_B");


    byte codeAND_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        AND_B_A,
        EXIT,
    };
    Execute(codeAND_B_A, 20);
    assert(getB() == 0x00, "AND_B_A");


    byte codeAND_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x01),
        AND_B_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeAND_B_IMM, 20);
    assert(getB() == 0x01, "AND_B_IMM");


    byte codeAND_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        AND_A_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeAND_A_IMM, 20);
    assert(getA() == 0x01, "AND_A_IMM");


    byte codeOR_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        OR_A_B,
        EXIT,
    };
    Execute(codeOR_A_B, 20);
    assert(getA() == 0x01, "OR_A_B");


    byte codeOR_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        OR_B_A,
        EXIT,
    };
    Execute(codeOR_B_A, 20);
    assert(getB() == 0x01, "OR_B_A");


    byte codeOR_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x01),
        OR_B_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeOR_B_IMM, 20);
    assert(getB() == 0x01, "OR_B_IMM");


    byte codeOR_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        OR_A_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeOR_A_IMM, 20);
    assert(getA() == 0x01, "OR_A_IMM");


    byte codeLT_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        LT_A_B,
        EXIT,
    };
    Execute(codeLT_A_B, 20);
    assert(getA() == 0x00, "LT_A_B");


    byte codeLT_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        LT_B_A,
        EXIT,
    };
    Execute(codeLT_B_A, 20);
    assert(getB() == 0x01, "LT_B_A");


    byte codeLT_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x01),
        LT_B_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeLT_B_IMM, 20);
    assert(getB() == 0x00, "LT_B_IMM");


    byte codeLT_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LT_A_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeLT_A_IMM, 20);
    assert(getA() == 0x00, "LT_A_IMM");


    byte codeLE_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        LE_A_B,
        EXIT,
    };
    Execute(codeLE_A_B, 20);
    assert(getA() == 0x00, "LE_A_B");


    byte codeLE_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        LE_B_A,
        EXIT,
    };
    Execute(codeLE_B_A, 20);
    assert(getB() == 0x01, "LE_B_A");


    byte codeLE_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x01),
        LE_B_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeLE_B_IMM, 20);
    assert(getB() == 0x00, "LE_B_IMM");


    byte codeLE_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LE_A_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeLE_A_IMM, 20);
    assert(getA() == 0x00, "LE_A_IMM");


    byte codeGT_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        GT_A_B,
        EXIT,
    };
    Execute(codeGT_A_B, 20);
    assert(getA() == 0x01, "GT_A_B");


    byte codeGT_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        GT_B_A,
        EXIT,
    };
    Execute(codeGT_B_A, 20);
    assert(getB() == 0x00, "GT_B_A");


    byte codeGT_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x01),
        GT_B_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeGT_B_IMM, 20);
    assert(getB() == 0x00, "GT_B_IMM");


    byte codeGT_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        GT_A_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeGT_A_IMM, 20);
    assert(getA() == 0x00, "GT_A_IMM");


    byte codeGE_A_B[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        GE_A_B,
        EXIT,
    };
    Execute(codeGE_A_B, 20);
    assert(getA() == 0x01, "GE_A_B");


    byte codeGE_B_A[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        LDB_IMM,
        list8bitImm(0x00),
        GE_B_A,
        EXIT,
    };
    Execute(codeGE_B_A, 20);
    assert(getB() == 0x00, "GE_B_A");


    byte codeGE_B_IMM[20] = {
        LDB_IMM,
        list8bitImm(0x01),
        GE_B_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeGE_B_IMM, 20);
    assert(getB() == 0x01, "GE_B_IMM");


    byte codeGE_A_IMM[20] = {
        LDA_IMM,
        list8bitImm(0x01),
        GE_A_IMM,
        list8bitImm(0x01),
        EXIT,
    };
    Execute(codeGE_A_IMM, 20);
    assert(getA() == 0x01, "GE_A_IMM");


}

void testBufferPointer() {
    byte code[28] = {
        ALLOC_IMM,
        list8bitImm(0x0F),
        LDA_IMM,
        list8bitImm(0xFF),
        LDM_BPOFF_A,
        list8bitImm(0x0E),
        EXIT,
    };

    Execute(code, 28);
    assert(peakBp(0x0E) == 0xFF, "LDM_BPOFF_A");
    freeBP();

    byte code1[28] = {
        ALLOC_IMM,
        list8bitImm(0x0F),
        LDB_IMM,
        list8bitImm(0xFE),
        LDM_BPOFF_B,
        list8bitImm(0x0C),
        EXIT,
    };
    Execute(code1, 28);
    assert(peakBp(0x0C) == 0xFE, "LDM_BPOFF_B");
    freeBP();

    byte code2[37] = {
        ALLOC_IMM,
        list8bitImm(0x0F),
        LDB_IMM,
        list8bitImm(0xFE),
        LDM_BPOFF_B,
        list8bitImm(0x0C),
        LDA_BP_OFF,
        list8bitImm(0x0C),
        EXIT,
    };
    Execute(code2, 37);
    assert(getA() == 0xFE, "LDA_BPOFF");
    freeBP();

    byte code3[37] = {
        ALLOC_IMM,
        list8bitImm(0x0F),
        LDA_IMM,
        list8bitImm(0xFF),
        LDM_BPOFF_A,
        list8bitImm(0x0E),
        LDB_BP_OFF,
        list8bitImm(0x0E),
        EXIT,
    };
    Execute(code3, 37);
    assert(getB() == 0xFF, "LDB_BPOFF");
    freeBP();

    byte code4[38] = {
        LDA_IMM,
        list8bitImm(0x0A),
        ALLOC_IMM,
        list8bitImm(0x0F),
        LDM_BPOFF_A,
        list8bitImm(0x0D),
        LDA_IMM,
        list8bitImm(0x0D),
        LDB_BPOFF_A,
        EXIT,
    };
    Execute(code4, 38);
    assert(getB() == 0x0A, "LDB_BPOFF_A");
    freeBP();

    byte code5[38] = {
        LDB_IMM,
        list8bitImm(0x0C),
        ALLOC_IMM,
        list8bitImm(0x0F),
        LDM_BPOFF_B,
        list8bitImm(0x0D),
        LDB_IMM,
        list8bitImm(0x0D),
        LDA_BPOFF_B,
        EXIT,
    };
    Execute(code5, 38);
    assert(getA() == 0x0C, "LDA_BPOFF_B");
    freeBP();

    byte code6[29] = {
        ALLOC_IMM,          /* Allocate buffer */
        list8bitImm(0x0F),
        LDB_IMM,            /* Load index into b */
        list8bitImm(0x0B),
        LDA_IMM,            /* Load value into a */
        list8bitImm(0x0A),
        LDM_BPOFFB_A, /* Move value into memory*/
        EXIT,
    };
    Execute(code6, 29);
    assert(peakBp(0x0B) == 0x0A, "LDM_BPOFFB_A");
    freeBP();


    byte code7[29] = {
        ALLOC_IMM,          /* Allocate buffer */
        list8bitImm(0x0F),
        LDA_IMM,            /* Load index into a */
        list8bitImm(0x0A),
        LDB_IMM,            /* Load value into b */
        list8bitImm(0x0B),
        LDM_BPOFFA_B, /* Move value into memory*/
        EXIT,
    };
    Execute(code7, 29);
    assert(peakBp(0x0A) == 0x0B, "LDM_BPOFFB_A");
    freeBP();
}

int main() {
    byte code[28] = {
        LDA_IMM,
        0xFE,
        0xFF,
        0xFF,
        0x10,
        0x00,
        0x00,
        0x00,
        0x00,
        SUB_A_IMM,
        list8bitImm(0x0F),
        JPA_NZ_OFF,
        list8bitImm(0x09),
        EXIT,
    };

    Execute(code, 28);
    return 0;
}
