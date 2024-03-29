#include "codegen.h"
#include "hash.h"
#include "parser.h"
#include "mem_mac.h"
#include "instructions.h"
#include "panic.h"

#include <string.h>

#define gen_panic(GEN, ...) {\
    panic(GEN->parser.curTok->line, GEN->parser.curTok->column, GEN->parser.curTok->filename, __VA_ARGS__); \
}

void AppendCode(struct CodeGenerator* cg, byte* code, int codelen);
void compileNode(struct CodeGenerator* cg, struct Node* n);
void compileInt(struct CodeGenerator* cg, struct Node* IntNode);
void compileChar(struct CodeGenerator* cg, struct Node* n);
void compileBool(struct CodeGenerator* cg, struct Node* BoolNode);
void compileBinOp(struct CodeGenerator* cg, struct Node* node);
void compileIfElse(struct CodeGenerator* cg, struct Node* node);
void compileWhile(struct CodeGenerator* cg, struct Node* n);
void compileLet(struct CodeGenerator* cg, struct Node* n);
void compileIdent(struct CodeGenerator* cg, struct Node* n);
void compileAssign(struct CodeGenerator* cg, struct Node* n);
void compileList(struct CodeGenerator* cg, struct Node* n);
void compileBuiltin(struct CodeGenerator* cg, struct Node* n);

struct CodeObj Compile(char* filename) {
    struct CodeGenerator cg;
    cg.OutputCode = NULL;
    cg.OutputCodeLen = 0;
    cg.parser = newParser(filename);

    cg.OpAImm = newMap();
    setPair(cg.OpAImm, "+", ADD_A_IMM);
    setPair(cg.OpAImm, "-", SUB_A_IMM);
    setPair(cg.OpAImm, "*", MUL_A_IMM);
    setPair(cg.OpAImm, "/", DIV_A_IMM);
    setPair(cg.OpAImm, "==", EQ_A_IMM);
    setPair(cg.OpAImm, "!=", NE_A_IMM);
    setPair(cg.OpAImm, "||", OR_A_IMM);
    setPair(cg.OpAImm, "&&", AND_A_IMM);

    cg.OpAB = newMap();
    setPair(cg.OpAB, "+", ADD_A_B);
    setPair(cg.OpAB, "-", SUB_A_B);
    setPair(cg.OpAB, "*", MUL_A_B);
    setPair(cg.OpAB, "/", DIV_A_B);
    setPair(cg.OpAB, "==", EQ_A_B);
    setPair(cg.OpAB, "!=", NE_A_B);
    setPair(cg.OpAB, "||", OR_A_B);
    setPair(cg.OpAB, "&&", AND_A_B);

    do { /* TODO: What about empty files */
        ParseStmt(&cg.parser);
        compileNode(&cg, cg.parser.curNode);
    } while (cg.parser.curTok->Type != EOF_TT);

    byte exitIns = EXIT;
    AppendCode(&cg, &exitIns, 1);

    deleteMap(cg.OpAImm);
    deleteMap(cg.OpAB);
    deleteParser(cg.parser);

    struct CodeObj retVal;
    retVal.code = cg.OutputCode;
    retVal.codelen = cg.OutputCodeLen;

    return retVal;
}

void AppendCode(struct CodeGenerator* cg, byte* code, int codelen)  {

    if (cg->OutputCode == NULL) {
        cg->OutputCode = new_array(byte, codelen);
        memcpy(cg->OutputCode, code, codelen);
    } else {
        expand_array(byte, cg->OutputCode, cg->OutputCodeLen, cg->OutputCodeLen + codelen);
        memcpy(cg->OutputCode + cg->OutputCodeLen, code, codelen);
    }

    cg->OutputCodeLen += codelen;
}

