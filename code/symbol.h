// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH
#include<stdio.h>
#include<string.h>
#include "utils.h"
#include "lexerDef.h"
#include "symbolTableDef.h"
#ifndef _SYM2_H
#define _SYM2_H

extern int hasSemanticError;

moduleST  * functionST;
idSTEntry* insertInIDHashTable(idSTEntry* val,hashTable_id* mappingTable);
idSTEntry* searchIDHashTable(char* str, hashTable_id* mappingTable);
hashTable_id* createIDHashTable();
moduleSTEntry* insertInModuleHashTable(moduleSTEntry* val, hashTable_func* mappingTable);
moduleSTEntry* searchModuleHashTable(char* str, hashTable_func* mappingTable);
hashTable_func* createModHashTable();
idST * createidST();
idSTEntry * lookupidST( idST * idt, char * lex);
idST * insertInidST(idST * idt, idSTEntry * in);
moduleST * createmoduleST();
void traverseAST(astNode* root, moduleST* mt, int errorFlag);
moduleSTEntry * lookupmoduleST( moduleST * mt, char* lex);
moduleST * insertInmoduleST( moduleST * mt, moduleSTEntry * me);
void populateParameterlist(astNode * node ,moduleSTEntry * mSTentry, int errorFlag);
void populateSTStmt(astNode *node, idST *idt,moduleST* mt,moduleSTEntry * min, int errorFlag);
void populatemoduleST(astNode *node , moduleST * mt, int errorFlag);
void populateSymbolTable(astNode * root, int errorFlag);
void printActivationRecSize(moduleST *hash);
void printSymbolTable(moduleST * hash);
void printModule(moduleSTEntry mod);
void printArrays(moduleST * hash);
void printIDT(idST *idt,moduleSTEntry mod);
void arrayHelper(idST *idt,moduleSTEntry mod);
idST * lookUpID(char * lex,idST * idt);
void ariBool(astNode *temp, idST *idt , moduleST * mt, int errorFlag);
void anyTerm(astNode *temp, idST *idt , moduleST * mt, int errorFlag);
void fact(astNode *temp, idST *idt , moduleST * mt, int errorFlag);
void trm(astNode *temp, idST *idt , moduleST * mt, int errorFlag);
void ariExp(astNode *temp, idST *idt , moduleST * mt, int errorFlag);
void varIdNum(astNode *temp, idST *idt , moduleST * mt, int errorFlag);
moduleST * getmt();

void findWidth(idST * in  , int * x);
#endif
