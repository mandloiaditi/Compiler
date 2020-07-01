// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#ifndef _UTILS_H
#define _UTILS_H
#define NUM_NT 57
#define MAX_RHSRULES 10
#define BUFF_SIZE 4096
#define N 200050
#define ll  long long

typedef struct map{
    char *str;
    int value;
    struct map* next;
}hashmap ;

int eof;
FILE *fptr;
char buffer1[BUFF_SIZE + 1];
char buffer2[BUFF_SIZE + 1];
char *lexStart, *fwd;
int buffptr1, buffptr2, lexSize, line;
hashmap keyWordTable[N];
hashmap genericMappingTable[N];
int num_T;

enum TandNT{ program, moduleDeclarations, otherModules, driverModule, moduleDeclaration, module, moduleDef,
    input_plist, ret, N1, output_plist, N2, dataType, type, range_arrays, index_, statements, statement, ioStmt,
    simpleStmt, declareStmt, conditionalStmt, iterativeStmt, var, whichId, boolConstt, var_id_num, assignmentStmt,
    moduleReuseStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, expression, optional, idList, N3, arithmeticOrBooleanExpr,
    U, unary_op, new_NT, arithmeticExpr, AnyTerm, N7, logicalOp, N8, relationalOp, op1, op2, term, N4, factor, N5,
    caseStmts, value, N9, default_, range,e, DECLARE, MODULE, ID, SEMICOL, DEF, DRIVER, PROGRAM, ENDDEF,
    DRIVERDEF, DRIVERENDDEF, TAKES, INPUT, SQBO, SQBC, RETURNS, COMMA, COLON, INTEGER, REAL, BOOLEAN, ARRAY, OF,
    RANGEOP, START, END, GET_VALUE, BO, BC, PRINT, NUM, RNUM, TRUE, FALSE, ASSIGNOP, USE, WITH, PARAMETERS, PLUS,
    MINUS, LE, LT, GE, GT, EQ, NE, AND, OR, MUL, DIV, SWITCH, CASE, BREAK, DEFAULT, FOR, IN, WHILE, DOLLAR, SYN, 
    ERROR,caseSingle,LABEL,GOTO,uminus,GOTOIFTRUE,GOTOIFFALSE,CALL,RETURN, ADDESPMOD, SUBESPMOD, INIT, ACTUAL_OUT_PARAMS, 
    ACTUAL_IN_PARAMS, FORMAL_OUT_PARAMS, FORMAL_IN_PARAMS, POP_ACTUAL_IN_PARAMS, POP_ACTUAL_OUT_PARAMS,TYPE,DYARR,ASSIGNARR,STATARR,
    INP_ARRAY_CHECK, EXIT,DINT,DBOOL,DREAL,UNASSIGNED, RUNTIME_TYPE_ERROR
    };
typedef enum TandNT Term ;


int computehash(char* str);
void addentry(char* ptr, Term val, hashmap* mappingTable);
int fin(char* str, hashmap* mappingTable);
hashmap* findthe(char* str, hashmap* mappingTable);
void init(hashmap* mappingTable);
void initialize_lookup_map1(hashmap* mappingTable);
void initialize_lookup_map2(hashmap* mappingTable);
Term stringToEnum(char*str, hashmap* mappingTable);
Term stringToEnum2(char*str);
char* enumToString(Term c) ;
float convertToFloat(char* str);

#endif