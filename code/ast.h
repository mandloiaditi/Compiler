// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "symbolTableDef.h"

#ifndef _AST_H
#define _AST_H

int numAstNodes;
int astMemUsage;
typedef struct astNT{
    Term dataType;
    Term symbol;
}astNT;

typedef struct astLeaf{
    token t;
    Term dataType;
}astLeaf ;

typedef union unionAst{
    astNT nt;
    astLeaf ln;
}uAst;

typedef struct astNode{
    uAst unode;
    int isLeaf;
    int ruleNum;
    char * addr;
    struct astNode *child, *sibling, *parent;
    idST * idt ;
}astNode;


typedef struct quadruple{
    char *arg1, *arg2,* result;
    Term op;
    astNode *node1, *node2, *nodeResult;
    idST * idt1,*idt2,*resultIdt;
    struct quadruple *next; 
    
}quadruple;

astNode *createNewLeaf(token t);
astNode * createNewNode(Term term, astNode *node);
astNode * generateAST(char *out, int printFlag);
astNode * memUsage(char *out);

void printAST(astNode *head);
#endif
