#include "ast.h"
#include "mem_mac.h"

#include <string.h>
#include <stdlib.h>

void printNodeHelper(struct Node* n, char* tab, char ext);
void printBlock(struct Block* b, char* tab, char ext);
void free_block(struct Block* b);

void printNode(struct Node* n) {
    printNodeHelper(n, "", ' ');
}

void printNodeHelper(struct Node* n, char* tab, char ext) {
    char* completeTab = new_array(char, strlen(tab) + 2);
    sprintf(completeTab, "%s%c", tab, ext);
    printf("%s", completeTab);

    switch (n->nt) {
    case INT_NT:
        printf("Integer: %d\n", n->as.Int->Value);
        break;
    case FLT_NT:
        printf("Float: %f\n", n->as.Flt->Value);
        break;
    case CHR_NT:
        printf("Char: %c\n", n->as.Chr->Value);
        break;
    case BINOP_NT:
        printf("Binary Operation (%s)\n", n->as.BinOp->Op);
        printf("%sRHS:\n", completeTab);
        printNodeHelper(n->as.BinOp->RHS, completeTab, '\t');
        printf("%sLHS:\n", completeTab);
        printNodeHelper(n->as.BinOp->LHS, completeTab, '\t');
        break;
    case BOOL_NT:
        if (n->as.Bool->Value) { printf("Boolean: true\n"); }
        else { printf("Boolean: false\n"); }
        break;
    case IFEL_NT:
        printf("If Else:\n");
        for (int i = 0; i < n->as.IfEl->numBlocks; i++) {
            printNodeHelper(n->as.IfEl->Conditions[i], completeTab, '\t');
            printBlock(n->as.IfEl->Blocks[i], completeTab, '\t');
            printf("\n");
        }
        printf("%s%cElse\n", completeTab, '\t');
        printBlock(n->as.IfEl->ElseBlock, completeTab, '\t');
        break;
    case LET_NT:
        printf("let %s ", n->as.Let->Identifier);
        printf("at offset %lld\n", n->as.Let->StackLocation);
        printf("%sValue:\n", completeTab);
        printNodeHelper(n->as.Let->Value, completeTab, '\t');
        break;
    case IDENT_NT:
        printf("Identifier '%s' at location %lld\n", n->as.Ident->Identifier, n->as.Ident->StackLocation);
        break;
    case ASSIGN_NT:
        printf("reassign at offset %lld\n", n->as.Assign->StackLocation);
        printf("%sValue:\n", completeTab);
        printNodeHelper(n->as.Assign->Value, completeTab, '\t');
        break;
    default:
        node_panic(n, "Could not print node\n");
    }
}

void printBlock(struct Block* b, char* tab, char ext) {
    char* completeTab = new_array(char, strlen(tab) + 2);
    sprintf(completeTab, "%s%c", tab, ext);
    printf("%sBlock: ", completeTab);
    printf("Scope size '%d'\n", b->numPrimativeVarsInScope);
    for (int i = 0; i < b->numStatements; i++) {
        printNodeHelper(b->Statements[i], completeTab, '\t');
    }
}

void deleteNode(struct Node* n) {
    switch (n->nt) {
    case INT_NT:
        free(n->as.Int);
        break;
    case FLT_NT:
        free(n->as.Flt);
        break;
    case CHR_NT:
        free(n->as.Chr);
        break;
    case BOOL_NT:
        free(n->as.Bool);
        break;
    case BINOP_NT:
        deleteNode(n->as.BinOp->LHS);
        deleteNode(n->as.BinOp->RHS);
        free(n->as.BinOp->Op);
        free(n->as.BinOp);
        break;
    case IFEL_NT:
        for (int i = 0; i < n->as.IfEl->numBlocks; i++) {
            deleteNode(n->as.IfEl->Conditions[i]);
            free_block(n->as.IfEl->Blocks[i]);
        }
        free(n->as.IfEl->Blocks);
        free(n->as.IfEl->Conditions);
        if (n->as.IfEl->ElseBlock != NULL) {
            free_block(n->as.IfEl->ElseBlock);
        }
        free(n->as.IfEl);
        break;
    case WHILE_NT:
        free_block(n->as.While->Block);
        deleteNode(n->as.While->Condition);
        free(n->as.While);
        break;
    case LET_NT:
        free(n->as.Let->Identifier);
        deleteNode(n->as.Let->Value);
        free(n->as.Let);
        break;
    case IDENT_NT:
        free(n->as.Ident); /*Identifier char* cleared when token freed */
        break;
    case ASSIGN_NT:
        free(n->as.Assign->Value);
        free(n->as.Assign);
        break;
    default:
        node_panic(n, "Could not free node '%d'\n", n->nt);
    }

    free(n->tok->Contents);
    free(n->tok);
    free(n);
}

void free_block(struct Block* b) {
    for (int i = 0; i < b->numStatements; i++) {
        deleteNode(b->Statements[i]);
    }
}