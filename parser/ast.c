#include "ast.h"
#include "mem_mac.h"

#include <string.h>
#include <stdlib.h>

void printNodeHelper(struct Node* n, char* tab, char ext);

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
    default:
        node_panic(n, "Could not print node\n");
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
        break;
    default:
        node_panic(n, "Could not free node\n");
    }

    free(n->tok->Contents);
    free(n->tok);
    free(n);
}
