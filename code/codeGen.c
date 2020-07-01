// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH



/*Quadruple           |               Op   |     arg1   |    arg2   |   result
----------------------|--------------------|----------- |-----------------------
t1 :=- c              |            uminus  |     c      |           |   t1
t2 :=b op t1          |             op     |    b       |   t1      |   t2
a := t5               |           ASSIGNOP |     t5     |     -     |    a
Label:                |            LABEL   | labelname  |           |
GOTO:                 |             GOTO   |            | labelname |
t := A[i]             |            ARRAY   |    A       |    i      |    t
if res goto label     |          GOTOIFTRUE|    res     | label     |
CALL function         |            CALL    |  function  |    -      |    -
*/
    


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "utils.h"
#include "lexerDef.h"
#include "parserDef.h"
#include "symbol.h"
#include "ast.h"
#include "semanticAnalyzer.h"
#include "intermediateCode.h"
#include "codeGen.h"
int hasRunTimeError =0;
int ne_count;
int inp_count =1, out_count=1 ;

void codeGeneration(quadruple *head, char *outfile){
    ne_count = 0;
    FILE * out  = fopen(outfile,"w");
    fprintf(out, "true equ 1\n");
    fprintf(out, "false equ 0\n\n");

    fprintf(out,"%%macro push_all_reg 0\n");
    fprintf(out,"\tpush rax\n");
    fprintf(out,"\tpush rcx\n");
    fprintf(out,"\tpush rdx\n");
    fprintf(out,"\tpush rbx\n");
    fprintf(out,"\tpush rsp\n");
    fprintf(out,"\tpush rbp\n");
    fprintf(out,"\tpush rsi\n");
    fprintf(out,"\tpush rdi\n");
    fprintf(out,"%%endmacro\n\n");

    fprintf(out,"%%macro pop_all_reg 0\n");
    fprintf(out,"\tpop rdi\n");
    fprintf(out,"\tpop rsi\n");
    fprintf(out,"\tpop rbp\n");
    fprintf(out,"\tpop rsp\n");
    fprintf(out,"\tpop rbx\n");
    fprintf(out,"\tpop rdx\n");
    fprintf(out,"\tpop rcx\n");
    fprintf(out,"\tpop rax\n");
    fprintf(out,"%%endmacro\n\n");

    fprintf(out,"%%macro align_16 0\n");
    fprintf(out,"\tmov rbx, rsp\n");
    fprintf(out,"\tand rsp, -16\n");
    fprintf(out,"%%endmacro\n\n");

    fprintf(out,"%%macro realign_16 0\n");
    fprintf(out,"\tmov rsp, rbx\n");
    fprintf(out,"%%endmacro\n\n");


    fprintf(out, "SECTION .text\n");
    fprintf(out,"\tglobal main\n");
    fprintf(out,"\textern scanf\n");
    fprintf(out,"\textern printf\n");
    
    fprintf(out, "SECTION .data\n");
    fprintf(out, "\tinputFormatstr: db \"%%d\",0\n");
    fprintf(out, "\toutputFormatstr: db \"Output: %%d\", 0\n");
    fprintf(out, "\toutputLineno: db \"LINE NO. : %%d :\", 0\n");
    fprintf(out, "\tarrayOutputFormatstr: db \"%%d \", 0\n");
    fprintf(out, "\tarrayInputFormatstr: db \"%%d \", 0\n"); //Enter 5 array elements of integer type for range 6 to 10
    fprintf(out, "\ttruemsg: db \"true\",0\n");
    fprintf(out, "\tfalsemsg: db \"false\",0\n");
    fprintf(out, "\tnewline: db \"\",10, 0\n");
    fprintf(out, "\tfmt: db \"Output: %%s\", 0\n");
    fprintf(out, "\tbarrayOutputFormatstr: db \"%%s \", 0\n");
    fprintf(out, "\tinpArrayMsg: db \"Input: Enter %%d array elements of %%s type for range %%d to %%d:\",10, 0\n");
    fprintf(out, "\tinpbArrayMsg: db \"Input: Enter %%d array elements of %%s type ( 0 for false and 1 for true) for range %%d to %%d:\",10, 0\n");
    fprintf(out, "\tinputMsg: db \"Input: Enter an integer value: \", 10, 0\n");
    fprintf(out, "\tbinputMsg: db \"Input: Enter a boolean value: \", 10, 0\n");
    fprintf(out, "\tOutputMsg: db \"Output: \", 0\n");
    fprintf(out, "\tintstr: db \"integer\", 0\n");
    fprintf(out, "\trealstr: db \"real\", 0\n");
    fprintf(out, "\tboolstr: db \"boolean\", 0\n");
    fprintf(out, "\tErrormsg: db \"RUN TIME ERROR:  Array index out of bound\", 10, 0\n");
    fprintf(out, "\tErrormsgType: db \"RUN TIME ERROR:  Type mismatch error\", 10, 0\n");
    fprintf(out, "\tExceptionmsg: db \"EXCEPTION: Divide by 0\", 10, 0\n\n");

    quadruple *q = head;
    while(q!=NULL){
        
        
        switch(q->op){
            
            case PLUS: {    

                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL, *res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            if(idt2!=NULL)
                                en2 = lookupidST(idt2,arg2);
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");
                
                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tadd rax, rbx\n");
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tadd rax, %s\n", arg2);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tadd rax, %s\n", arg1);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tadd rax, %s\n", arg2);
                            }
            
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
            
                }break;
            
            case MINUS: {
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            if(idt2!=NULL)
                                en2 = lookupidST(idt2,arg2);
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");
                
                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tsub rax, rbx\n");
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tsub rax, %s\n", arg2);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tsub rax, %s\n", arg1);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tsub rax, %s\n", arg2);
                            }
            
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
            
                }break;
            
            case MUL: {
                            char *arg1 = q->arg1;
                            char * arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            if(idt2!=NULL)
                                en2 = lookupidST(idt2,arg2);
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");
                
                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\timul rax, rbx\n");
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\timul rax, %s\n", arg2);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\timul rax, %s\n", arg1);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\timul rax, %s\n", arg2);
                            }
            
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
            
                }break;
              case DIV: {
                           char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            if(idt2!=NULL)
                                en2 = lookupidST(idt2,arg2);
                    
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");
                
                            int exit = newLabel();
                              
                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmov edx, 0\n");           
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rbx,0\n");
                                fprintf(out,"\tje l%d\n",exit );
                                fprintf(out, "\tidiv rbx\n");
                               
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmov edx, 0\n");           
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmov rbx, %s\n", arg2);
                                fprintf(out, "\tcmp rbx,0\n");
                                fprintf(out,"\tje l%d\n",exit );
                                fprintf(out, "\tidiv rbx\n");
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmov edx, 0\n");           
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tcmp rax,0\n");
                                fprintf(out,"\tje l%d\n",exit );
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rbx,0\n");
                                fprintf(out,"\tje l%d\n",exit );
                                fprintf(out, "\tidiv rbx\n");

                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov edx, 0\n");           
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tmov rbx, %s\n", arg2);
                                fprintf(out, "\tcmp rbx,0\n");
                                fprintf(out,"\tje l%d\n",exit );
                                fprintf(out, "\tidiv rbx\n");
                            }
            
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            int newl = newLabel();
                            fprintf(out,"\tjmp l%d\n",newl);
                            fprintf(out, "l%d:\n",exit);
                            fprintf(out, "\tmov rdi, Exceptionmsg\n");//print divide by 0 exception
                            fprintf(out, "\tpush_all_reg\n");
                            fprintf(out, "\tmov rax, 0\n");
                            fprintf(out, "\talign_16\n");
                            fprintf(out, "\tcall printf\n");
                            fprintf(out, "\trealign_16\n");
                            fprintf(out, "\tpop_all_reg\n");

                            fprintf(out, "\tmov rdi, 0\n");//exit from program
                            fprintf(out, "\tmov rax, 60\n");
                            fprintf(out, "\tsyscall\n");

                            fprintf(out, "l%d:\n",newl);
                            fprintf(out, "\tpop_all_reg\n");
                }break;
        case ASSIGNOP: {

                            char *arg1 = q->arg1;
                            idST *idt1 = q->idt1;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            
                            if(en1!=NULL && res!=NULL && en1->isArray==1 && res->isArray==1){

                                if(en1->isStatic == 0 || res->isStatic == 0){

                                    if(en1->start_pop ==1){

                                        int start1 = en1->start.num;
                                        fprintf(out,"\tmov rax, %d\n",start1);
                                    }
                                    else{
                                        idSTEntry * begin  = en1->start.entry;
                                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n",begin->offset*8);
                                    }

                                     if(res->start_pop ==1){

                                        int start2 = res->start.num;
                                        fprintf(out,"\tmov rbx, %d\n",start2);
                                    }
                                    else{
                                        idSTEntry * begin  = res->start.entry;
                                        fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n",begin->offset*8);
                                    }

                                    fprintf(out, "\tcmp rbx, rax\n");
                                    int l1 = newLabel();
                                    fprintf(out, "\tjne dtypecheck%d\n", l1);
                                    int l2 = newLabel();
                                    fprintf(out, "\tjmp dtypecheck%d\n", l2);
                                    fprintf(out, "dtypecheck%d:\n", l1);

                                    int lineno = q->nodeResult->unode.ln.t.lineno; 
                                    fprintf(out, "\tmov rdi, outputLineno\n"); //Print dynamic type error line no
                                    fprintf(out, "\tmov rsi, %d\n",lineno);
                                    fprintf(out, "\tpush_all_reg\n");
                                    fprintf(out, "\tmov rax, 0\n");
                                    fprintf(out, "\talign_16\n");
                                    fprintf(out, "\tcall printf\n");
                                    fprintf(out, "\trealign_16\n");
                                    fprintf(out, "\tpop_all_reg\n");

                                    fprintf(out, "\tmov rdi, ErrormsgType\n");//print dynamic type error
                                    fprintf(out, "\tpush_all_reg\n");
                                    fprintf(out, "\tmov rax, 0\n");
                                    
                                    fprintf(out, "\talign_16\n");
                                    fprintf(out, "\tcall printf\n");
                                    fprintf(out, "\trealign_16\n");
                                    fprintf(out, "\tpop_all_reg\n");

                                    fprintf(out, "\tmov rdi, 0\n"); //exit after run-time error
                                    fprintf(out, "\tmov rax, 60\n");
                                    fprintf(out, "\tsyscall\n");

                                    fprintf(out, "dtypecheck%d:\n", l2);
                                    if(en1->end_pop ==1){

                                        int end1 = en1->end.num;
                                        fprintf(out,"\tmov rax, %d\n",end1);
                                    }
                                    else{
                                        idSTEntry * end  = en1->end.entry;
                                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n",end->offset*8);
                                    }

                                     if(res->end_pop ==1){

                                        int end2 = res->end.num;
                                        fprintf(out,"\tmov rbx, %d\n",end2);
                                    }
                                    else{
                                        idSTEntry * end  = res->end.entry;
                                        fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n",end->offset*8);
                                    }

                                    fprintf(out, "\tcmp rbx, rax\n");
                                    int l3 = newLabel();
                                    fprintf(out, "\tjne dtypecheck%d\n", l3);
                                    int l4 = newLabel();
                                    fprintf(out, "\tjmp dtypecheck%d\n", l4);
                                    fprintf(out, "dtypecheck%d:\n", l3);
 
                                    fprintf(out, "\tmov rdi, outputLineno\n"); //Print dynamic type error line no
                                    fprintf(out, "\tmov rsi, %d\n",lineno);
                                    fprintf(out, "\tpush_all_reg\n");
                                    fprintf(out, "\tmov rax, 0\n");
                                    fprintf(out, "\talign_16\n");
                                    fprintf(out, "\tcall printf\n");
                                    fprintf(out, "\trealign_16\n");
                                    fprintf(out, "\tpop_all_reg\n");

                                    fprintf(out, "\tmov rdi, ErrormsgType\n");//print dynamic type error
                                    fprintf(out, "\tpush_all_reg\n");
                                    fprintf(out, "\tmov rax, 0\n");
                                    
                                    fprintf(out, "\talign_16\n");
                                    fprintf(out, "\tcall printf\n");
                                    fprintf(out, "\trealign_16\n");
                                    fprintf(out, "\tpop_all_reg\n");

                                    fprintf(out, "\tmov rdi, 0\n"); //exit after run-time error
                                    fprintf(out, "\tmov rax, 60\n");
                                    fprintf(out, "\tsyscall\n");

                                    fprintf(out, "dtypecheck%d:\n", l4);

                                }
                                fprintf(out, "\tpush_all_reg\n");
                                fprintf(out, "\tmov rax, QWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                                fprintf(out, "\tpop_all_reg\n");

                            }
                            else{
                                if(en1!=NULL){
                                    fprintf(out, "\tpush_all_reg\n");
                                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                    fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                                    fprintf(out, "\tpop_all_reg\n");
                                    
                                }
                                else if(en1==NULL){
                                    fprintf(out, "\tmov QWORD[rbp-8-%d], %s\n", res->offset*8,arg1);
                                }
                            }
                            
                }break;
            case ASSIGNARR: {
                            fprintf(out, "\tpush_all_reg\n");
                            int offset = -1;
                            idSTEntry * arr =  lookupidST(q->resultIdt,q->result);

                            int width = 0;
                            if(arr->dataType==INTEGER){
                                width = 2;
                            }
                            else if(arr->dataType==BOOLEAN){
                                width =1;
                            }
                            else{
                                width =4;
                            }
                            if(arr->isStatic ==1){
                                offset = arr->offset + 1;
                                fprintf(out, "\tmov rbx , QWORD[rbp-8-%d]\n",arr->offset*8);
                                int start = arr->start.num;
                                fprintf(out,"\tmov rax, %d\n",start); 
                            }
                            else{
                                fprintf(out, "\tmov rbx , QWORD[rbp-8-%d]\n",arr->offset*8);
                                if(arr->start_pop ==1){

                                    int start = arr->start.num;
                                    fprintf(out,"\tmov rax, %d\n",start);
                                }
                                else{
                                    idSTEntry * begin  = arr->start.entry;
                                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n",begin->offset*8);
                                }
                            }   

                            if(q->idt1 == NULL){
                                fprintf(out, "\tsub rax , %s\n",q->arg1);
                                fprintf(out, "\tneg rax\n");
                            }
                            else{
                                idSTEntry * index = lookupidST(q->idt1,q->arg1);
                                if(index == NULL){
                                    printf("ERROR UNDECLARED\n");
                                }
                                fprintf(out, "\tmovsxd rcx, DWORD[rbp-8-%d]\n",index->offset*8);
                                fprintf(out, "\tsub rax,rcx\n");
                                fprintf(out, "\tneg rax\n");
                            }
                            
                            fprintf(out, "\timul rax,%d\n",width);
                            idST *idt2 = q->idt2;
                            idSTEntry * en1= NULL;
                            if(idt2!=NULL)
                                en1 = lookupidST(idt2,q->arg2);
                            
                                if(en1!=NULL){
                                    fprintf(out, "\timul rax, 8\n");
                                    fprintf(out, "\tsub rbx, rax\n");
                                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                    fprintf(out, "\tmov QWORD[rbx], rax\n");
                                }
                                else if(en1==NULL){
                                    fprintf(out, "\timul rax, 8\n");
                                    fprintf(out, "\tsub rbx, rax\n");
                                    fprintf(out, "\tmov QWORD[rbx], %s\n",q->arg2);
                                }

                                fprintf(out, "\tpop_all_reg\n");

                }break;
            case ARRAY:{
                        fprintf(out, "\tpush_all_reg\n");
                        char* arg1 = q->arg1;
                        idSTEntry * arr =  lookupidST(q->idt1,arg1);
                        int offset =0;
                        int width = 0;
                        if(arr->dataType==INTEGER){
                            width = 2;
                        }
                        else if(arr->dataType==BOOLEAN){
                            width =1;
                        }
                        else if(arr->dataType == REAL){
                            width =4;
                        }

                        if(arr->isStatic ==1){
                                offset = arr->offset + 1;
                                fprintf(out, "\tmov rbx , QWORD[rbp-8-%d]\n",arr->offset*8);
                                int start = arr->start.num;
                                fprintf(out,"\tmov rax, %d\n",start); 
                            }
                        else{
                            fprintf(out, "\tmov rbx , QWORD[rbp-8-%d]\n",arr->offset*8);
                            if(arr->start_pop ==1){

                                int start = arr->start.num;
                                fprintf(out,"\tmov rax, %d\n",start);
                                 //????
                            }
                            else{
                                idSTEntry * begin  = arr->start.entry;
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n",begin->offset*8);
                            }
                        }
                        if(q->idt2 == NULL){
                                fprintf(out, "\tsub rax , %s\n",q->arg2);
                                fprintf(out, "\tneg rax\n");
                            }
                        else{
                            idSTEntry * index = lookupidST(q->idt2,q->arg2);
                            if(index == NULL){
                                printf("ERROR UNDECLARED\n");
                            }
                            fprintf(out, "\tmovsxd rcx, DWORD[rbp-8-%d]\n",index->offset*8);
                            fprintf(out, "\tsub rax,rcx\n");
                            fprintf(out, "\tneg rax\n");
                        }

                        idSTEntry * temp = lookupidST(q->resultIdt,q->result);

                            fprintf(out, "\timul rax, %d\n",width * 8);
                            fprintf(out, "\tsub rax, rbx\n");
                            fprintf(out, "\tneg rax\n" );
                            fprintf(out, "\tmovsxd rbx, DWORD[rax]\n");
                            fprintf(out, "\tmov QWORD[rbp-8-%d],rbx\n",temp->offset*8);
                    
                        fprintf(out, "\tpop_all_reg\n");  

                        

        }break;
           case LABEL: {
                            char *arg1 = q->arg1;
                            char *modname = "123driver";
                            if(strcmp(arg1,modname)==0)
                                fprintf(out, "main:\n");
                            else
                                fprintf(out, "%s:\n",arg1);
                }break;
            case GOTO: {
                            char *arg1 = q->arg2;
                            fprintf(out, "\tjmp %s\n",arg1);
                }break;
        case GOTOIFTRUE:{

                            char* arg1 = q->arg1;
                            char* label = q->arg2;
                            idST *idt1 = q->idt1;
                            idSTEntry * en1= NULL;

                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                            fprintf(out, "\tpush_all_reg\n");

                            if(en1!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out,"\tcmp rax, 1\n"); 
                                fprintf(out, "\tpop_all_reg\n");
                                fprintf(out,"\tje %s\n",label);
                            }
                            else{
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out,"\tcmp rax, 1\n"); //decimal 1
                                fprintf(out, "\tpop_all_reg\n");
                                fprintf(out,"\tje %s\n",label);
                            }
                            
                }break;
                case NE:{
                            
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            if(idt2!=NULL)
                                en2 = lookupidST(idt2,arg2);
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");

                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, rbx\n");
                                fprintf(out, "\tjne not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjne not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg1);
                                fprintf(out, "\tjne not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjne not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }

                            ne_count++;
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
                }break;
        case GOTOIFFALSE:{
                            
                            char* arg1 = q->arg1;
                            char* label = q->arg2;
                            idST *idt1 = q->idt1;
                            idSTEntry * en1= NULL;

                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                            fprintf(out, "\tpush_all_reg\n");

                            if(en1!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out,"\tcmp rax, 0\n"); //decimal 0
                                fprintf(out, "\tpop_all_reg\n");
                                fprintf(out,"\tje %s\n",label);
                            }
                            else{
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out,"\tcmp rax, 0\n"); //decimal 0
                                fprintf(out, "\tpop_all_reg\n");
                                fprintf(out,"\tje %s\n",label);
                            }
                            
                }break;

            case uminus:{

                            char * arg1 = q->arg1;
                            idST *idt1 = q->idt1;
                            idSTEntry * en1= NULL,*res;

                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);

                            idST *residt = q->resultIdt;
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");

                            if(en1!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tneg rax\n");
                            }
                            else{
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tneg rax\n");
                            }

                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");

                }break;

            case CALL:{
                            char* arg1 = q->arg1;
                            fprintf(out, "\tcall %s\n",arg1);

            }break;

            case INIT:{     
                            fprintf(out, "\tpush rbp\n");
                            fprintf(out, "\tmov rbp, rsp\n");
                
            }break;

        case ADDESPMOD:{    //Grow stack by adding module size to rsp
                            moduleST * mt = getmt();
                            char *arg1 = q->arg1;
                            moduleSTEntry *mod = lookupmoduleST(mt,arg1);
                            int sz = mod->moduleSize;
                            char buffer[20];
                            sprintf(buffer, "%d", sz*8);
                            
                            if(strcmp(mod->lexeme_mod,"123driver") ==0)
                                fprintf(out, "main_exit:\n");
                            else{
                                fprintf(out, "%s_exit:\n",mod->lexeme_mod);  
                            }

                            fprintf(out, "\tmov rsp, rbp\n");
            
                            
                            

            }break;

        case SUBESPMOD:{    //Grow stack by adding module size to rsp
                            moduleST * mt = getmt();
                            char *arg1 = q->arg1;
                            moduleSTEntry *mod = lookupmoduleST(mt,arg1);
                            int sz = mod->moduleSize;
                            char buffer[20];
                            sprintf(buffer, "%d", sz*8);
                            fprintf(out, "\tsub rsp, %s\n",buffer);
                            
                            

            }break;

