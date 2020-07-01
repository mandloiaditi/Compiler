// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "utils.h"
#include "lexerDef.h"
#include "parserDef.h"
#include "symbol.h"
#include "ast.h"

#ifndef _R_H
#define _R_H

typedef struct iserror
{
	int line;
	int type;
}isError;

extern isError errArr[5000];
extern int hasSemanticError;

void travAndSet(astNode *tmp,moduleST *mt, int errorFlag);
void travAndSet1(astNode *tmp,moduleST *mt, int errorFlag);
int correct(astNode *tmp,moduleST *mt, int errorFlag);
int check(astNode *tmp,moduleST *mt);
void checkModuleReuse(astNode * node, moduleST * mt, int errorFlag);
int checkConditional(astNode * node, moduleST * mt, int errorFlag);
void semanticAnalyzer(astNode * root, moduleST * mt, int errorFlag);
void checkIterative(astNode * node, moduleST * mt, int errorFlag);
astNode *findWhileEntry(astNode *node);
Term checkArray(astNode * node, int errorFlag);
void checkAssign(astNode * node, moduleST * mt, int errorFlag);
Term U_type(astNode* whichST, int line, int errorFlag);
Term anyTerm1(astNode *temp,int line, int errorFlag);
Term ariExp1(astNode *temp,int line, int errorFlag);
Term trmTrav(astNode *node, int line, int errorFlag);
Term factTrav(astNode *node,int line, int errorFlag);
Term ariBoolTrav(astNode *temp,int line, int errorFlag);
Term trm1(astNode *node,int line, int errorFlag);
Term fact1(astNode *node,int line, int errorFlag);
Term fact2(astNode *node,int line, int errorFlag);
Term ariBool1(astNode *temp,int line, int errorFlag);
#endif