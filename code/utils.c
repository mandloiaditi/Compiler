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
#include "utils.h"
#include "lexer.h"

char* enumToString(Term c){
    char* val=(char*)malloc(sizeof(char)*30);
    switch(c){
        case program:{
            strcpy(val,"program");
        }break;
        case moduleDeclarations:{
            strcpy(val,"moduleDeclarations");
        }break;
        case otherModules:{
            strcpy(val,"otherModules");
        }break;
        case driverModule:{
            strcpy(val,"driverModule");
        }break;
        case moduleDeclaration:{
            strcpy(val,"moduleDeclaration");
        }break;
        case module:{
            strcpy(val,"module");
        }break;
        case moduleDef:{
            strcpy(val,"moduleDef");
        }break;
        case input_plist:{
            strcpy(val,"input_plist");
        }break;
        case ret:{
            strcpy(val,"ret");
        }break;
        case N1:{
            strcpy(val,"N1");
        }break;
        case output_plist:{
            strcpy(val,"output_plist");
        }break;
        case N2:{
            strcpy(val,"N2");
        }break;
        case dataType:{
            strcpy(val,"dataType");
        }break;
        case type:{
            strcpy(val,"type");
        }break;
        case range_arrays:{
            strcpy(val,"range_arrays");
        }break;
        case index_:{
            strcpy(val,"index");
        }break;
        case statements:{
            strcpy(val,"statements");
        }break;
        case statement:{
            strcpy(val,"statement");
        }break;
        case ioStmt:{
            strcpy(val,"ioStmt");
        }break;
        case simpleStmt:{
            strcpy(val,"simpleStmt");
        }break;
        case declareStmt:{
            strcpy(val,"declareStmt");
        }break;
        case conditionalStmt:{
            strcpy(val,"conditionalStmt");
        }break;
        case iterativeStmt:{
            strcpy(val,"iterativeStmt");
        }break;
        case var:{
            strcpy(val,"var");
        }break;
        case whichId:{
            strcpy(val,"whichId");
        }break;
        case boolConstt:{
            strcpy(val,"boolConstt");
        }break;
        case var_id_num:{
            strcpy(val,"var_id_num");
        }break;
        case assignmentStmt:{
            strcpy(val,"assignmentStmt");
        }break;
        case moduleReuseStmt:{
            strcpy(val,"moduleReuseStmt");
        }break;
        case whichStmt:{
            strcpy(val,"whichStmt");
        }break;
        case lvalueIDStmt:{
            strcpy(val,"lvalueIDStmt");
        }break;
        case lvalueARRStmt:{
            strcpy(val,"lvalueARRStmt");
        }break;
        case expression:{
            strcpy(val,"expression");
        }break;
        case optional:{
            strcpy(val,"optional");
        }break;
        case idList:{
            strcpy(val,"idList");
        }break;
        case N3:{
            strcpy(val,"N3");
        }break;
        case arithmeticOrBooleanExpr:{
            strcpy(val,"arithmeticOrBooleanExpr");
        }break;
        case U:{
            strcpy(val,"U");
        }break;
        case unary_op:{
            strcpy(val,"unary_op");
        }break;
        case new_NT:{
            strcpy(val,"new_NT");
        }break;
        case arithmeticExpr:{
            strcpy(val,"arithmeticExpr");
        }break;
        case AnyTerm:{
            strcpy(val,"AnyTerm");
        }break;
        case N7:{
            strcpy(val,"N7");
        }break;
        case logicalOp:{
            strcpy(val,"logicalOp");
        }break;
        case N8:{
            strcpy(val,"N8");
        }break;
        case relationalOp:{
            strcpy(val,"relationalOp");
        }break;
        case op1:{
            strcpy(val,"op1");
        }break;
        case op2:{
            strcpy(val,"op2");
        }break;
        case term:{
            strcpy(val,"term");
        }break;
        case N4:{
            strcpy(val,"N4");
        }break;
        case factor:{
            strcpy(val,"factor");
        }break;
        case N5:{
            strcpy(val,"N5");
        }break;
        case caseStmts:{
            strcpy(val,"caseStmts");
        }break;
        case value:{
            strcpy(val,"value");
        }break;
        case N9:{
            strcpy(val,"N9");
        }break;
        case default_:{
            strcpy(val,"default");
        }break;
        case range:{
            strcpy(val,"range");
        }break;
        case e:{
            strcpy(val,"e");
        }break;
        case DECLARE:{
            strcpy(val,"DECLARE");
        }break;
        case MODULE:{
            strcpy(val,"MODULE");
        }break;
        case ID:{
            strcpy(val,"ID");
        }break;
        case SEMICOL:{
            strcpy(val,"SEMICOL");
        }break;
        case DEF:{
            strcpy(val,"DEF");
        }break;
        case DRIVERENDDEF:{
            strcpy(val,"DRIVERENDDEF");
        }break;
        case DRIVERDEF:{
            strcpy(val,"DRIVERDEF");
        }break;
        case DRIVER:{
            strcpy(val,"DRIVER");
        }break;
        case ENDDEF:{
            strcpy(val,"ENDDEF");
        }break;
        case TAKES:{
            strcpy(val,"TAKES");
        }break;
        case INPUT:{
            strcpy(val,"INPUT");
        }break;
        case SQBO:{
            strcpy(val,"SQBO");
        }break;
        case SQBC:{
            strcpy(val,"SQBC");
        }break;
        case RETURNS:{
            strcpy(val,"RETURNS");
        }break;
        case COMMA:{
            strcpy(val,"COMMA");
        }break;
        case COLON:{
            strcpy(val,"COLON");
        }break;
        case INTEGER:{
            strcpy(val,"INTEGER");
        }break;
        case REAL:{
            strcpy(val,"REAL");
        }break;
        case BOOLEAN:{
            strcpy(val,"BOOLEAN");
        }break;
        case ARRAY:{
            strcpy(val,"ARRAY");
        }break;
        case OF:{
            strcpy(val,"OF");
        }break;
        case RANGEOP:{
            strcpy(val,"RANGEOP");
        }break;
        case START:{
            strcpy(val,"START");
        }break;
        case END:{
            strcpy(val,"END");
        }break;
        case GET_VALUE:{
            strcpy(val,"GET_VALUE");
        }break;
        case BO:{
            strcpy(val,"BO");
        }break;
        case BC:{
            strcpy(val,"BC");
        }break;
        case PRINT:{
            strcpy(val,"PRINT");
        }break;
        case NUM:{
            strcpy(val,"NUM");
        }break;
        case RNUM:{
            strcpy(val,"RNUM");
        }break;
        case TRUE:{
            strcpy(val,"TRUE");
        }break;
        case FALSE:{
            strcpy(val,"FALSE");
        }break;
        case ASSIGNOP:{
            strcpy(val,"ASSIGNOP");
        }break;
        case USE:{
            strcpy(val,"USE");
        }break;
        case WITH:{
            strcpy(val,"WITH");
        }break;
        case PARAMETERS:{
            strcpy(val,"PARAMETERS");
        }break;
        case PLUS:{
            strcpy(val,"PLUS");
        }break;
        case MINUS:{
            strcpy(val,"MINUS");
        }break;
        case LE:{
            strcpy(val,"LE");
        }break;
        case LT:{
            strcpy(val,"LT");
        }break;
        case GE:{
            strcpy(val,"GE");
        }break;
        case GT:{
            strcpy(val,"GT");
        }break;
        case EQ:{
            strcpy(val,"EQ");
        }break;
        case NE:{
            strcpy(val,"NE");
        }break;
        case AND:{
            strcpy(val,"AND");
        }break;
        case OR:{
            strcpy(val,"OR");
        }break;
        case MUL:{
            strcpy(val,"MUL");
        }break;
        case DIV:{
            strcpy(val,"DIV");
        }break;
        case SWITCH:{
            strcpy(val,"SWITCH");
        }break;
        case CASE:{
            strcpy(val,"CASE");
        }break;
        case BREAK:{
            strcpy(val,"BREAK");
        }break;
        case DEFAULT:{
            strcpy(val,"DEFAULT");
        }break;
        case FOR:{
            strcpy(val,"FOR");
        }break;
        case IN:{
            strcpy(val,"IN");
        }break;
        case WHILE:{
            strcpy(val,"WHILE");
        }break;
        case DOLLAR:{
            strcpy(val,"$");
        }break;
        case ERROR:{
            strcpy(val,"ERROR");
        }break;
        case PROGRAM:{
            strcpy(val,"PROGRAM");
        }break;
        case SYN:{
            strcpy(val,"SYN");
        }break;
        case caseSingle:{
            strcpy(val,"caseSingle");
        }break;
        case LABEL:{
            strcpy(val,"LABEL");
        }break;
        case GOTO:{
            strcpy(val,"GOTO");
        }break;
        case uminus:{
            strcpy(val,"uminus");
        }break;
        case GOTOIFTRUE:{
            strcpy(val,"GOTOIFTRUE");
        }break;
        case GOTOIFFALSE:{
            strcpy(val,"GOTOIFFALSE");
        }break;
        case RETURN:{
            strcpy(val,"RETURN");
        }break;
        case INIT:{
            strcpy(val,"INIT");
        }break;
        case CALL:{
            strcpy(val,"CALL");
        }break;
        case TYPE:{
            strcpy(val,"TYPE");
        }break;
        case DYARR:{
            strcpy(val,"DYARR");
        }break;
        case STATARR:{
            strcpy(val,"STATARR");
        }break;
        case ASSIGNARR:{
            strcpy(val,"ASSIGNARR");
        }break;
        case ADDESPMOD:{
            strcpy(val,"ADDESPMOD");
        }break;
        case SUBESPMOD:{
            strcpy(val,"SUBESPMOD");
        }break;
        case ACTUAL_OUT_PARAMS:{
            strcpy(val,"ACTUAL_OUT_PARAMS");
        }break;
        case ACTUAL_IN_PARAMS:{
            strcpy(val,"ACTUAL_IN_PARAMS");
        }break;
        case FORMAL_IN_PARAMS:{
            strcpy(val,"FORMAL_IN_PARAMS");
        }break;
        case FORMAL_OUT_PARAMS:{
            strcpy(val,"FORMAL_OUT_PARAMS");
        }break;
        case POP_ACTUAL_IN_PARAMS:{
            strcpy(val,"POP_ACTUAL_IN_PARAMS");
        }break;
        case POP_ACTUAL_OUT_PARAMS:{
            strcpy(val,"POP_ACTUAL_OUT_PARAMS");
        }break;
        case INP_ARRAY_CHECK:{
            strcpy(val,"INP_ARRAY_CHECK");
        }break;
        case EXIT:{
            strcpy(val,"EXIT");
        }break;
        case DINT:{
            strcpy(val,"DINT");
        }break;
        case DREAL:{
            strcpy(val,"DREAL");
        }break;
        case DBOOL:{
            strcpy(val,"DBOOL");
        }break;
        case UNASSIGNED:{
            strcpy(val,"UNASSIGNED");
        }break;
        case RUNTIME_TYPE_ERROR:{
            strcpy(val,"RUNTIME_TYPE_ERROR");
        }break;

    }

    return val;

}