void compileNode(struct CodeGenerator* cg, struct Node* n) {
    switch (n->nt) {
    case INT_NT:
        compileInt(cg, n);
        break;
    case BOOL_NT:
        compileBool(cg, n);
        break;
    case BINOP_NT:
        compileBinOp(cg, n);
        break;
    case IFEL_NT:
        compileIfElse(cg, n);
        break;
    case WHILE_NT:
        compileWhile(cg, n);
        break;
    case LET_NT:
        compileLet(cg, n);
        break;
    case IDENT_NT:
        compileIdent(cg, n);
        break;
    case ASSIGN_NT:
        compileAssign(cg, n);
        break;
    case LIST_NT:
        compileList(cg, n);
        break;
    case CHR_NT:
        compileChar(cg, n);
        break;
    case BUILTIN_NT:
        compileBuiltin(cg, n);
        break;
    default:
        gen_panic(cg, "Could not generate code for '%s'\n", cg->parser.curNode->tok->Contents);
    }
}

void putIntInByteArray(qword val, byte* buffer) {
    memcpy(buffer, &val, sizeof(qword));
}

void compileInt(struct CodeGenerator* cg, struct Node* IntNode) {
    byte code[9];
    code[0] = LDA_IMM;
    putIntInByteArray(IntNode->as.Int->Value, code+1);
    AppendCode(cg, code, 9);
}

void compileChar(struct CodeGenerator* cg, struct Node* n) {
    byte code[9];
    code[0] = LDA_IMM;
    putIntInByteArray((qword) (n->as.Chr->Value), code+1);
    AppendCode(cg, code, 9);
}

void compileBool(struct CodeGenerator* cg, struct Node* BoolNode) {
    byte code[9];
    code[0] = LDA_IMM;
    putIntInByteArray(BoolNode->as.Bool->Value, code+1);
    AppendCode(cg, code, 9);
}

void compileBinOp(struct CodeGenerator* cg, struct Node* node) {
    compileNode(cg, node->as.BinOp->LHS);

    int opcode;
    switch (node->as.BinOp->RHS->nt) {
    case INT_NT:
        opcode = lookup(cg->OpAImm, node->as.BinOp->Op);
        byte int_code[9];
        int_code[0] = opcode;
        putIntInByteArray(node->as.BinOp->RHS->as.Int->Value, int_code+1);
        AppendCode(cg, int_code, 9);
        break;
    case BOOL_NT:
        opcode = lookup(cg->OpAImm, node->as.BinOp->Op);
        byte bool_code[9];
        bool_code[0] = opcode;
        putIntInByteArray(node->as.BinOp->RHS->as.Bool->Value, bool_code+1);
        AppendCode(cg, bool_code, 9);
        break;
    case BINOP_NT:
        opcode = lookup(cg->OpAB, node->as.BinOp->Op);

        byte push_code = PUSH_A;
        AppendCode(cg, &push_code, 1);

        compileNode(cg, node->as.BinOp->RHS);

        byte binop_code[3] = {LDB_A, POP_A, opcode};
        AppendCode(cg, binop_code, 3);

        break;
    default:
        gen_panic(cg, "Could not generate code for RHS\n");
    }

    if (opcode == -1) {
        gen_panic(cg, "Could not find opcode\n");
    }
}

void compileBlock(struct CodeGenerator* cg, struct Block* b) {
    for (int i = 0; i < b->numStatements; i ++) {
        compileNode(cg, b->Statements[i]);
    }

    if (b->numPrimativeVarsInScope != 0) {
        byte frame_code[9];
        frame_code[0] = SHRINK_STACK_SIZE;
        putIntInByteArray(b->numPrimativeVarsInScope, frame_code+1);
        AppendCode(cg, frame_code, 9);
    } if (b->numCompositeVarsInScope != 0) {
        byte frame_code[9];
        frame_code[0] = SHRINK_BUFFER_STACK_SIZE;
        putIntInByteArray(b->numCompositeVarsInScope, frame_code+1);
        AppendCode(cg, frame_code, 9);
    }
}

