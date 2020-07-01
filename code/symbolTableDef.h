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
#ifndef _SYM_H
#define _SYM_H

typedef struct idST idST;

typedef struct idSTEntry idSTEntry;

typedef union uni{
  int num ;
  idSTEntry * entry;  
}rangeNode ;

typedef struct idSTEntry {
    char lexeme[50];
    Term dataType;
    int offset;
    int width;    
    token t;
    int isArray;
    int isAssigned;
    rangeNode start;
    rangeNode end;
    int isStatic ;
    int start_pop, end_pop;
}idSTEntry;

typedef struct hashdata{
       char* str;
       idSTEntry data ;
       struct hashdata* next;
}hashTable_id;


typedef struct pNode {
    char lexeme[50]; 
    Term dataType;
    int isAssigned;
    struct pNode * next;
    int isArray;
}pNode;

typedef struct idST{
      hashTable_id * idHashTable ;
	    int start,end;
      struct idST * sibling, * child, * parent;
      int nesting;
}idST;

typedef struct moduleSTEntry{
    char lexeme_mod[25];
    token t;
    pNode *inParams, *outParams;
    int numInp, numOut;
    int moduleDefined;
    int moduleDeclared;
    int isDecValid;
    int moduleSize;
    int decOffset; 
    idST * idt;
} moduleSTEntry;


typedef struct hashdata2{
       char* str;
       moduleSTEntry data ;
       struct hashdata2* next;
}hashTable_func;

typedef struct moduleST{
    hashTable_func * modHashTable;
}moduleST;

#endif