int computehash(char* str){
    ll n = strlen(str),mod = 1e9 +7,po = 1e6 + 3,cur = 0, ans = 0;
    ll i = 0;
    for( i = 0 ; i < n ; i++){
        ll j = str[i];
        cur = (cur * po)+ j;
        if(cur>=mod)cur%=mod;
    }
    cur%=N;
    return cur;
}

void addentry(char* ptr, Term val, hashmap* mappingTable){
    ll f = computehash(ptr);
    if(mappingTable[f].str==NULL){
        mappingTable[f].str =(char*) malloc(strlen(ptr)*sizeof(char)+1);
        mappingTable[f].value =val;
        strcpy(mappingTable[f].str,ptr);
        return;
    }
    else{
        if(mappingTable[f].next==NULL ){
            mappingTable[f].next = (hashmap*)malloc(sizeof(hashmap));
            hashmap* tmp=mappingTable[f].next;
            tmp->next=NULL;
            tmp->str =(char*) malloc(strlen(ptr)*sizeof(char)+1);
            strcpy(tmp->str,ptr);
            tmp->value=val;
        }
        else{
            hashmap* tmp = mappingTable[f].next;
            while(1){
                if(tmp->next==NULL )break;
                tmp = tmp->next;
            }
            tmp -> next = (hashmap*)malloc(sizeof(hashmap));
            tmp = tmp->next;
            tmp->next=NULL;
            tmp->str =(char*) malloc(strlen(ptr)*sizeof(char)+1);
            strcpy(tmp->str,ptr);
            tmp->value=val;
        }
    }
}

