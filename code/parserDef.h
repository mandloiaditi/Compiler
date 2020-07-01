// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH


#include "utils.h"
#include "lexer.h"
#include "ast.h"
#ifndef _PARSERD_H
#define _PARSERD_H

typedef struct node{
    int isTerminal; 
    Term symbol;
    struct node *next;  
}node;

typedef struct NTnode{
    Term value;
	struct treeNode *child,*sibling, *parent;
}NTnode;

typedef struct leafNode{
    token t;
    struct treeNode *child, *sibling, *parent;
}leafNode;

typedef union unionNode{
    NTnode nt;
    leafNode ln;
}unionNode;

typedef struct treeNode{
    astNode *anode,*inh,*syn,*syn_list;
    unionNode unode; 
    int isTerminal;
}treeNode;


typedef struct stackNode{
    struct stackNode *next;
    treeNode *position;
    
}stackNode;


typedef struct lhs{
    node *rhs[MAX_RHSRULES];
    Term value;
    int numRules;
    int isEps;
}lhs;

typedef long long * First;

typedef long long * Follow;

typedef struct grammar{
    lhs head[NUM_NT];
    First first;
    Follow follow;
}grammar;

typedef struct parseTableEntry{
    Term whichNT;
    int ruleIndex;
}parseTableEntry;

treeNode * treeHead ;
#endif 