case ACTUAL_OUT_PARAMS:{    //Push actual output parameters to stack
                            char *arg1 = q->arg1;
                            idST *idt1 = q->idt1;
                            
                            idSTEntry * en1= NULL;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);

                            fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                            fprintf(out, "\tpush rax\n");
            

                            
                }break;
case POP_ACTUAL_OUT_PARAMS:{    //Extract returned values from function
                            
                            fprintf(out, "\tpop rax\n");
                            char *arg1 = q->arg1;
                            idST *idt1 = q->idt1;
                            
                            idSTEntry * en1= NULL;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);

                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", en1->offset*8);
                            
                            
            }break;

case ACTUAL_IN_PARAMS:{    //Push actual input parameters to stack
                            char *arg1 = q->arg1;
                            idST *idt1 = q->idt1;
                            
                            idSTEntry * en1= NULL;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);

                            if(en1->isArray == 0 && q->arg2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tpush rax\n");
                            }

                            else if(en1->isArray == 1 && q->arg2==NULL){

                                if(en1->isStatic == 1){
                                    fprintf(out, "\tmov rax, %d\n", en1->end.num);  //push end index
                                    fprintf(out, "\tpush rax\n");
                                    fprintf(out, "\tmov rax, %d\n", en1->start.num);    //push start index
                                    fprintf(out, "\tpush rax\n");

                                }

                                else if(en1->start_pop == 1 && en1->end_pop == 0){
                                    idSTEntry * end  = en1->end.entry;
                                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n",end->offset*8);
                                    fprintf(out, "\tpush rax\n");
                                    fprintf(out, "\tmov rax, %d\n", en1->start.num);
                                    fprintf(out, "\tpush rax\n");


                                }

                                else if(en1->start_pop == 0 && en1->end_pop == 1){
                                    idSTEntry * begin  = en1->start.entry;
                                    fprintf(out, "\tmov rax, %d\n", en1->end.num);
                                    fprintf(out, "\tpush rax\n");
                                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", begin->offset*8);
                                    fprintf(out, "\tpush rax\n"); 
                                    
                                }

                                else if(en1->start_pop == 0 && en1->end_pop == 0){
                                    idSTEntry * begin  = en1->start.entry;
                                    idSTEntry * end  = en1->end.entry;
                                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", end->offset*8);
                                    fprintf(out, "\tpush rax\n");
                                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", begin->offset*8);
                                    fprintf(out, "\tpush rax\n");
                                    
                                }

                                fprintf(out, "\tmov rax, QWORD[rbp-8-%d]\n", en1->offset*8); //array base address
                                fprintf(out, "\tpush rax\n");

                            }
                            else if(strcmp(q->arg2,"array")==0){
                               
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", (en1->offset+3)*8); //array base address
                                fprintf(out, "\tpush rax\n");
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", (en1->offset+1)*8); //array base address
                                fprintf(out, "\tpush rax\n");
                                fprintf(out, "\tmov rax, QWORD[rbp-8-%d]\n", en1->offset*8); //array base address
                                fprintf(out, "\tpush rax\n");
                            }

                            
            }break;