int fin(char* str, hashmap* mappingTable){
    int ha = computehash(str);
    if(mappingTable[ha].str==NULL)return 0;
    if(mappingTable[ha].next==NULL){
        return 1;
    }
    hashmap* tmp = &mappingTable[ha];
    while(tmp){
        if(strcmp(tmp->str,str)==0){
        return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

hashmap* findthe(char* str, hashmap* mappingTable){
    int ha = computehash(str);
    hashmap* ans  = (hashmap*)malloc(sizeof(hashmap));
    if(mappingTable[ha].str==NULL)return ans;
    if(mappingTable[ha].next==NULL){
        ans = &mappingTable[ha];
                return ans;
    }
    hashmap* tmp = &mappingTable[ha];
    while(tmp){
        if(strcmp(tmp->str,str)==0){
        return tmp;
        }
        tmp = tmp->next;
    }
    return tmp;
}
void init(hashmap* mappingTable){
    ll i=0;
    for(i=0;i<N;i++){
        mappingTable[i].str = NULL ;
        mappingTable[i].value = 0;
        mappingTable[i].next = NULL;
    }
}

void initialize_lookup_map1(hashmap * mappingTable){
    init(mappingTable);
    addentry("integer",INTEGER,mappingTable);
    addentry("real",REAL,mappingTable);
    addentry("boolean",BOOLEAN,mappingTable);
    addentry("of",OF,mappingTable);
    addentry("array",ARRAY,mappingTable);
    addentry("start",START,mappingTable);
    addentry("declare",DECLARE,mappingTable);
    addentry("module",MODULE,mappingTable);
    addentry("end",END,mappingTable);
    addentry("driver",DRIVER,mappingTable);
    addentry("program",PROGRAM,mappingTable);
    addentry("get_value",GET_VALUE,mappingTable);
    addentry("print",PRINT,mappingTable);
    addentry("use",USE,mappingTable);
    addentry("with",WITH,mappingTable);
    addentry("parameters",PARAMETERS,mappingTable);
    addentry("true",TRUE,mappingTable);
    addentry("false",FALSE,mappingTable);
    addentry("takes",TAKES,mappingTable);
    addentry("input",INPUT,mappingTable);
    addentry("returns",RETURNS,mappingTable);
    addentry("AND",AND,mappingTable);
    addentry("OR",OR,mappingTable);
    addentry("for",FOR,mappingTable);
    addentry("in",IN,mappingTable);
    addentry("switch",SWITCH,mappingTable);
    addentry("case",CASE,mappingTable);
    addentry("break",BREAK,mappingTable);
    addentry("default",DEFAULT,mappingTable);
    addentry("while",WHILE,mappingTable);
    addentry("ERROR",ERROR,mappingTable);  
}
void initialize_lookup_map2(hashmap * mappingTable){
    addentry("program",program, mappingTable);
    addentry("moduleDeclarations",moduleDeclarations,mappingTable);
    addentry("otherModules",otherModules,mappingTable);
    addentry("driverModule",driverModule ,mappingTable);
    addentry("moduleDeclaration",moduleDeclaration,mappingTable);
    addentry("module",module,mappingTable);
    addentry("moduleDef",moduleDef,mappingTable);
    addentry("input_plist",input_plist,mappingTable);
    addentry("ret",ret,mappingTable);
    addentry("N1",N1,mappingTable);
    addentry("output_plist",output_plist,mappingTable);
    addentry("N2",N2,mappingTable);
    addentry("dataType",dataType,mappingTable);
    addentry("type",type,mappingTable);
    addentry("range_arrays",range_arrays,mappingTable);
    addentry("index",index_,mappingTable);
    addentry("statements",statements,mappingTable);
    addentry("statement",statement,mappingTable);
    addentry("ioStmt",ioStmt,mappingTable);
    addentry("simpleStmt",simpleStmt,mappingTable);
    addentry("declareStmt",declareStmt,mappingTable);
    addentry("conditionalStmt",conditionalStmt,mappingTable);
    addentry("iterativeStmt",iterativeStmt,mappingTable);
    addentry("var",var,mappingTable);
    addentry("whichId",whichId,mappingTable);
    addentry("boolConstt",boolConstt,mappingTable);
    addentry("var_id_num",var_id_num,mappingTable);
    addentry("assignmentStmt",assignmentStmt,mappingTable);
    addentry("moduleReuseStmt",moduleReuseStmt,mappingTable);
    addentry("whichStmt",whichStmt,mappingTable);
    addentry("lvalueIDStmt",lvalueIDStmt,mappingTable);
    addentry("lvalueARRStmt",lvalueARRStmt,mappingTable);
    addentry("expression",expression,mappingTable);
    addentry("optional",optional,mappingTable);
    addentry("idList",idList,mappingTable);
    addentry("N3",N3,mappingTable);
    addentry("arithmeticOrBooleanExpr",arithmeticOrBooleanExpr,mappingTable);
    addentry("U",U,mappingTable);
    addentry("unary_op",unary_op,mappingTable);
    addentry("new_NT",new_NT,mappingTable);
    addentry("arithmeticExpr",arithmeticExpr,mappingTable);
    addentry("AnyTerm",AnyTerm,mappingTable);
    addentry("N7",N7,mappingTable);
    addentry("logicalOp",logicalOp,mappingTable);
    addentry("N8",N8,mappingTable);
    addentry("relationalOp",relationalOp,mappingTable);
    addentry("op1",op1,mappingTable);
    addentry("op2",op2,mappingTable);
    addentry("term",term,mappingTable);
    addentry("N4",N4,mappingTable);
    addentry("factor",factor,mappingTable);
    addentry("N5",N5,mappingTable);
    addentry("caseStmts",caseStmts,mappingTable);
    addentry("value",value,mappingTable);
    addentry("N9",N9,mappingTable);
    addentry("default",default_,mappingTable);
    addentry("range",range,mappingTable);
    addentry("e",e,mappingTable);
    addentry("DECLARE",DECLARE,mappingTable);
    addentry("MODULE",MODULE,mappingTable);
    addentry("ID",ID,mappingTable);
    addentry("SEMICOL",SEMICOL,mappingTable);
    addentry("DEF",DEF,mappingTable);
    addentry("DRIVER",DRIVER,mappingTable);
    addentry("PROGRAM",PROGRAM,mappingTable);
    addentry("ENDDEF",ENDDEF,mappingTable);
    addentry("DRIVERDEF",DRIVERDEF,mappingTable);
    addentry("DRIVERENDDEF",DRIVERENDDEF,mappingTable);
    addentry("TAKES",TAKES,mappingTable);
    addentry("INPUT",INPUT,mappingTable);
    addentry("SQBO",SQBO,mappingTable);
    addentry("SQBC",SQBC,mappingTable);
    addentry("RETURNS",RETURNS,mappingTable);
    addentry("COMMA",COMMA,mappingTable);
    addentry("COLON",COLON,mappingTable);
    addentry("INTEGER",INTEGER,mappingTable);
    addentry("REAL",REAL,mappingTable);
    addentry("BOOLEAN",BOOLEAN,mappingTable);
    addentry("ARRAY",ARRAY,mappingTable);
    addentry("OF",OF,mappingTable);
    addentry("RANGEOP",RANGEOP,mappingTable);
    addentry("START",START,mappingTable);
    addentry("END",END,mappingTable);
    addentry("GET_VALUE",GET_VALUE,mappingTable);
    addentry("BO",BO,mappingTable);
    addentry("BC",BC,mappingTable);
    addentry("PRINT",PRINT,mappingTable);
    addentry("NUM",NUM,mappingTable);
    addentry("RNUM",RNUM,mappingTable);
    addentry("TRUE",TRUE,mappingTable);
    addentry("FALSE",FALSE,mappingTable);
    addentry("ASSIGNOP",ASSIGNOP,mappingTable);
    addentry("USE",USE,mappingTable);
    addentry("WITH",WITH,mappingTable);
    addentry("PARAMETERS",PARAMETERS,mappingTable);
    addentry("PLUS",PLUS,mappingTable);
    addentry("MINUS",MINUS,mappingTable);
    addentry("LE",LE,mappingTable);
    addentry("LT",LT,mappingTable);
    addentry("GE",GE,mappingTable);
    addentry("GT",GT,mappingTable);
    addentry("EQ",EQ,mappingTable);
    addentry("NE",NE,mappingTable);
    addentry("AND",AND,mappingTable);
    addentry("OR",OR,mappingTable);
    addentry("MUL",MUL,mappingTable);
    addentry("DIV",DIV,mappingTable);
    addentry("SWITCH",SWITCH,mappingTable);
    addentry("CASE",CASE,mappingTable);
    addentry("BREAK",BREAK,mappingTable);
    addentry("DEFAULT",DEFAULT,mappingTable);
    addentry("FOR",FOR,mappingTable);
    addentry("IN",IN,mappingTable);
    addentry("WHILE",WHILE,mappingTable);
    addentry("$",DOLLAR,mappingTable);
    addentry("SYN",SYN,mappingTable);
    addentry("ERROR",ERROR,mappingTable);     
    addentry("caseSingle",caseSingle,mappingTable);
}

Term stringToEnum(char*str, hashmap* mappingTable){
    if(fin(str,mappingTable)){
        return findthe(str,mappingTable)->value;
    }
    else{
        return findthe("ERROR",mappingTable)->value;
    }
}