void compileIfElse(struct CodeGenerator* cg, struct Node* node) {
    int numJmpOutBackpatches = 0;
    int* jmpOutBackpatches = NULL;

    for (int i = 0; i < node->as.IfEl->numBlocks; i++) {
        compileNode(cg, node->as.IfEl->Conditions[i]);
        int jmpNextBackpatch = cg->OutputCodeLen + 1;

        byte jmpNext_code[9];
        jmpNext_code[0] = JPA_Z_OFF;
        AppendCode(cg, jmpNext_code, 9);

        compileBlock(cg, node->as.IfEl->Blocks[i]);

        numJmpOutBackpatches ++;
        if (jmpOutBackpatches == NULL) {
            jmpOutBackpatches = new_array(int, numJmpOutBackpatches);
        } else {
            expand_array(int, jmpOutBackpatches, numJmpOutBackpatches - 1, numJmpOutBackpatches);
        }

        jmpOutBackpatches[numJmpOutBackpatches-1] = cg->OutputCodeLen + 1;

        byte jmpOut_code[9];
        jmpOut_code[0] = JP_OFF;
        AppendCode(cg, jmpOut_code, 9);

        putIntInByteArray(cg->OutputCodeLen, cg->OutputCode + jmpNextBackpatch);
    }

    if (node->as.IfEl->ElseBlock != NULL) {
        compileBlock(cg, node->as.IfEl->ElseBlock);
    }

    for (int i = 0; i < numJmpOutBackpatches; i++) {
        putIntInByteArray(cg->OutputCodeLen, cg->OutputCode + jmpOutBackpatches[i]);
    }

    free(jmpOutBackpatches);
}

void compileWhile(struct CodeGenerator* cg, struct Node* n) {
    if (n->as.While->isDoWhile) {
        qword startOfLoop = cg->OutputCodeLen;
        compileBlock(cg, n->as.While->Block);
        compileNode(cg, n->as.While->Condition);

        byte do_jmp[9];
        do_jmp[0] = JPA_OFF;
        putIntInByteArray(startOfLoop, do_jmp+1);
        AppendCode(cg, do_jmp, 9);
    } else {
        compileNode(cg, n->as.While->Condition);
        int backpatch = cg->OutputCodeLen + 1;

        byte jmp[9];
        jmp[0] = JPA_Z_OFF;
        AppendCode(cg, jmp, 9);

        compileBlock(cg, n->as.While->Block);
        putIntInByteArray(cg->OutputCodeLen, cg->OutputCode + backpatch);
    }
}

void compileLet(struct CodeGenerator* cg, struct Node* n) {
    compileNode(cg, n->as.Let->Value);

    if (isTypeComplex(n->as.Let->Value->vt)) {
        byte ess[9];
        ess[0] = ENSURE_BUFFER_STACK_SIZE;
        putIntInByteArray(n->as.Let->StackLocation + 1, ess + 1);
        AppendCode (cg, ess, 9);
        
        byte insert[9];
        insert[0] = INSERT_BUFFER_STACK_IMM_BPA;
        putIntInByteArray(n->as.Let->StackLocation, insert + 1);
        AppendCode(cg, insert, 9);

    } else {
        byte ess[9];
        ess[0] = ENSURE_STACK_SIZE;
        putIntInByteArray(n->as.Let->StackLocation + 1, ess + 1);
        AppendCode(cg, ess, 9);

        byte insert[9];
        insert[0] = INSERT_STACK_IMM_A;
        putIntInByteArray(n->as.Let->StackLocation, insert + 1);
        AppendCode(cg, insert, 9);
    }
}

void compileIdent(struct CodeGenerator* cg, struct Node* n) {
    if (isTypeComplex(n->vt)) {
        byte output_code[9];
        output_code[0] = LDBPA_STACK_IMM;
        putIntInByteArray(n->as.Ident->StackLocation, output_code + 1);
        AppendCode(cg, output_code, 9);

    } else {
        byte output_code[9];
        output_code[0] = LDA_STACK_IMM;
        putIntInByteArray(n->as.Ident->StackLocation, output_code + 1);
        AppendCode(cg, output_code, 9);
    }
}