case POP_ACTUAL_IN_PARAMS:{    
                                // pop all input params;
                            char *arg1 = q->arg1;
                            idST *idt1 = q->idt1;
                            
                            idSTEntry * en1= NULL;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);

                                fprintf(out, "\tpop rax\n");
                            
            }break;

case FORMAL_OUT_PARAMS:{    //Push actual output parameters to stack
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2; //module name
                            idST *idt1 = q->idt1;
                            
                            
                            moduleST * mt = getmt();
                            moduleSTEntry *mod = lookupmoduleST(mt,arg2);
                            int count_par =0,count_inpar=0;
                            pNode *tmp = mod->outParams;
                            while(tmp!=NULL)
                            {
                                count_par++;
                                tmp = tmp->next;
                            }   

                            tmp = mod->inParams;
                            while(tmp!=NULL)
                            {
                                count_inpar++;
                                tmp = tmp->next;
                            }   

                            idSTEntry * en1= NULL;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);

                            fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                            fprintf(out, "\tmov QWORD[rbp+ 8 + %d + %d], rax\n", count_inpar*8, out_count*8);
                            
            
                            if(count_par == out_count)
                                out_count = 1;
                            else out_count++;
            

                            
                }break;
case FORMAL_IN_PARAMS:{    //Extract input parameter values from stack
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2; //module name
                            idST *idt1 = q->idt1;
                            
                            
                            moduleST * mt = getmt();
                            moduleSTEntry *mod = lookupmoduleST(mt,arg2);
                            int count_par =1 ;
                            pNode *tmp = mod->inParams;
                            while(tmp!=NULL)
                            {
                                if(tmp->isArray ==1 )
                                    count_par +=3;
                                else
                                    count_par++;
                                tmp = tmp->next;
                            }   

                            idSTEntry * en1= NULL;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);

                            if(en1->isArray == 0){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp+8+%d]\n", inp_count*8); //+1??
                                fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", en1->offset*8);
                            }

                            else{

                                fprintf(out, "\tmov rax, QWORD[rbp+8+%d]\n", inp_count*8); //get base address of array
                                fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rax, DWORD[rbp+8+%d]\n", (inp_count+1)*8); //get start index
                                fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", (en1->offset+1)*8);
                                fprintf(out, "\tmovsxd rax, DWORD[rbp+8+%d]\n", (inp_count+2)*8); //get end index
                                fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", (en1->offset+3)*8);
                            }
            
                            

                            if(en1->isArray == 1)
                                inp_count += 3;
                            
                            else
                                inp_count++;


                            if(count_par == inp_count)
                                inp_count = 1;
                            

                            
                }break;

            case GT:{
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL){
                                en1 = lookupidST(idt1,arg1);
                            }
                
                            if(idt2!=NULL){
                                en2 = lookupidST(idt2,arg2);
                            }
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");
                
                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, rbx\n");
                                fprintf(out, "\tjg not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjg not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg1);
                                fprintf(out, "\tjl not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjg not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }

                            ne_count++;
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
            }break;
        case GE:{
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL){
                                en1 = lookupidST(idt1,arg1);
                            }
                
                            if(idt2!=NULL){
                                en2 = lookupidST(idt2,arg2);
                            }
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");

                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, rbx\n");
                                fprintf(out, "\tjge not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjge not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg1);
                                fprintf(out, "\tjle not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjge not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }

                            ne_count++;

                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
            }break;
        case LE:{
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            if(idt2!=NULL)
                                en2 = lookupidST(idt2,arg2);
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");
                
                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, rbx\n");
                                fprintf(out, "\tjle not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjle not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg1);
                                fprintf(out, "\tjge not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjle not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }

                            ne_count++;
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
            }break;
        case LT:{
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            if(idt2!=NULL)
                                en2 = lookupidST(idt2,arg2);
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");
                
                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, rbx\n");
                                fprintf(out, "\tjl not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjl not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg1);
                                fprintf(out, "\tjg not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tjl not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }

                            ne_count++;
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
            }break;

        case EQ:{
                            char *arg1 = q->arg1;
                            char *arg2 = q->arg2;
                            idST *idt1 = q->idt1;
                            idST *idt2 = q->idt2;
                            idST *residt = q->resultIdt;
                            
                            idSTEntry * en1= NULL, *en2 = NULL,*res;
                            if(idt1!=NULL)
                                en1 = lookupidST(idt1,arg1);
                
                            if(idt2!=NULL)
                                en2 = lookupidST(idt2,arg2);
                
                            char *result = q->result;
                            res = lookupidST(residt,result);
                            fprintf(out, "\tpush_all_reg\n");
                
                            if(en1!=NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, rbx\n");
                                fprintf(out, "\tje not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                                
                            }
                            else if(en1!=NULL && en2==NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tje not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
                
                            else if(en1==NULL && en2!=NULL){
                                fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                                fprintf(out, "\tcmp rax, %s\n", arg1);
                                fprintf(out, "\tje not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }
            
                             else if(en1==NULL && en2==NULL){ 
                                fprintf(out, "\tmov rax, %s\n", arg1);
                                fprintf(out, "\tcmp rax, %s\n", arg2);
                                fprintf(out, "\tje not_equal%d\n",ne_count);
                                fprintf(out, "\tjmp equal%d\n",ne_count);
                                fprintf(out, "not_equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 1\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "equal%d:\n",ne_count);
                                fprintf(out, "\tmov rax, 0\n");
                                fprintf(out, "\tjmp end%d\n",ne_count);
                                fprintf(out, "end%d:\n",ne_count);
                            }

                            ne_count++;
                            fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                            fprintf(out, "\tpop_all_reg\n");
            }break;
        case OR:{ 
                    char *arg1 = q->arg1;
                    char *arg2 = q->arg2;
                    idST *idt1 = q->idt1;
                    idST *idt2 = q->idt2;
                    idST *residt = q->resultIdt;
                    
                    idSTEntry * en1= NULL, *en2 = NULL, *res;
                    if(idt1!=NULL)
                        en1 = lookupidST(idt1,arg1);
        
                    if(idt2!=NULL)
                        en2 = lookupidST(idt2,arg2);
        
                    char *result = q->result;
                    res = lookupidST(residt,result);
                    fprintf(out, "\tpush_all_reg\n");
        
                    if(en1!=NULL && en2!=NULL){
                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                        fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                        fprintf(out, "\tor rax, rbx\n");
                        
                    }
                    else if(en1!=NULL && en2==NULL){
                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                        fprintf(out, "\tor rax, %s\n", arg2);
                    }
        
                    else if(en1==NULL && en2!=NULL){
                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                        fprintf(out, "\tor rax, %s\n", arg1);
                    }
    
                     else if(en1==NULL && en2==NULL){ 
                        fprintf(out, "\tmov rax, %s\n", arg1);
                        fprintf(out, "\tor rax, %s\n", arg2);
                    }
    
                    fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                    fprintf(out, "\tpop_all_reg\n");
    

        }break;
        case AND:{

                    char *arg1 = q->arg1;
                    char *arg2 = q->arg2;
                    idST *idt1 = q->idt1;
                    idST *idt2 = q->idt2;
                    idST *residt = q->resultIdt;
                    
                    idSTEntry * en1= NULL, *en2 = NULL, *res;
                    if(idt1!=NULL)
                        en1 = lookupidST(idt1,arg1);
        
                    if(idt2!=NULL)
                        en2 = lookupidST(idt2,arg2);
        
                    char *result = q->result;
                    res = lookupidST(residt,result);
                    fprintf(out, "\tpush_all_reg\n");
        
                    if(en1!=NULL && en2!=NULL){
                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                        fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en2->offset*8);
                        fprintf(out, "\tand rax, rbx\n");
                        
                    }
                    else if(en1!=NULL && en2==NULL){
                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en1->offset*8);
                        fprintf(out, "\tand rax, %s\n", arg2);
                    }
        
                    else if(en1==NULL && en2!=NULL){
                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                        fprintf(out, "\tand rax, %s\n", arg1);
                    }
    
                     else if(en1==NULL && en2==NULL){ 
                        fprintf(out, "\tmov rax, %s\n", arg1);
                        fprintf(out, "\tand rax, %s\n", arg2);
                    }
    
                    fprintf(out, "\tmov QWORD[rbp-8-%d], rax\n", res->offset*8);
                    fprintf(out, "\tpop_all_reg\n");

            }break;


        case RETURN:{
                        fprintf(out, "\tpop rbp\n");
                        fprintf(out, "\t%s\n\n", "ret");
            }break;

        case PRINT:{
                        //Do for Array Case, Floating point
                        char* arg1 = q->arg1;
                        idST *idt1 = q->idt1;
                        int width = 0;
                        idSTEntry * en1= NULL;
                        int arr = 0;
                        int looplabel = newLabel();
                        int endlabel = newLabel();
                        if(idt1!=NULL){
                            en1 = lookupidST(idt1,arg1);
                            arr = en1->isArray;
                        }
                        fprintf(out, "\tpush_all_reg\n");
                        if(en1!=NULL){
                            int offset = en1->offset;
                            if(arr == 1){ //arrayCase
                                offset = offset + 1;
                                fprintf(out, "\tmov rdi, OutputMsg\n");
                                fprintf(out, "\tpush_all_reg\n");
                                fprintf(out, "\tmov rax, 0\n");
                                
                                fprintf(out, "\talign_16\n");
                                fprintf(out, "\tcall printf\n");
                                fprintf(out, "\trealign_16\n");
                                fprintf(out, "\tpop_all_reg\n");
                                

                                if(en1->start_pop==1){
                                    int start = en1->start.num; 
                                    fprintf(out,"\tmov rax, %d\n",start);
                                }
                                else{
                                    idSTEntry * begin  = en1->start.entry;
                                    fprintf(out, "\tmovsxd rax , DWORD[rbp-8-%d]\n",begin->offset*8);
                                }
                                if(en1->end_pop==1){
                                    int end =  en1->end.num;
                                    fprintf(out,"\tmov rbx, %d\n",end);
                                }
                                else{
                                    idSTEntry * end  = en1->end.entry;
                                    fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n",end->offset*8);
                                }
                                
                                fprintf(out, "\tsub rbx,rax\n");
                                fprintf(out, "\tadd rbx,1\n");
                            }
                            else{
                                fprintf(out, "\tmov rbx,1\n");
                            }

                            if(arr==0 ){
                                fprintf(out, "\tmov rcx, %d\n",offset*8);
                                fprintf(out, "\tadd rcx,8\n");
                                fprintf(out, "\tsub rcx,rbp\n");
                                fprintf(out, "\tneg rcx\n");
                            }
                            else {
                                fprintf(out, "\tmov rcx, QWORD[rbp-8-%d]\n",en1->offset*8);
                            }
                            
                            
                            fprintf(out, "l%d: \n",looplabel);
                            fprintf(out, "\tcmp rbx,0\n");
                            fprintf(out, "\tje l%d\n",endlabel);

                            if(arr == 1){
                                fprintf(out, "\tmov rdi, arrayOutputFormatstr\n");
                            }
                            else{
                                fprintf(out, "\tmov rdi, outputFormatstr\n");
                            }
                        
                            /*
                            cmp    ax, bx      
                             jl     Less  
                             mov    word [X], 1    
                             jmp    Both            
                            Less: 
                             mov    word [X], -1  
                            Both:

                            */
                            
                            if(en1->dataType == REAL)
                                width = 4;
                            else if(en1->dataType == INTEGER)
                                width = 2;
                            else
                                width = 1;
                           

                            if(en1->dataType == BOOLEAN){
                                if(en1->isArray == 0)
                                    fprintf(out, "\tmov rdi,fmt\n");
                                else
                                    fprintf(out, "\tmov rdi,barrayOutputFormatstr\n");
                                fprintf(out, "\tmov rax, QWORD[rcx]\n");
                                fprintf(out, "\tcmp al,0 \n");
                                int l1 = newLabel();
                                int l2 = newLabel();
                                fprintf(out, "\tjne  l%d\n",l1 );
                                fprintf(out, "\tmov rsi,falsemsg\n");
                                fprintf(out, "\tjmp l%d\n",l2 );
                                fprintf(out, "l%d:\n",l1 );
                                fprintf(out, "\tmov rsi,truemsg\n");
                                fprintf(out, "l%d:\n",l2 );
                            }
                            else{
                                fprintf(out, "\tmov rsi, QWORD[rcx]\n");
                            }
                        }


                    else{
                        fprintf(out, "\tmov rdi, outputFormatstr\n");
                        fprintf(out, "\tmov rsi, %s\n",arg1);
                    }

                    fprintf(out, "\tpush_all_reg\n");
                    fprintf(out, "\tmov rax, 0\n");
                    
                    fprintf(out, "\talign_16\n");
                    fprintf(out, "\tcall printf\n");
                    fprintf(out, "\trealign_16\n");
                    fprintf(out, "\tpop_all_reg\n");
                    
                    fprintf(out, "\tsub rbx,1\n");
                    fprintf(out, "\tsub rcx,%d\n",width*8);
                    if(en1!=NULL){
                    fprintf(out, "\tjmp l%d\n",looplabel);
                    fprintf(out, "l%d: \n",endlabel);
                    }

                    fprintf(out, "\tmov rdi, newline\n");
                    fprintf(out, "\tpush_all_reg\n");
                    fprintf(out, "\tmov rax, 0\n");
                    fprintf(out, "\talign_16\n");
                    fprintf(out, "\tcall printf\n");
                    fprintf(out, "\trealign_16\n");
                    fprintf(out, "\tpop_all_reg\n");

                    fprintf(out, "\tpop_all_reg\n");


            }break;

        case GET_VALUE:{
                        
                        char* arg1 = q->arg1;
                        idST *idt1 = q->idt1;
                        int looplabel = newLabel();
                            int endlabel = newLabel();  
                        idSTEntry * en1= NULL;
                    
                        if(idt1!=NULL)
                            en1 = lookupidST(idt1,arg1);
                        if(en1->isArray ==0){
                            if(en1->dataType == INTEGER)
                                fprintf(out, "\tmov rdi, inputMsg\n");
                            else if(en1->dataType == BOOLEAN)
                                fprintf(out, "\tmov rdi, binputMsg\n");
                            fprintf(out, "\tpush_all_reg\n");

                            fprintf(out, "\tmov rax, 0\n");
                            
                            fprintf(out, "\talign_16\n");
                            fprintf(out, "\tcall printf\n");
                            fprintf(out, "\trealign_16\n");
                            fprintf(out, "\tpop_all_reg\n");
                            fprintf(out, "\tmov rdi, inputFormatstr\n");
                            fprintf(out, "\tlea rsi, [rbp-8-%d]\n",en1->offset*8);
                            fprintf(out, "\tpush_all_reg\n");
                            fprintf(out, "\talign_16\n");
                            fprintf(out, "\tmov rax, 0\n");
                            fprintf(out, "\tcall scanf\n");
                            fprintf(out, "\trealign_16\n");
                            fprintf(out, "\tpop_all_reg\n");
                        }   
                        else{

                            fprintf(out, "\tpush_all_reg\n");

                            int width =0;
                            if(en1->dataType == REAL)
                                width = 4;
                            else if(en1->dataType == INTEGER)
                                width = 2;
                            else
                                width = 1;
                            
                            if(en1->start_pop==1){
                                int start = en1->start.num; 
                                fprintf(out,"\tmov rax, %d\n",start);
                            }
                            else{
                                idSTEntry * begin  = en1->start.entry;
                                fprintf(out, "\tmovsxd rax , DWORD[rbp-8-%d]\n",begin->offset*8);
                            }
                            if(en1->end_pop==1){
                                int end =  en1->end.num;
                                fprintf(out,"\tmov rbx, %d\n",end);
                            }
                            else{
                                idSTEntry * end  = en1->end.entry;
                                fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n",end->offset*8);
                            }

                            fprintf(out, "\tmov r8, rbx\n");
                            fprintf(out, "\tsub rbx,rax\n");
                            fprintf(out, "\tadd rbx,1\n");
                            if(en1->dataType == INTEGER || en1->dataType == REAL)
                                fprintf(out, "\tmov rdi, inpArrayMsg\n"); //message for input array
                            else
                                fprintf(out, "\tmov rdi, inpbArrayMsg\n"); //message for input array
                            fprintf(out, "\tmov rsi, rbx\n");

                            if(en1->dataType == REAL)
                                fprintf(out, "\tmov rdx,  realstr\n");
                            else if(en1->dataType == INTEGER)
                                fprintf(out, "\tmov rdx,  intstr\n");
                            else
                                fprintf(out, "\tmov rdx,  boolstr\n");

                            
                            fprintf(out, "\tmov rcx, rax \n");
                            fprintf(out, "\tmov r8, r8\n");
                            fprintf(out, "\tpush_all_reg\n");
                            fprintf(out, "\tmov rax,0\n" );
                            
                            fprintf(out, "\talign_16\n");
                            fprintf(out, "\tcall printf\n");
                            fprintf(out, "\trealign_16\n");
                            fprintf(out, "\tpop_all_reg\n");


                            fprintf(out, "\tmov rcx, QWORD[rbp-8-%d]\n",en1->offset*8);


                            fprintf(out, "l%d: \n",looplabel);
                            fprintf(out, "\tcmp rbx,0\n");
                            fprintf(out, "\tje l%d\n",endlabel);
                            fprintf(out, "\tpush_all_reg\n");

                            fprintf(out, "\tmov rdi, inputFormatstr\n");
                            fprintf(out, "\tlea rsi, [rcx]\n");

                            fprintf(out, "\tpush_all_reg\n");
                            fprintf(out, "\tmov rax, 0\n");
                            
                            fprintf(out, "\talign_16\n");
                            fprintf(out, "\tcall scanf\n");
                            fprintf(out, "\trealign_16\n");
                            fprintf(out, "\tpop_all_reg\n");
                            fprintf(out, "\tpop_all_reg\n");
                            fprintf(out, "\tsub rbx,1\n");
                            fprintf(out, "\tsub rcx,%d\n",width*8);
                            fprintf(out, "\tjmp l%d\n",looplabel);
                            fprintf(out, "l%d: \n",endlabel);
                            fprintf(out, "\tpop_all_reg\n");
                            
                        }
                        
                    
            
            }break;


        case TYPE:{
                        if(strcmp(q->arg1,"ERROR") ==0){ 

                           char *lineno = q->result; 
                           fprintf(out, "\tmov rdi, outputLineno\n"); //Print dynamic type error line no
                           fprintf(out, "\tmov rsi, %s\n",lineno);
                           fprintf(out, "\tpush_all_reg\n");
                           fprintf(out, "\tmov rax, 0\n");
                           fprintf(out, "\talign_16\n");
                           fprintf(out, "\tcall printf\n");
                           fprintf(out, "\trealign_16\n");
                           fprintf(out, "\tpop_all_reg\n");

                           fprintf(out, "\tmov rdi, Errormsg\n");//PRINT DYNAMIC BOUND ERROR
                           fprintf(out, "\tpush_all_reg\n");
                           fprintf(out, "\tmov rax, 0\n");
                           fprintf(out, "\talign_16\n");
                           fprintf(out, "\tcall printf\n");
                           fprintf(out, "\trealign_16\n");
                           fprintf(out, "\tpop_all_reg\n");
                        }

                        else{
                            idSTEntry* var = lookupidST(q->idt2,q->arg2);
                            var->dataType = stringToEnum(q->arg1,genericMappingTable);
                        }
                        
        }break;
        
        case DYARR:{
                    char *arg1 = q->arg1;
                    char *arg2 = q->arg2;
                    idST *idt1 = q->idt1;
                    idST *idt2 = q->idt2;
                    idST *residt = q->resultIdt;
                    
                    idSTEntry * en1= NULL, *en2 = NULL,*res;
                    idST * e1,*e2;
                    if(idt1!=NULL){
                        e1 = lookUpID(arg1,idt1);
                        en1 = lookupidST(e1,arg1);
                    }
        
                    if(idt2!=NULL){
                        e2 = lookUpID(arg2,idt2);
                        en2 = lookupidST(e2,arg2);
                    }
        
                    char *result = q->result;
                    res = lookupidST(residt,result);
                    int width =0;
                            if(res->dataType == REAL)
                                width = 4;
                            else if(res->dataType == INTEGER)
                                width = 2;
                            else
                                width = 1;
        
                    if(en1!=NULL && en2!=NULL){
                        fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en1->offset*8);

                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                        fprintf(out, "\tsub rax, rbx\n");
                        fprintf(out, "\tadd rax, 1\n" );
                        fprintf(out, "\timul rax,%d\n",width *8);
                        // decimal
                        fprintf(out, "\tmov rbx,rsp\n");
                        fprintf(out, "\tsub rbx,8\n");
                        fprintf(out, "\tmov QWORD[rbp-8-%d], rbx\n",res->offset*8);
                        fprintf(out, "\tsub rsp,rax\n");
                        
                    }
                    else if(en1!=NULL && en2==NULL){
                        fprintf(out, "\tmovsxd rbx, DWORD[rbp-8-%d]\n", en1->offset*8);
                        fprintf(out, "\tmov rax, %s\n", arg2);
                        fprintf(out, "\tsub rax, rbx\n");
                        fprintf(out, "\tadd rax, 1\n" );
                        fprintf(out, "\timul rax,%d\n",width *8);
                        fprintf(out, "\tmov rbx,rsp\n");
                        fprintf(out, "\tsub rbx,8\n");
                        fprintf(out, "\tmov QWORD[rbp-8-%d], rbx\n",res->offset*8);
                        fprintf(out, "\tsub rsp,rax\n");
                    }
        
                    else if(en1==NULL && en2!=NULL){
                        fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n", en2->offset*8);
                        fprintf(out, "\tmov rbx, %s\n", arg1);
                        fprintf(out, "\tsub rax, rbx\n");
                        fprintf(out, "\tadd rax, 1\n" );
                        fprintf(out, "\timul rax,%d\n",width * 8);
                        fprintf(out, "\tmov rbx,rsp\n");
                        fprintf(out, "\tsub rbx,8\n");
                        fprintf(out, "\tmov QWORD[rbp-8-%d], rbx\n",res->offset*8);
                        fprintf(out, "\tsub rsp,rax\n");
                    }
    
        }break;

         case STATARR:{

                    char *result = q->result;
                    idST *residt = q->resultIdt;
                    idSTEntry * res = lookupidST(residt,result);
                    int width =0;
                    if(res->dataType == REAL)
                        width = 4;
                    else if(res->dataType == INTEGER)
                        width = 2;
                    else
                        width = 1;
                    fprintf(out, "\tpush_all_reg\n");
                    fprintf(out, "\tmov rbx, rbp\n");
                    fprintf(out, "\tsub rbx, %d\n",8*(res->offset +2));
                    fprintf(out, "\tmov QWORD[rbp-8-%d], rbx\n",res->offset*8); 
                    fprintf(out, "\tpop_all_reg\n");


        }break;

        case INP_ARRAY_CHECK:{
                    char *arrayname = q->arg1; 
                    idST *idt1 = q->idt1;
                    char *index = q->arg2; //index
                    idST *idt2 = q->idt2;

                    idSTEntry * arridt = lookupidST(idt1,arrayname);

                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n",8*(arridt->offset +1)); //start
                    if(idt2 == NULL){
                        fprintf(out, "\tmov rbx , %s\n",q->arg2);
                    }
                    else{
                        idSTEntry * index = lookupidST(q->idt2,q->arg2);
                        if(index == NULL){
                            printf("ERROR UNDECLARED\n");
                        }
                        fprintf(out, "\tmovsxd rbx , DWORD[rbp-8-%d]\n",index->offset*8); //index
                    }

                    fprintf(out, "\tcmp rbx, rax\n");
                    int l1 = newLabel();
                    fprintf(out, "\tjge cmp_array%d\n", l1);
                    int l2 = newLabel();
                    fprintf(out, "\tjmp cmp_array%d\n", l2);
                    fprintf(out, "cmp_array%d:\n", l1);

                    fprintf(out, "\tmovsxd rax, DWORD[rbp-8-%d]\n",8*(arridt->offset +3));
                    fprintf(out, "\tcmp rbx, rax\n");
                    int l3 = newLabel();
                    fprintf(out, "\tjle cmp_array%d\n", l3);
                    fprintf(out, "\tjmp cmp_array%d\n", l2);
                    fprintf(out, "cmp_array%d:\n", l3);
                    int l4 = newLabel();
                    fprintf(out, "\tjmp cmp_array%d\n", l4);

                    fprintf(out, "cmp_array%d:\n", l2);

                    char *lineno = q->result; 
                    fprintf(out, "\tmov rdi, outputLineno\n"); //Print dynamic type error line no
                    fprintf(out, "\tmov rsi, %s\n",lineno);
                    fprintf(out, "\tpush_all_reg\n");
                    fprintf(out, "\tmov rax, 0\n");
                    fprintf(out, "\talign_16\n");
                    fprintf(out, "\tcall printf\n");
                    fprintf(out, "\trealign_16\n");
                    fprintf(out, "\tpop_all_reg\n");

                    fprintf(out, "\tmov rdi, Errormsg\n");//PRINT DYNAMIC BOUND ERROR
                    fprintf(out, "\tpush_all_reg\n");
                    fprintf(out, "\tmov rax, 0\n");
                    
                    fprintf(out, "\talign_16\n");
                    fprintf(out, "\tcall printf\n");
                    fprintf(out, "\trealign_16\n");
                    fprintf(out, "\tpop_all_reg\n");

                    fprintf(out, "\tmov rdi, 0\n"); //exit after run-time error
                    fprintf(out, "\tmov rax, 60\n");
                    fprintf(out, "\tsyscall\n");

                    fprintf(out, "cmp_array%d:\n", l4);

        }break;

        case EXIT:{
                    //exit because run time error
                    fprintf(out, "\tmov rdi, 0\n");
                    fprintf(out, "\tmov rax, 60\n");
                    fprintf(out, "\tsyscall\n");

        }break;
        case RUNTIME_TYPE_ERROR:{
                    //exit because run time type error
                     char *lineno = q->arg1; 
                     fprintf(out, "\tmov rdi, outputLineno\n"); //Print dynamic type error line no
                     fprintf(out, "\tmov rsi, %s\n",lineno);
                     fprintf(out, "\tpush_all_reg\n");
                     fprintf(out, "\tmov rax, 0\n");
                     fprintf(out, "\talign_16\n");
                     fprintf(out, "\tcall printf\n");
                     fprintf(out, "\trealign_16\n");
                     fprintf(out, "\tpop_all_reg\n");
                    
                     fprintf(out, "\tmov rdi, ErrormsgType\n"); //Print dynamic type error
                     fprintf(out, "\tpush_all_reg\n");
                     fprintf(out, "\tmov rax, 0\n");
                     fprintf(out, "\talign_16\n");
                     fprintf(out, "\tcall printf\n");
                     fprintf(out, "\trealign_16\n");
                     fprintf(out, "\tpop_all_reg\n");
                    
                     fprintf(out, "\tmov rdi, 0\n");
                     fprintf(out, "\tmov rax, 60\n");
                     fprintf(out, "\tsyscall\n");

        }break;
      }
        q = q->next;
    }
    fclose(out);
}