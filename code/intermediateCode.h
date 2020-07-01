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

#ifndef _IC_H
#define _IC_H
extern int hasRunTimeError;
extern int errorIndex;
extern isError errArr[5000];

int newTemp();
int newReg();
int newLabel();
quadruple * createQuadruple();
quadruple * addToEnd( quadruple * head, quadruple * node);
void printQuadruple(quadruple *head);
quadruple* generateIntermediateCode(astNode *root);
quadruple * generateIntermediateCodeHelper(quadruple *head, astNode *root,moduleSTEntry * min);
quadruple *IRcodeIoStmt(astNode *node,moduleSTEntry * min);
quadruple *IRcodeVarArrayCase(astNode *node,moduleSTEntry * min, int nextlabel);
quadruple* IRcodeIterative(astNode * node ,moduleSTEntry * min);
idST *findIDT(idST *idt,int line);
quadruple *IRcodeAssign(astNode *node,moduleSTEntry * min);
quadruple *IRcodeModuleReuse(astNode *node, moduleSTEntry * min);
quadruple *UCode(astNode* node, idST *idtable,moduleSTEntry * min);
quadruple *trmCode(astNode *node, idST *idt,moduleSTEntry * min);
quadruple *postTravTrm(astNode *node, idST *idt,moduleSTEntry * min);
idST *travandfind(astNode *node);
quadruple *factCode(astNode *node, idST *idt,moduleSTEntry * min);
quadruple *postTravFact(astNode *node, idST *idt,moduleSTEntry * min);
quadruple *fact1Code(astNode *node, idST *idt,moduleSTEntry * min);
quadruple *ariBoolCode(astNode *node,idST *idt,moduleSTEntry * min);
quadruple *postTravAny(astNode *node, idST *idt,moduleSTEntry * min);
quadruple *anyTermCode(astNode *node, idST *idt,moduleSTEntry * min);
quadruple *ariExpCode(astNode *node, idST *idt,moduleSTEntry * min);
quadruple * IRcodeConditional(astNode* node, quadruple* head,moduleSTEntry * min);
quadruple * IRcodeStmts(astNode * node, quadruple * head,moduleSTEntry * min);
quadruple* reverse(quadruple * in);
quadruple * moduleTraverse(astNode * node);
quadruple * IRcodeDeclareStmt(astNode * temp,moduleSTEntry *min);


#endif