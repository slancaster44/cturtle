#include "codegen.h"
#include "instructions.h"
#include "mem_mac.h"
#include "parser.h"
#include "ast.h"

#include <string.h>

void pushStackFrame(struct CodeGenerator* cg);
void popStackFrame(struct CodeGenerator* cg);

byte* compileExpr(struct CodeGenerator* cg, struct Node* expr);
byte* compileInt(struct CodeGenerator* cg, struct Node* intExpr);

void intToByteArray(uint64_t value, byte* outputBuffer);

struct CodeGenerator* newCodeGenerator(char* filename) {
    struct CodeGenerator* retVal = new(struct CodeGenerator);

    retVal->parser = new(struct Parser);
    struct Parser tempParser = newParser(filename);
    memcpy(retVal->parser, &tempParser, sizeof(struct Parser));

    retVal->currentStatementCode = NULL;

    retVal->symbolStackDepth = 0;
    retVal->symbolStack = NULL; /*TODO*/

    return retVal;
}

void deleteCodeGenerator(struct CodeGenerator* cg) {
    deleteParser(*cg->parser);
    free(cg->parser);

    for (int i = 0; i < cg->symbolStackDepth; i++) {
        deleteSymTab(cg->symbolStack[i]);
    }
    
    if (cg->symbolStack != NULL)
        free(cg->symbolStack);

    if (cg->currentStatementCode != NULL)
        free(cg->currentStatementCode);

}

void compileCurrentStatement(struct CodeGenerator* cg) {
    ParseStmt(cg->parser);
    struct Node* statement = cg->parser->curNode;
    cg->currentStatementCode = compileExpr(cg, statement);
}

byte* compileExpr(struct CodeGenerator* cg, struct Node* expr) {
    switch (expr->nt) {
    case INT_NT:
        return compileInt(cg, expr);
        break;
    default:
        printf("Could not generate code for expression type '%d'\n", expr->nt);
        exit(1);
    }
}

void copyIntToByteArray(uint64_t value, byte* outputBuffer) {
    memcpy(outputBuffer, &value, sizeof(uint64_t));
}

#define SIZEOF_LDA_IMM 9
byte* compileInt(struct CodeGenerator* cg, struct Node* intExpr) {
    int val = intExpr->as.Int->Value;
    byte* instructions = new_array(byte, SIZEOF_LDA_IMM);
    
    instructions[0] = LDA_IMM;
    copyIntToByteArray((uint64_t) val, instructions + 1);

    return instructions;
}
