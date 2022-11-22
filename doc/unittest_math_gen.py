#!/usr/bin/python3

def Mk_Arithmetic_OP_A_B_Test(
    initalAValue,
    initalBValue,
    Operation,
    ExpectedResult_A
):
    return """
        byte code{2}[20] = {{
            LDA_IMM,
            list8bitImm({0}),
            LDB_IMM,
            list8bitImm({1}),
            {2},
            EXIT,
        }};
        Execute(code{2}, 20);
        assert(getA() == {3}, "{2}");
    """.format(initalAValue, 
        initalBValue,
        Operation,
        ExpectedResult_A)

def Mk_Arithmetic_OP_B_A_Test(
    initalAValue,
    initalBValue,
    Operation,
    ExpectedResult_A
):
    return """
        byte code{2}[20] = {{
            LDA_IMM,
            list8bitImm({0}),
            LDB_IMM,
            list8bitImm({1}),
            {2},
            EXIT,
        }};
        Execute(code{2}, 20);
        assert(getB() == {3}, "{2}");
    """.format(initalAValue, 
        initalBValue,
        Operation,
        ExpectedResult_A)

def Mk_Arithmetic_OP_B_IMM_Test(
    initalBValue,
    Operation,
    Op_Param,
    ExpectedResult_B
):
    return """
        byte code{1}[20] = {{
            LDB_IMM,
            list8bitImm({0}),
            {1},
            list8bitImm({2}),
            EXIT,
        }};
        Execute(code{1}, 20);
        assert(getB() == {3}, "{1}");
    """.format(
        initalBValue,
        Operation,
        Op_Param,
        ExpectedResult_B)

def Mk_Arithmetic_OP_A_IMM_Test(
    initalBValue,
    Operation,
    Op_Param,
    ExpectedResult_B
):
    return """
        byte code{1}[20] = {{
            LDA_IMM,
            list8bitImm({0}),
            {1},
            list8bitImm({2}),
            EXIT,
        }};
        Execute(code{1}, 20);
        assert(getA() == {3}, "{1}");
    """.format(
        initalBValue,
        Operation,
        Op_Param,
        ExpectedResult_B)

print(Mk_Arithmetic_OP_A_B_Test("0x02","0x01","SUB_A_B","0x01"))
print(Mk_Arithmetic_OP_B_A_Test("0x02","0x01","SUB_B_A","0x01"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x02","SUB_B_IMM","0x01", "0x01"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x02","SUB_A_IMM","0x01", "0x01"))

print(Mk_Arithmetic_OP_A_B_Test("0x04","0x02","DIV_A_B","0x02"))
print(Mk_Arithmetic_OP_B_A_Test("0x02","0x04","DIV_B_A","0x02"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x04","DIV_B_IMM","0x02", "0x02"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x04","DIV_A_IMM","0x02", "0x02"))

print(Mk_Arithmetic_OP_A_B_Test("0x04","0x02","MUL_A_B","0x08"))
print(Mk_Arithmetic_OP_B_A_Test("0x02","0x04","MUL_B_A","0x08"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x04","MUL_B_IMM","0x02", "0x08"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x04","MUL_A_IMM","0x02", "0x08"))

print(Mk_Arithmetic_OP_A_B_Test("0x04","0x02","ADD_A_B","0x06"))
print(Mk_Arithmetic_OP_B_A_Test("0x02","0x04","ADD_B_A","0x06"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x04","ADD_B_IMM","0x02", "0x06"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x04","ADD_A_IMM","0x02", "0x06"))

print(Mk_Arithmetic_OP_A_B_Test("0x04","0x02","POW_A_B","0x10"))
print(Mk_Arithmetic_OP_B_A_Test("0x02","0x04","POW_B_A","0x10"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x04","POW_B_IMM","0x02", "0x10"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x04","POW_A_IMM","0x02", "0x10"))

print(Mk_Arithmetic_OP_A_B_Test("0x01","0x00","XOR_A_B","0x01"))
print(Mk_Arithmetic_OP_B_A_Test("0x01","0x00","XOR_B_A","0x01"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x01","XOR_B_IMM","0x01", "0x00"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x01","XOR_A_IMM","0x01", "0x00"))

print(Mk_Arithmetic_OP_A_B_Test("0x01","0x00","AND_A_B","0x00"))
print(Mk_Arithmetic_OP_B_A_Test("0x01","0x00","AND_B_A","0x00"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x01","AND_B_IMM","0x01", "0x01"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x01","AND_A_IMM","0x01", "0x01"))

print(Mk_Arithmetic_OP_A_B_Test("0x01","0x00","OR_A_B","0x01"))
print(Mk_Arithmetic_OP_B_A_Test("0x01","0x00","OR_B_A","0x01"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x01","OR_B_IMM","0x01", "0x01"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x01","OR_A_IMM","0x01", "0x01"))

print(Mk_Arithmetic_OP_A_B_Test("0x01","0x00","LT_A_B","0x00"))
print(Mk_Arithmetic_OP_B_A_Test("0x01","0x00","LT_B_A","0x01"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x01","LT_B_IMM","0x01", "0x00"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x01","LT_A_IMM","0x01", "0x00"))

print(Mk_Arithmetic_OP_A_B_Test("0x01","0x00","LE_A_B","0x00"))
print(Mk_Arithmetic_OP_B_A_Test("0x01","0x00","LE_B_A","0x01"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x01","LE_B_IMM","0x01", "0x00"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x01","LE_A_IMM","0x01", "0x00"))

print(Mk_Arithmetic_OP_A_B_Test("0x01","0x00","GT_A_B","0x01"))
print(Mk_Arithmetic_OP_B_A_Test("0x01","0x00","GT_B_A","0x00"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x01","GT_B_IMM","0x01", "0x00"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x01","GT_A_IMM","0x01", "0x00"))

print(Mk_Arithmetic_OP_A_B_Test("0x01","0x00","GE_A_B","0x01"))
print(Mk_Arithmetic_OP_B_A_Test("0x01","0x00","GE_B_A","0x00"))
print(Mk_Arithmetic_OP_B_IMM_Test("0x01","GE_B_IMM","0x01", "0x01"))
print(Mk_Arithmetic_OP_A_IMM_Test("0x01","GE_A_IMM","0x01", "0x01"))