void compileAssign(struct CodeGenerator* cg, struct Node* n) {
    compileNode(cg, n->as.Assign->Value);
    byte insert[9];

    if (isTypeComplex(n->as.Assign->Value->vt)) {
        insert[0] = INSERT_BUFFER_STACK_IMM_BPA;
    } else {
        insert[0] = INSERT_STACK_IMM_A;
    }

    putIntInByteArray(n->as.Assign->StackLocation, insert + 1);
    AppendCode(cg, insert, 9);
}

void compileList(struct CodeGenerator* cg, struct Node* n) {
    if (isTypeComplex(n->vt->subtype_info.ListEntryType)) {
        /* LDQBS 0
         * LDRBS <sizeof_list>
         * ALLOC BPA
         * 
         * PUSH BPA
         * EVAL EXPR
         * LDBPB BPA
         * POP BPA
         * LDM BPA[iter], BPB
         */

        byte ldqbs[9];
        ldqbs[0] = LDQBS_IMM;
        putIntInByteArray(0, ldqbs+1);
        AppendCode(cg, ldqbs, 9);

        byte ldrbs[9];
        ldrbs[0] = LDRBS_IMM;
        putIntInByteArray(n->as.List->numValues, ldrbs+1);
        AppendCode(cg, ldrbs, 9);

        byte alloc = ALLOC_BPA;
        AppendCode(cg, &alloc, 1);

        for (int i = 0; i < n->as.List->numValues; i++) {
            byte push = PUSH_BPA;
            AppendCode(cg, &push, 1);

            compileNode(cg, n->as.List->Values[i]);

            byte pop[2] = {LDBPB_BPA, POP_BPA};
            AppendCode(cg, pop, 2);

            byte ldm[9];
            ldm[0] = LDM_BPAOFFIMM_BPB;
            putIntInByteArray(i, ldm+1);
            AppendCode(cg, ldm, 9);
        }

    } else {
        /* LDQBS <sizeof_list>
         * LDRBS 0
         * ALLOC BPA
         * EVAL EXPR
         * LDM BPA[iter], a
         */

        byte ldqbs[9];
        ldqbs[0] = LDQBS_IMM;
        putIntInByteArray(n->as.List->numValues, ldqbs + 1);
        AppendCode(cg, ldqbs, 9);

        byte ldrbs[9];
        ldrbs[0] = LDRBS_IMM;
        putIntInByteArray(0, ldrbs+1);
        AppendCode(cg, ldrbs, 9);

        byte alloc = ALLOC_BPA;
        AppendCode(cg, &alloc, 1);

        for (int i = 0; i < n->as.List->numValues; i++) {
            compileNode(cg, n->as.List->Values[i]);
            byte ldm[9];
            ldm[0] = LDM_BPAOFFIMM_A;
            putIntInByteArray(i, ldm+1);
            AppendCode(cg, ldm, 9);
        }
    }
}

void compilePrint(struct CodeGenerator* cg, struct Node* n) {
    for (int i = 0; i < n->as.Builtin->numArgs; i++) {
        struct Node* thisNode = n->as.Builtin->Args[i];

        byte code[2];
        code[0] = BUILTIN;

        compileNode(cg, thisNode);
        if (thisNode->vt->base_type == LIST_BT && thisNode->vt->subtype_info.ListEntryType->base_type == CHR_BT) {
            code[1] = PRINT_ASCII_BPA;
        } else if (isTypeComplex(thisNode->vt)) {
            code[1] = PRINT_BPA;
        } else if (thisNode->vt->base_type == CHR_BT) {
            code[1] = PRINT_ASCII_A;
        } else {
            code[1] = PRINT_HEX_A;
        }

        AppendCode(cg, code, 2);
    }
}

void compileBuiltin(struct CodeGenerator* cg, struct Node* n) {
    if (!strcmp("print", n->as.Builtin->builtinName)) {
        compilePrint(cg, n);
    } else {
        gen_panic(cg, "Invalid builtin '%s'\n", n->as.Builtin->builtinName);
    }
}