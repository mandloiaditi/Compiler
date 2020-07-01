// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH

#include "utils.h"
#include "lexer.h"
#include "parserDef.h"
#ifndef _PARSER_H
#define _PARSER_H

extern int hasSyntacticError;
extern int ptMemUsage;

void initializeParser(char *fname1, char *fname2, int printFlag,int printTree, int printErr);
grammar readGrammar(FILE * ptr);
void computeFirstAndFollowSets(grammar *G);
long long calcFirst(grammar * G, int NT_index, int *visited);
void calcFollow(grammar *G, int NT_index, int visited[]);
long long calcFirstRule(grammar * G, int NT_index, int numRule);
void printFirst(grammar * G);
void printFollow(grammar * G);
void printGrammar(grammar g);
void createParseTable(grammar *G, parseTableEntry T[][DOLLAR-NUM_NT]);
void parseInputFile(FILE * fptr, parseTableEntry Table[][DOLLAR-NUM_NT],grammar g, char *out, int printErr);
void printStack(stackNode *head);
void printTN(treeNode * t , FILE * fptr, int errFlag);
void inorder(treeNode *treeHead,FILE * out, int errFlag);
treeNode * returnRoot(char *out);
#endif



