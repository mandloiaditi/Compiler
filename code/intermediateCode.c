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
GOTO:                 |             GOTO   | labelname  |           |
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
int temp_labels=0, new_labels=0;


int newTemp(){      //generates new temporary variable
    
    temp_labels ++;
    return temp_labels-1;
}
    
int newLabel(){
    new_labels++;
    return new_labels-1;

}
        
quadruple * createQuadruple(){
    
    quadruple*  new =(quadruple*)malloc(sizeof(quadruple));
    new->arg1 = NULL;
    new->arg2 = NULL;
    new->result = NULL;
    new->op = ERROR;
    new->node1 = NULL;
    new->node2 = NULL;
    new->nodeResult = NULL;
    new->idt1 = NULL;
    new->idt2 = NULL;
    new->resultIdt = NULL;
    new->next = NULL;
    return new;
}
        
quadruple * addToEnd( quadruple * head, quadruple * node){
    quadruple * temp = head;
    
    if(temp == NULL && node != NULL) {
        return node;
    }
    if(node == NULL && head != NULL){
        return head;
    }
    if(temp == NULL && node == NULL)
        return NULL;
    
    while(temp->next!=NULL){
     temp = temp->next;
    }
    temp->next = node;
    return head;
}

  
void printQuadruple(quadruple *head){

    while(head!=NULL){
       printf("operator:%s\n", enumToString(head->op));
       printf("arg1:%s\n", head->arg1);
       printf("arg2:%s\n", head->arg2);
       printf("result:%s\n", head->result);
       printf("\n");
       head=head->next;
    }  
    
    return;
      
}


quadruple * generateIntermediateCodeHelper(quadruple *head, astNode *root, moduleSTEntry * min){
    
    if(root==NULL) {
        return head;
    }
 
    if(root->isLeaf == 0 && root->unode.nt.symbol == declareStmt){
        head = addToEnd(head,IRcodeDeclareStmt(root,min));
    }
    else if(root->isLeaf == 0 && root->unode.nt.symbol == moduleReuseStmt){
        head = addToEnd(head,IRcodeModuleReuse(root,min));
    }

    else if(root->isLeaf == 0 && root->unode.nt.symbol == conditionalStmt){
        head = IRcodeConditional(root,head,min);
        return head;
    }

    else if(root->isLeaf == 0 && root->unode.nt.symbol == iterativeStmt){
        head = addToEnd(head,IRcodeIterative(root,min));
        return head;
    }
    
    else if(root->isLeaf == 0 && root->unode.nt.symbol == assignmentStmt){
        astNode *temp = root->child;
        int lineno = temp->unode.ln.t.lineno ;
        head = addToEnd(head,IRcodeAssign(root,min));

        for(int i=0; i<errorIndex; i++){

            if(errArr[i].line == lineno){
                quadruple *typeerr = createQuadruple();
                typeerr->op = RUNTIME_TYPE_ERROR;
                char buff[10];
                typeerr->arg1 = (char *)malloc(sizeof(char) * 30);
                sprintf(buff, "%d", lineno);
                strcpy(typeerr->arg1,buff);
                head = addToEnd(head, typeerr);
                break;
            }
        }
    }
    
    else if(root->isLeaf == 0 && root->unode.nt.symbol == ioStmt){
        head = addToEnd(head,IRcodeIoStmt(root,min));
    
    }

    if(root == NULL)
    return head; 
    
    head = generateIntermediateCodeHelper(head,root->child,min);
    astNode* temp=root->child;
    
    if(temp!=NULL)
        temp=temp->sibling;
    while(temp != NULL){
        head = generateIntermediateCodeHelper(head, temp,min);
        temp=temp->sibling;
    }

    return head;
}


        
quadruple *IRcodeIoStmt(astNode *node,moduleSTEntry * min){
    quadruple *io = createQuadruple(),*arr = NULL;
    Term t;
    astNode *temp;
    char * check;
    int l1 = newLabel();
    if(node->ruleNum == 0){ //GET_VALUE code
        t = GET_VALUE;
        io->op = t;
        temp =  node->child; //ID  //handle arrays?
        check =  temp->unode.ln.t.lexeme;
        io->arg1 = check;
        io->node1 = temp;
        io->idt1 = temp->idt;
    }
    
    else if(node->ruleNum == 1){ //PRINT code
        t = PRINT;
        io->op = t;
        temp =  node->child;
        

        if(temp->isLeaf == 1){
            check =  temp->unode.ln.t.lexeme;
            io->arg1 = check;
            io->node1 = temp;
            
        }
        
        else if(temp->isLeaf == 0 && temp->unode.nt.symbol == var_id_num){ //ID<whichIDio> case
            astNode *temp2 = temp->child;

            if(temp2->sibling!=NULL){
                arr = IRcodeVarArrayCase(temp,min,l1);
                
                quadruple * temp = arr;
                while(temp != NULL && temp->next!= NULL){
                    temp = temp->next;
                }
              
                io->arg1 = temp->result;
                io->idt1 = temp->resultIdt;
                quadruple* q3 = createQuadruple();
                char * arg1 = (char *)malloc(sizeof(char) * 30);
                sprintf(arg1, "l%d",l1);
                q3->arg1  = arg1;
                q3->op = LABEL;
                io= addToEnd(io,q3);
                
            }

            else{

                io->arg1 = temp2->unode.ln.t.lexeme;
                io->node1 = temp2;
                io->idt1 = temp2->idt;
            }
        }

        io = addToEnd(arr, io);
        
    
    }
    
    return io;

}  
  
quadruple *IRcodeVarArrayCase(astNode *node,moduleSTEntry * min, int l){
    
    //bound & type checking
    int l1 = newLabel();
    int l2 = newLabel();
    int l3 = newLabel();
    int l4 = newLabel();
    int l5 = newLabel();
    
    quadruple * head = NULL;
    quadruple *arr = createQuadruple();
    
    astNode *temp = node->child; //ID
    astNode *arraynode = temp;
    char * check =  temp->unode.ln.t.lexeme;
    idST* current_idt = temp->idt;
    idSTEntry * search = lookupidST(temp->idt,check);
    astNode *temp3 = NULL ;
    if(temp->sibling->isLeaf ==1)
        temp3= temp->sibling;//NUM/ID
    else if(temp->sibling->unode.nt.symbol == lvalueARRStmt){
        temp3 =temp->sibling->child;
    }

    int paraFlag = 0;
    pNode *in = min->inParams;
        while(in != NULL){
            if(strcmp(in->lexeme,check)==0){
                paraFlag = 1;
                break;
            }
            
            in = in->next;
        }
  
    if(paraFlag == 0 && search->isArray==1 && (search->isStatic==0 || temp3->unode.ln.t.term==ID)){
        int t1 = newTemp();
        idSTEntry* ide1 = (idSTEntry*) malloc(sizeof(idSTEntry));
        sprintf(ide1->lexeme,"1t%d",t1);
        ide1->dataType = BOOLEAN;
        ide1->isArray = 0;
        ide1->width = 1;
        ide1->offset = min->moduleSize;
        min->moduleSize += ide1->width;
        current_idt = insertInidST(current_idt, ide1);
        quadruple * cond_var = createQuadruple(); 
        cond_var->arg1 = temp3->unode.ln.t.lexeme;
        cond_var->idt1 = temp3->idt;
        cond_var->node1 = temp3;

        if(search->start_pop ==0){
            cond_var->arg2 = search->start.entry->lexeme; 
            idST * allot = lookUpID(search->start.entry->lexeme,temp->idt);
            cond_var->idt2 = allot;
        }
        else{
            cond_var->arg2 = (char *)malloc(sizeof(char) * 30);
            sprintf(cond_var->arg2,"%d",search->start.num); 
            cond_var->idt2 = temp->idt ;
            cond_var->node2 = temp;
        }
        cond_var->result = (char *)malloc(sizeof(char) * 30);
        sprintf(cond_var->result,"1t%d",t1);
        cond_var->resultIdt = current_idt;
        cond_var->op = GE; 
        cond_var->node1 = temp3;

        head = addToEnd(head,cond_var);

        
        quadruple* q1 = createQuadruple();
        q1->arg1 = (char *)malloc(sizeof(char) * 30);
        sprintf(q1->arg1,"1t%d",t1);
        q1->idt1 = current_idt;
        q1->op = GOTOIFTRUE;
        q1->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(q1->arg2,"l%d",l1);
        head = addToEnd(head,q1);
        
        quadruple* q2 = createQuadruple();
        q2->op = GOTO;
        q2->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(q2->arg2,"l%d",l3);
        head = addToEnd(head,q2);
        
        quadruple* q3 = createQuadruple();
        char * arg1 = (char *)malloc(sizeof(char) * 30);
        sprintf(arg1, "l%d",l1);
        q3->arg1  = arg1;
        q3->op = LABEL;
        head = addToEnd(head,q3);
        
        int t2 = newTemp();
        idSTEntry* ide2 = (idSTEntry*) malloc(sizeof(idSTEntry));
        ide2->dataType = BOOLEAN;
        ide2->isArray = 0;
        ide2->width = 1;
        ide2->offset = min->moduleSize;
        min->moduleSize += ide2->width;

        sprintf(ide2->lexeme,"1t%d",t2);
        current_idt = insertInidST(current_idt, ide2);
        
        quadruple * cond_var2 = createQuadruple(); 
        cond_var2->arg1 = temp3->unode.ln.t.lexeme;
        cond_var2->resultIdt = current_idt;
        if(search->end_pop ==0){
            cond_var2->arg2 = search->end.entry->lexeme; 
            idST * allot = lookUpID(search->end.entry->lexeme,temp->idt);
            cond_var2->idt2 = allot;
        }
        else{
            cond_var2->arg2 = (char *)malloc(sizeof(char) * 30);
            sprintf(cond_var2->arg2,"%d",search->end.num); 
            cond_var2->idt2 = temp->idt ;
            cond_var2->node2 = temp;
        }
        cond_var2->result = (char *)malloc(sizeof(char) * 30);
        sprintf(cond_var2->result,"1t%d",t2);
        cond_var2->op = LE; 
        cond_var2->node1 = temp3;
        cond_var2->idt1 = temp3->idt;
        head = addToEnd(head,cond_var2);
        
        quadruple* q4 = createQuadruple();
        q4->arg1 = (char *)malloc(sizeof(char) * 30);
        sprintf(q4->arg1,"1t%d",t2);
        q4->idt1 = current_idt;
        q4->op = GOTOIFTRUE;
        q4->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(q4->arg2,"l%d",l2);
        head = addToEnd(head,q4);
        
        quadruple* q5 = createQuadruple();
        q5->op = GOTO;
        q5->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(q5->arg2,"l%d",l3);
        head = addToEnd(head,q5);
        
        quadruple* q6 = createQuadruple();
        char * arg11 = (char *)malloc(sizeof(char) * 30);
        sprintf(arg11, "l%d",l2);
        q6->arg1  = arg11;
        q6->op = LABEL;
        head = addToEnd(head,q6);
        
        quadruple* q7 = createQuadruple();
        q7->op = TYPE;
        q7->arg1 = (char *)malloc(sizeof(char) * 30);
        strcpy(q7->arg1,enumToString(search->dataType));
        q7->arg2 = check;
        q7->idt2 = temp->idt;
        q7->node2 = temp;
        head = addToEnd(head,q7);

        
        quadruple* q8 = createQuadruple();
        q8->op = GOTO;
        q8->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(q8->arg2,"l%d",l4);
        head = addToEnd(head,q8);
        
        quadruple* q9 = createQuadruple();
        char * arg111 = (char *)malloc(sizeof(char) * 30);
        sprintf(arg111, "l%d",l3);
        q9->arg1  = arg111;
        q9->op = LABEL;
        head = addToEnd(head,q9);
        
        quadruple* q10 = createQuadruple();
        q10->op = TYPE;
        q10->arg1 = (char *)malloc(sizeof(char) * 30);
        q10->result = (char *)malloc(sizeof(char) * 30);
        strcpy(q10->arg1,enumToString(ERROR));
        q10->arg2 = check;
        q10->idt2 = temp->idt;
        q10->node2 = temp;
        int lineno = temp3->unode.ln.t.lineno;
        char buff[10];
        sprintf(buff, "%d", lineno);
        strcpy(q10->result,buff);
        head = addToEnd(head,q10);
        quadruple* q12 = createQuadruple();
        q12->op = EXIT;
        q12->arg2 = (char *)malloc(sizeof(char) * 30);
        
        if(strcmp(min->lexeme_mod,"123driver") !=0)
            sprintf(q12->arg2,"main_exit");
        else{
            sprintf(q12->arg2,"main_exit");
        }
        
        head = addToEnd(head,q12);
        
        quadruple* q11 = createQuadruple();
        char * arg1111 = (char *)malloc(sizeof(char) * 30);
        sprintf(arg1111, "l%d",l4);
        q11->arg1  = arg1111;
        q11->op = LABEL;
        head = addToEnd(head,q11);
    }
    
    /*If (10>= m) goto L1
    goto L3
    L1: if (10<= n) goto L2
    goto L3
    L2: type1 = datatype of A
    goto L4
    L3: type1 = error
        printf("ERROR");
        goto L5
    L4: (t := A[10]), datatype bhi in t
    t2 = t1+B
    L5:
    */
    
  
    quadruple *param = createQuadruple();
    if(paraFlag == 1){
        
        param->op = INP_ARRAY_CHECK;
        param->arg1 = check;
        param->idt1 = arraynode->idt;
        param->node1 = arraynode;
        param->arg2 = temp3->unode.ln.t.lexeme;
        param->idt2 = temp3->idt;
        param->node1 = temp3;
     
        param->result = (char *)malloc(sizeof(char) * 30);
        int lineno = temp3->unode.ln.t.lineno;
        char buff[10];
        sprintf(buff, "%d", lineno);
        strcpy(param->result,buff);
        head = addToEnd(head,param);

        quadruple* q7 = createQuadruple();
        q7->op = TYPE;
        q7->arg1 = (char *)malloc(sizeof(char) * 30);
        strcpy(q7->arg1,enumToString(search->dataType));
        q7->arg2 = check;
        q7->idt2 = temp->idt;
        q7->node2 = temp;
        head = addToEnd(head,q7);
    
    }
    
    arr->arg1 = check;
    arr->node1 = temp;
    arr->idt1 = temp->idt;

    char* temp2 = (char *)malloc(sizeof(char) * 30);
    int temporary = newTemp();
    idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
    sprintf(ide->lexeme,"1t%d", temporary);
    ide->dataType = search->dataType;
    ide->isArray = 0;
    if(search->dataType == BOOLEAN)
        ide->width = 1;
    else if(search->dataType == INTEGER)
        ide->width =2;
    else
        ide->width =4;



    ide->offset = min->moduleSize;
    min->moduleSize += ide->width;
    current_idt = insertInidST(current_idt, ide);
    sprintf(temp2,"1t%d", temporary);
    arr->result = temp2;
    arr->resultIdt = current_idt;

    Term t = ARRAY;
    arr->op = t;
    
    
    arr->arg2 = temp3->unode.ln.t.lexeme;
    arr->node2 = temp3;

    if(temp3->unode.ln.t.term == ID)
        arr->idt2 = temp3->idt;
    head = addToEnd(head,arr);
    return head;
       
}

        
quadruple* IRcodeIterative(astNode * node, moduleSTEntry * min ){
    
        // &node
        //L1:
        //VAR1=i<n;
        //if(VAR1) goto L2;
        //goto L3;
        //L4: i++;
        //goto L1;
        //L2: VAR2=b+1;
        //a=VAR2;
    quadruple * head = NULL;
    if(node->ruleNum ==0){
      
        astNode* id = node->child;
        int start = id->sibling->child->unode.ln.t.value.v2;
        int end = id->sibling->child->sibling->unode.ln.t.value.v2;
        quadruple * loop_var = createQuadruple(); 
        loop_var->arg1 = (char *)malloc(sizeof(char) * 30);
        sprintf(loop_var->arg1, "%d",start);
        loop_var->result = id->unode.ln.t.lexeme;
        loop_var->op = ASSIGNOP; 
        loop_var->nodeResult = id;
        loop_var->resultIdt = id->idt;
        head = addToEnd(head,loop_var);
 
        char* temp2 = (char *)malloc(sizeof(char) * 30);
        int t2 = newTemp();
        idST* current_idt = id->idt;
        idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
        sprintf(ide->lexeme,"1t%d", t2);
        ide->dataType = BOOLEAN;
        ide->isArray = 0;
        ide->width = 1;

        ide->offset = min->moduleSize;
        min->moduleSize += ide->width;

        current_idt = insertInidST(current_idt, ide);
        sprintf(temp2,"1t%d", t2);
        int start_label = newLabel();
        quadruple * startlab = createQuadruple();
        startlab->arg1 = (char *)malloc(sizeof(char) * 30);
        sprintf(startlab->arg1, "l%d",start_label);
        startlab->op = LABEL;
        head = addToEnd(head,startlab);
        quadruple* bool = createQuadruple();
        bool->arg1 = id->unode.ln.t.lexeme;
        bool->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(bool->arg2, "%d",end);
        bool->result = temp2;
        bool->op = GT;
        bool->node1 = id;

        bool->resultIdt = current_idt;
        bool->idt1 = id->idt;

        head = addToEnd(head,bool);
        int endlabel = newLabel();
        char * arg1 = (char *)malloc(sizeof(char) * 30);
        sprintf(arg1, "l%d",endlabel);
        quadruple * endlab = createQuadruple();
        endlab->arg1  = arg1;
        endlab->op = LABEL;
        quadruple* check = createQuadruple();
        
        check->arg1 = temp2;
        check->idt1 = current_idt;

        check->op = GOTOIFTRUE; //ENUM
        check->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(check->arg2,"l%d",endlabel);
        head = addToEnd(head,check);
        head = IRcodeStmts(id->sibling->sibling,head,min);
        
        quadruple* increment = createQuadruple();
        increment->op = PLUS;
        increment->arg1 = id->unode.ln.t.lexeme;
        increment->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(increment->arg2,"%d",1);
        increment->result = id->unode.ln.t.lexeme;
        
        increment->node1 = id;
        increment->idt1 = id->idt;

        increment->nodeResult = id;
        increment->resultIdt = id->idt;
        head = addToEnd(head, increment);
       
        quadruple* gotostart = createQuadruple();
        gotostart->op = GOTO; 
        gotostart->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(gotostart->arg2,"l%d",start_label);
        head = addToEnd(head,gotostart);
       
        head = addToEnd(head,endlab);
        
    }
       /*
       S.begin := newlabel ;
       E.true := newlabel ;
       E.false := S.next ;
       S1.next := S.begin ;
       S.code := gen(S.begin ‘:’) || E.code || gen(E.true ‘:’) || S1.code || gen(‘goto’ S.begin)
       */
    else
       if(node->ruleNum== 1){
          int l1 = newLabel();
          int l2 = newLabel(); //truelabel
          int l3 = newLabel(); //falselabel
        
          char* arg1 = (char *)malloc(sizeof(char) * 30);
          sprintf(arg1, "l%d",l1);
          quadruple * repeat = createQuadruple();
          repeat->arg1  = arg1;
          repeat->op = LABEL;
          head = addToEnd(head,repeat);
          
          idST * idt  = travandfind(node->child) ;
          
          quadruple * answer;
          if(idt==NULL)
            answer = ariBoolCode(node->child,min->idt,min);
          
          else answer = ariBoolCode(node->child,idt,min);
          if(answer==NULL){
              answer = createQuadruple();
              answer->result = node->child->unode.ln.t.lexeme;
              if(node->child->unode.ln.t.term ==ID){
                  answer->resultIdt = node->child->idt;
              }
              
          }
          else{
            head = addToEnd(head, answer);
          }
          while(answer->next != NULL){
            answer = answer->next;
          }
            quadruple* check = createQuadruple();
            check->arg1 = answer->result;
            check->idt1 = answer->resultIdt;

            check->op = GOTOIFFALSE; 
            
            check->arg2 = (char *)malloc(sizeof(char) * 30);
            sprintf(check->arg2,"l%d",l3);
            head = addToEnd(head,check);
        
          char* arg2 = (char *)malloc(sizeof(char) * 30);
          sprintf(arg2, "l%d",l2);
          quadruple * labeltrue = createQuadruple();
          labeltrue->arg1  = arg2;
          labeltrue->op = LABEL;
          head = addToEnd(head,labeltrue);
          head = IRcodeStmts(node->child->sibling,head,min);
          
        
          quadruple* gotoq = createQuadruple();
          gotoq->op = GOTO; 
          gotoq->arg2 = repeat->arg1;
          gotoq->idt2 = repeat->idt1;
          head = addToEnd(head,gotoq);
        
          char* arg3 = (char *)malloc(sizeof(char) * 30);
          sprintf(arg3, "l%d",l3);
          quadruple * labelfalse = createQuadruple();
          labelfalse->arg1  = arg3;
          labelfalse->op = LABEL;
          head = addToEnd(head,labelfalse);

          return head;
              
     }
     return head;
}
      
idST *travandfind(astNode *node)
{
    if(node->isLeaf==1 && node->unode.ln.t.term==ID)
    {
        return node->idt;
    }
    else
    {
        idST *a;
        astNode *tmp=node->child;
        while(tmp!=NULL)
        {
        if((a=travandfind(tmp))!=NULL)
           {
               return a;
           }
            tmp=tmp->sibling;
        }
    }
    return NULL;
}


idST *findIDT(idST *idt,int line)
{
    if(idt==NULL||idt->child==NULL)
        return idt;

    idST *tmp=idt->child;
    while(tmp!=NULL)
    {
        if(tmp->start<=line && tmp->end>=line)
            return findIDT(tmp,line);
        else tmp=tmp->sibling;
    }
    return idt;

}

quadruple *IRcodeAssign(astNode *node,moduleSTEntry * min)
{
    quadruple* assign = createQuadruple(), *head=NULL;
    astNode *temp = node->child;//ID;
    int lineno = temp->unode.ln.t.lineno ;
    int l1 = newLabel();
    int lerr =0;
    astNode *whichST = node->child->sibling;//whichStmt
    if(whichST->unode.nt.symbol == lvalueARRStmt)
    {   
        lerr =1;
        astNode * index = whichST->child;
        quadruple *arr = IRcodeVarArrayCase(node,min,lineno);
        quadruple * temp7 = arr;
        int erflag = 0;
        while(temp7 != NULL && temp7->next!= NULL){
            temp7 = temp7->next;
        }

        whichST = whichST->child->sibling;
        int lin=temp->unode.ln.t.lineno;
        idST *ta = findIDT(temp->idt,lin);
        if(whichST->unode.nt.symbol == U)
        {
            head = addToEnd(head,UCode(whichST,ta,min));
        }        
        else   //if(temp->unode.nt.symbol == expr vala casee)
        {   
            head = addToEnd(head,ariBoolCode(whichST,ta,min));
        }
        head = addToEnd(head,arr);
        char * check = temp->unode.ln.t.lexeme;
        assign->result = check;
        assign->resultIdt = lookUpID(check,temp->idt);
        assign->nodeResult = temp;
        assign->arg1 = temp7->arg2;
        assign->node1 = temp7->node2;
        assign->idt1 = temp7->idt2;
        assign->arg2 = whichST->addr;
        assign->node2 = whichST;
        assign->idt2 = lookUpID(whichST->addr,ta);
        assign->op = ASSIGNARR;
    }
    else
    {
        ///l value id
        int lin=temp->unode.ln.t.lineno;
        idST *ta = findIDT(temp->idt,lin);
        if(whichST->unode.nt.symbol == U)
        {
            head = addToEnd(head,UCode(whichST,ta,min));
        }        
        else   //if(temp->unode.nt.symbol == expr vala casee)
        {   
            head = addToEnd(head,ariBoolCode(whichST,ta,min));
        }

        char * check = temp->unode.ln.t.lexeme;
        assign->result = check;
        assign->resultIdt = lookUpID(check,temp->idt);
        assign->nodeResult = temp;
        assign->arg1 = whichST->addr;
        assign->node1 = whichST;
        assign->idt1 = lookUpID(whichST->addr,ta);
        
        
        assign->op = ASSIGNOP;
        
    }
    

        quadruple* q3 = createQuadruple();
        char * arg1 = (char *)malloc(sizeof(char) * 30);
        sprintf(arg1, "l%d",l1);
        q3->arg1  = arg1;
        q3->op = LABEL;
    head = addToEnd(head,assign);
    if(lerr ==1)
    head = addToEnd(head,q3);
    
    return head;
}
       
quadruple *UCode(astNode* node, idST *idtable, moduleSTEntry * min)
{
    quadruple *quad_u = NULL;
    astNode *newNT = node->child->sibling;//newNt
    int l1 = newLabel();
    if(newNT->unode.nt.symbol == var_id_num){//ID whichID       
        astNode *temp = newNT->child;//ID
        idSTEntry * search = lookupidST(temp->idt,temp->unode.ln.t.lexeme);
        if(search != NULL){
            if(temp->sibling==NULL)
            {
                if(node->child->unode.ln.t.term==PLUS)
                {
                    if(search->isArray==1)
                    {
                        if(temp->sibling==NULL)
                        {
                          ;
                        }
                        else
                        {
                            quadruple *arr = IRcodeVarArrayCase(newNT,min,0);
                            quadruple * temp7 = arr;
                            while(temp7 != NULL && temp7->next!= NULL){
                                temp7 = temp7->next;
                            }
                            quad_u = addToEnd(arr,quad_u);
                            node->addr = temp7->result;
                            quadruple* q3 = createQuadruple();
                            char * arg1 = (char *)malloc(sizeof(char) * 30);
                            sprintf(arg1, "l%d",l1);
                            q3->arg1  = arg1;
                            q3->op = LABEL;
                            quad_u = addToEnd(quad_u,q3);
                        }
                    }
                    else
                    {
                        node->addr=temp->unode.ln.t.lexeme;
                    }
                }
                else
                {
                    if(search->isArray==1)
                    {
                        if(temp->sibling==NULL)
                        {
                            ;
                        }
                        else
                        {
                            quadruple *arr = IRcodeVarArrayCase(newNT,min,0);
                            quadruple * tp = arr;
                            while(tp != NULL && tp->next!= NULL){
                                tp = tp->next;
                            }
                            quad_u = addToEnd(tp,quad_u);
                            quadruple *arr1 = createQuadruple();
                            char* temp2 = (char *)malloc(sizeof(char) * 30);
                            int temporary = newTemp();
                            idST* current_idt = idtable;
                            current_idt = lookUpID(temp->unode.ln.t.lexeme, current_idt);
                            idSTEntry * search1 = lookupidST(current_idt,temp->unode.ln.t.lexeme);
                            idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
                            sprintf(ide->lexeme,"1t%d", temporary);
                            ide->dataType = search1->dataType;
                            ide->isArray = 0;
                            ide->width = search1->width; 


                            ide->offset = min->moduleSize;
                            min->moduleSize += ide->width;
                            current_idt = insertInidST(current_idt, ide);
                            sprintf(temp2,"1t%d", temporary);
                            arr1->result = temp2;
                            arr1->resultIdt = current_idt;
                            arr1->op = uminus;
                            arr1->arg1 = tp->result;
                            arr1->idt1 = lookUpID(temp->unode.ln.t.lexeme,current_idt);
                            quad_u = addToEnd(quad_u,arr1);
                            
                            
                            node->addr = arr1->result;
                            quadruple* q3 = createQuadruple();
                            char * arg1 = (char *)malloc(sizeof(char) * 30);
                            sprintf(arg1, "l%d",l1);
                            q3->arg1  = arg1;
                            q3->op = LABEL;
                            quad_u = addToEnd(quad_u,q3);
                            
                        }
                    }
                    else
                    {
                        
                            quadruple *neg = createQuadruple();
                            char* temp2 = (char *)malloc(sizeof(char) * 30);
                            int temporary = newTemp();
                            idST* current_idt = idtable;
                            current_idt = lookUpID(temp->unode.ln.t.lexeme, current_idt);
                            idSTEntry * search1 = lookupidST(current_idt,temp->unode.ln.t.lexeme);
                            idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
                            sprintf(ide->lexeme,"1t%d", temporary);
                            ide->dataType = search1->dataType;
                            ide->isArray = 0;
                            ide->width = search1->width;


                            ide->offset = min->moduleSize;
                            min->moduleSize += ide->width;

                            current_idt = insertInidST(current_idt, ide);
                            sprintf(temp2,"1t%d", temporary);
                            neg->result = temp2;
                            neg->resultIdt = current_idt;
                            neg->op = uminus;
                            neg->arg1 = temp->unode.ln.t.lexeme;
                            neg->idt1 = lookUpID(temp->unode.ln.t.lexeme,current_idt);
                            quad_u = addToEnd(quad_u,neg);
                            
                            
                            node->addr = neg->result;
                    }
                }
            }
        }
        else
        {
            // printf("SEMANTIC UNDECLARED ERROR LINE : %d    %d\n",line,temp->unode.ln.t.lineno);
            return NULL;
        }
    }

    else if(newNT->isLeaf==1)
    {

        if(node->child->unode.ln.t.term==MINUS){
            quadruple *neg = createQuadruple();
            char* temp2 = (char *)malloc(sizeof(char) * 30);
            int temporary = newTemp();
            idST* current_idt = idtable;
            idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
            sprintf(ide->lexeme,"1t%d", temporary);
            ide->dataType = newNT->unode.ln.t.term;
            ide->isArray = 0;

            if(ide->dataType == NUM ){
                ide->dataType= INTEGER;
                ide->width = 2;
            }
            else if(ide->dataType == RNUM ){
                ide->dataType= REAL;
                ide->width = 4;
            }


            ide->offset = min->moduleSize;
            min->moduleSize += ide->width;

            current_idt = insertInidST(current_idt, ide);
            sprintf(temp2,"1t%d", temporary);
            neg->result = temp2;
            neg->resultIdt = current_idt;
            neg->op = uminus;
            neg->arg1 = newNT->unode.ln.t.lexeme;
            neg->idt1 = lookUpID(newNT->unode.ln.t.lexeme,current_idt);
            quad_u = addToEnd(quad_u,neg);

            node->addr = neg->result;
            return quad_u;

        }

        else{
            node->addr=newNT->unode.ln.t.lexeme;
            return NULL;
        }
    }  

    else{
        quadruple *head1=NULL;
        quadruple *trm = trmCode(newNT,idtable,min);
        head1 = addToEnd(head1,trm);

        if(node->child->unode.ln.t.term==MINUS){
            quadruple *neg = createQuadruple();
            char* temp2 = (char *)malloc(sizeof(char) * 30);
            int temporary = newTemp();
            idST* current_idt = idtable;
            idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
            sprintf(ide->lexeme,"1t%d", temporary);
            ide->dataType = trmTrav(newNT, 0,0);
        

            if(ide->dataType == INTEGER){
                ide->width = 2;
                
            }
            else if(ide->dataType == REAL){
                ide->width = 4;
                
            }
            else if(ide->dataType == BOOLEAN){
                ide->width = 1;
                
            }

            ide->offset = min->moduleSize;
            min->moduleSize += ide->width;
            ide->isArray = 0;
            
            current_idt = insertInidST(current_idt, ide);
            sprintf(temp2,"1t%d", temporary);
            neg->result = temp2;
            neg->resultIdt = current_idt;
            neg->op = uminus;
            neg->arg1 = newNT->addr;
            neg->idt1 = lookUpID(newNT->addr,current_idt);;
            head1 = addToEnd(head1,neg);
            node->addr = temp2;

        }

        else{
            node->addr = newNT->addr;
        }
        
        return head1;
    }
    
    return quad_u;
}
        
quadruple *trmCode(astNode *node, idST *idt,moduleSTEntry * min)
{
    if(node->unode.nt.symbol==op1)
    {       
        quadruple *head1 = postTravTrm(node,idt,min);
        return head1;
    }    
    else 
    {   
        quadruple *head1 = factCode(node,idt,min);
        if(head1==NULL){
            return NULL;
        }
        return head1;
    }
}
        
quadruple *postTravTrm(astNode *node, idST *idt, moduleSTEntry * min)
{
    quadruple *val=NULL;
    Term dt;
    if(node->child->sibling->unode.nt.symbol==op1)
    {   

        quadruple *tmp = NULL,*tmp1 = NULL;
        tmp=postTravTrm(node->child->sibling,idt, min);
       
        
        tmp1=factCode(node->child->sibling->sibling,idt,min);
        dt = trmTrav(node, 0,0);
        val = addToEnd(val,tmp);
        val = addToEnd(val,tmp1);
    }
    else
    {

        quadruple *tmp= NULL,*tmp1= NULL;
        tmp=factCode(node->child->sibling,idt,min);
        tmp1=factCode(node->child->sibling->sibling,idt,min);
        dt = trmTrav(node, 0,0);
        val= addToEnd(val,tmp);
        val =addToEnd(val,tmp1);
        
      }
        quadruple *tempvar = createQuadruple();
        tempvar->arg1 = node->child->sibling->addr;
        tempvar->idt1 = lookUpID(node->child->sibling->addr,idt);
        tempvar->arg2 = node->child->sibling->sibling->addr;
        tempvar->idt2 = lookUpID(node->child->sibling->sibling->addr,idt); 
        tempvar->op = node->child->unode.ln.t.term;
        char* temp2 = (char *)malloc(sizeof(char) * 30);
        int temporary = newTemp();
        idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));

        ide->dataType = dt;

        if(ide->dataType == INTEGER){
                ide->width = 2;
            
        }
        else if(ide->dataType == REAL){
            ide->width = 4;
            
        }
        else if(ide->dataType == BOOLEAN){
            ide->width = 1;
            
        }
        ide->offset = min->moduleSize;
        min->moduleSize += ide->width;
        sprintf(ide->lexeme,"1t%d", temporary);
        ide->isArray = 0;
        idST* current_idt = insertInidST(idt, ide);
        sprintf(temp2,"1t%d", temporary);
        tempvar->result = temp2;
        tempvar->resultIdt = idt;
        val = addToEnd(val,tempvar);
        node->addr = tempvar->result;
        return val;

}
        
quadruple *factCode(astNode *node, idST *idt, moduleSTEntry * min)
{
    if(node->unode.nt.symbol==op2)
    {       
        return postTravFact(node,idt,min);
    }    
    else 
    {      
        //node is factor
        quadruple *head1 = fact1Code(node,idt, min);
        if(head1==NULL){
            return NULL;
        }
        return head1;
    }
}
        
quadruple *postTravFact(astNode *node, idST *idt, moduleSTEntry * min)               
{
    quadruple *val=NULL;
    Term dt;
    if(node->child->sibling->unode.nt.symbol == op2)
    {
        quadruple *tmp = NULL,*tmp1= NULL;
        tmp=postTravFact(node->child->sibling,idt,min);
        tmp1=fact1Code(node->child->sibling->sibling,idt, min);
        dt = factTrav(node,0,0);
        val = addToEnd(val,tmp);
        val=addToEnd(val,tmp1);
    }
    else
    {
        quadruple *tmp = NULL,*tmp1= NULL;
        tmp=fact1Code(node->child->sibling,idt, min);
        tmp1=fact1Code(node->child->sibling->sibling,idt, min);
        dt = factTrav(node,0,0);
        val = addToEnd(val,tmp);
        val=addToEnd(val,tmp1);
    }
    
        quadruple *tempvar = createQuadruple();
        tempvar->arg1 = node->child->sibling->addr;
        tempvar->idt1 = lookUpID(node->child->sibling->addr,idt);
        tempvar->arg2 = node->child->sibling->sibling->addr; 
        tempvar->idt2 = lookUpID(node->child->sibling->sibling->addr,idt);
        tempvar->op = node->child->unode.ln.t.term;//NAYA ENUM KA STUFF REPLACE||||||||||||||||||||||
        
        char* temp2 = (char *)malloc(sizeof(char) * 30);
        int temporary = newTemp();
        idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));

        ide->dataType = dt;

        if(ide->dataType == INTEGER){
                ide->width = 2;
            
        }
        else if(ide->dataType == REAL){
            ide->width = 4;
            
        }
        else if(ide->dataType == BOOLEAN){
            ide->width = 1;
            
        }

        ide->offset = min->moduleSize;
        min->moduleSize += ide->width;  
        sprintf(ide->lexeme,"1t%d", temporary);
        ide->isArray = 0;
        idST * current_idt = insertInidST(idt, ide);
        sprintf(temp2,"1t%d", temporary);
        tempvar->result = temp2;
        tempvar->resultIdt = idt;

        val = addToEnd(val,tempvar);
        node->addr = tempvar->result;
        return val;

}
        
quadruple *fact1Code(astNode *node, idST *idt ,moduleSTEntry * min)  
{
    quadruple *fact1 = NULL;
    int l1 = newLabel();
    int lbl = 0 ;
    if(node->child->unode.nt.symbol == var_id_num)
    {
        if(node->child->child->unode.ln.t.term==ID && node->child->child->sibling!=NULL)
        {
            //ID whichID
            quadruple *arr = IRcodeVarArrayCase(node->child,min,0);

            quadruple * temp = arr;
                while(temp != NULL && temp->next!= NULL){
                    temp = temp->next;
            }
            idSTEntry * search = lookupidST(node->child->child->idt, node->child->child->unode.ln.t.lexeme);
            fact1 = addToEnd(arr,fact1);
            node->addr = temp->result;
            astNode * t = node->child->child->sibling;
            if(search != NULL && search->isArray==1 && (search->isStatic==0 || t->unode.ln.t.term==ID)){
                quadruple* q3 = createQuadruple();
                char * arg1 = (char *)malloc(sizeof(char) * 30);
                sprintf(arg1, "l%d",l1);
                q3->arg1  = arg1;
                q3->op = LABEL;
                fact1 = addToEnd(fact1,q3);
            }
           
        }


        else if(node->child->child->unode.ln.t.term==ID)
        {
            node->addr= node->child->child->unode.ln.t.lexeme;
            return NULL;
        }

        else{

            node->addr= node->child->unode.ln.t.lexeme; 
            return NULL;
        
            }
            return fact1;

    }
    else
    {
        quadruple *head1 = ariBoolCode(node->child,idt, min);
        node->addr = node->child->addr;
        return head1;
    }

}
        
quadruple *ariBoolCode(astNode *node,idST *idt, moduleSTEntry * min)
{
    if(node->isLeaf==1)
    {
        node->addr=node->unode.ln.t.lexeme;
        return NULL;
    }
    else
    {
        if(node->unode.nt.symbol == logicalOp)
        {
            return postTravAny(node,idt, min);
        }
        else
        {
            return anyTermCode(node,idt, min);
        }
    }
}
     
quadruple *postTravAny(astNode *node, idST *idt, moduleSTEntry * min)
{
    quadruple *val=NULL;
    Term dt;
    if(node->child->sibling->unode.nt.symbol==logicalOp)
    {
        quadruple *tmp = NULL,*tmp1= NULL;
        tmp=postTravAny(node->child->sibling,idt, min);
        tmp1=anyTermCode(node->child->sibling->sibling,idt, min);
        dt = BOOLEAN;
        val = addToEnd(val,tmp);
        val=addToEnd(val,tmp1);
           
    }
    else
    {
        quadruple *tmp = NULL,*tmp1 = NULL;
        tmp=anyTermCode(node->child->sibling,idt, min);
        tmp1=anyTermCode(node->child->sibling->sibling,idt, min);
        dt = BOOLEAN;
        val = addToEnd(val,tmp);
        val=addToEnd(val,tmp1);
        
      }
    
        quadruple *tempvar = createQuadruple();
        tempvar->arg1 = node->child->sibling->addr;
        tempvar->idt1 = lookUpID(node->child->sibling->addr,idt);
        tempvar->arg2 = node->child->sibling->sibling->addr; 
        tempvar->idt2 = lookUpID(node->child->sibling->sibling->addr,idt);
        tempvar->op = node->child->unode.ln.t.term;
        char* temp2 = (char *)malloc(sizeof(char) * 30);
        int temporary = newTemp();
        idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
        ide->dataType = dt;

        if(ide->dataType == INTEGER){
                ide->width = 2;
            
        }
        else if(ide->dataType == REAL){
            ide->width = 4;
            
        }
        else if(ide->dataType == BOOLEAN){
            ide->width = 1;
            
        }

        ide->offset = min->moduleSize;
        min->moduleSize += ide->width;
        
        sprintf(ide->lexeme,"1t%d", temporary);
        
        ide->isArray = 0;
        
        idST * current_idt = insertInidST(idt, ide);
        sprintf(temp2,"1t%d", temporary);
        tempvar->result = temp2;
        tempvar->resultIdt = idt;
        val = addToEnd(val,tempvar);
        node->addr = tempvar->result;
    
        return val;

}

quadruple *anyTermCode(astNode *node, idST *idt, moduleSTEntry * min)
{
    if(node->isLeaf==1)
    {
        node->addr=node->unode.ln.t.lexeme;
        return NULL;
    }
    else
    {
        quadruple *head1= NULL;
        head1 = addToEnd(head1, ariExpCode(node,idt, min));
        return head1;
    }
}
        
        
quadruple *ariExpCode(astNode *node, idST *idt, moduleSTEntry * min)
{
    Term dt;
    if(node->unode.nt.symbol==relationalOp)
    {
        quadruple *tmp = NULL,*tmp1= NULL;
        tmp=trmCode(node->child->sibling,idt, min);     
        tmp1=trmCode(node->child->sibling->sibling,idt,min);    
        dt = ariExp1(node,0,0);    
        quadruple *val=NULL;
        val=addToEnd(val,tmp);
        val=addToEnd(val,tmp1);
        
        //relational Op
        quadruple *tempvar = createQuadruple();
        tempvar->arg1 = node->child->sibling->addr;
        tempvar->idt1 = lookUpID(node->child->sibling->addr,idt);
        tempvar->arg2 = node->child->sibling->sibling->addr; 
        tempvar->idt2 = lookUpID(node->child->sibling->sibling->addr,idt);
        tempvar->op = node->child->unode.ln.t.term;
        char* temp2 = (char *)malloc(sizeof(char) * 30);
        int temporary = newTemp();
        idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));

        ide->dataType = dt;

        if(ide->dataType == INTEGER){
                ide->width = 2;
            
        }
        else if(ide->dataType == REAL){
            ide->width = 4;
            
        }
        else if(ide->dataType == BOOLEAN){
            ide->width = 1;
            
        }
        
        ide->offset = min->moduleSize;
        min->moduleSize += ide->width;
        

        sprintf(ide->lexeme,"1t%d", temporary);
        ide->isArray = 0;
        idST * current_idt = insertInidST(idt, ide);
        sprintf(temp2,"1t%d", temporary);
        tempvar->result = temp2;
        tempvar->resultIdt = idt;
        val = addToEnd(val,tempvar);
        node->addr = tempvar->result;
    
        return val;
    }
    else {
        
        return trmCode(node,idt,min);
        
    }

}
             
quadruple * IRcodeConditional(astNode* node, quadruple* head, moduleSTEntry * min){
    
    astNode * idnode = node->child;
    astNode * caseStmts = idnode->sibling;
    astNode * caseSingle = caseStmts->child;
    char* var = idnode->unode.ln.t.lexeme; //name of switch variable ID

    int end = newLabel();

    idSTEntry* entry = lookupidST(idnode->idt, idnode->unode.ln.t.lexeme);
    if(entry->dataType == INTEGER){
        
        astNode* temp = caseSingle;
        quadruple* q1 = createQuadruple();
        q1->arg1 = var;
        q1->idt1 = idnode->idt;
        q1->node1 = idnode;
        
        q1->arg2 = temp->child->unode.ln.t.lexeme;
        q1->node2 = temp->child;
        q1->idt2 = temp->idt;

        char* temp1 = (char *)malloc(sizeof(char) * 30);
        int t1 = newTemp();
        idST* current_idt = idnode->idt;
        idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
        sprintf(ide->lexeme,"1t%d", t1);
        ide->dataType = BOOLEAN;
        ide->isArray = 0;
        ide->width = 1;

        ide->offset = min->moduleSize;
        min->moduleSize += ide->width;
        current_idt = insertInidST(current_idt, ide);
        sprintf(temp1,"1t%d", t1);

        q1->result = temp1;
        q1->resultIdt = current_idt ;
        q1->op = NE;

        head = addToEnd(head,q1);

        int l1=newLabel();
        quadruple* check = createQuadruple();
        check->arg1 = temp1;
        check->idt1 = current_idt;

        check->op = GOTOIFTRUE;
        
        check->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(check->arg2,"l%d",l1);
        head = addToEnd(head,check);

        head = IRcodeStmts(temp->child->sibling,head, min);

        int end=newLabel();
        quadruple* q2 = createQuadruple();
        q2->op = GOTO;
        q2->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(q2->arg2,"l%d",end);
        head = addToEnd(head,q2);

        quadruple * q3 = createQuadruple();
        q3->arg1  = (char *)malloc(sizeof(char) * 30);

        sprintf(q3->arg1, "l%d",l1);
        q3->op = LABEL;
        head = addToEnd(head,q3);
        if (temp->sibling!=NULL)
            temp = temp->sibling->child;
        else temp=temp->sibling;
        while(temp!=NULL){
            quadruple* q11 = createQuadruple();
            q11->arg1 = var;
            q11->arg2 = temp->child->unode.ln.t.lexeme;
            q11->node1 = idnode;
            q11->idt1 = idnode->idt;

            q11->node2 = temp->child;
            q11->idt2 = temp->child->idt;

            char* temp11 = (char *)malloc(sizeof(char) * 30);
            int t11 = newTemp();
            current_idt = idnode->idt;
            idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
            sprintf(ide->lexeme,"1t%d", t11);
            ide->dataType = BOOLEAN;
            ide->isArray = 0;
            ide->width = 1;

            ide->offset = min->moduleSize;
            min->moduleSize += ide->width;
            current_idt = insertInidST(current_idt, ide);
            sprintf(temp11,"1t%d", t11);

            q11->result = temp11;
            q11->resultIdt =  current_idt;

            q11->op = NE;
            head = addToEnd(head,q11);

            int l11=newLabel();
            quadruple* check = createQuadruple();
            check->arg1 = temp11;
            check->idt1 =  current_idt;

            check->op = GOTOIFTRUE; 
            check->arg2 = (char *)malloc(sizeof(char) * 30);

            sprintf(check->arg2,"l%d",l11);
            head = addToEnd(head,check);

            head = IRcodeStmts(temp->child->sibling,head, min);

            quadruple* q21 = createQuadruple();
            q21->op = GOTO;
            q21->arg2 = (char *)malloc(sizeof(char) * 30);

            sprintf(q21->arg2,"l%d",end);
            head = addToEnd(head,q21);

            quadruple * q31 = createQuadruple();
            q31->arg1  = (char *)malloc(sizeof(char) * 30);

            sprintf(q31->arg1, "l%d",l11);
            q31->op = LABEL;
            head = addToEnd(head,q31);

            if (temp->sibling!=NULL)
                temp = temp->sibling->child;
            else break;
        }
        astNode * ndefault = caseStmts->sibling;

        head = IRcodeStmts(ndefault->child,head, min);

        quadruple * q4 = createQuadruple();
        q4->arg1  = (char *)malloc(sizeof(char) * 30);
        sprintf(q4->arg1, "l%d",end);
        q4->op = LABEL;
        head = addToEnd(head,q4);
        
        
    }
    else if(entry->dataType == BOOLEAN){

        astNode* temp = caseSingle;
        quadruple* q1 = createQuadruple();
        q1->arg1 = var;
        q1->idt1 = idnode->idt;
        q1->node1 = idnode;
        
        q1->arg2 = temp->child->unode.ln.t.lexeme;
        q1->node2 = temp->child;
        q1->idt2 = temp->idt;

        char* temp1 = (char *)malloc(sizeof(char) * 30);
        int t1 = newTemp();
        idST* current_idt = idnode->idt;
        idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
        sprintf(ide->lexeme,"1t%d", t1);
        ide->dataType = BOOLEAN;
        ide->isArray = 0;
        ide->width = 1;

        ide->offset = min->moduleSize;
        min->moduleSize += ide->width;
        current_idt = insertInidST(current_idt, ide);
        sprintf(temp1,"1t%d", t1);

        q1->result = temp1;
        q1->resultIdt = current_idt ;
        q1->op = NE;

        head = addToEnd(head,q1);

        int l1=newLabel();
        quadruple* check = createQuadruple();
        check->arg1 = temp1;
        check->idt1 = current_idt;

        check->op = GOTOIFTRUE; 
        check->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(check->arg2,"l%d",l1);
        head = addToEnd(head,check);

        head = IRcodeStmts(temp->child->sibling,head, min);

        int end=newLabel();
        quadruple* q2 = createQuadruple();
        q2->op = GOTO;
        q2->arg2 = (char *)malloc(sizeof(char) * 30);
        sprintf(q2->arg2,"l%d",end);
        head = addToEnd(head,q2);

        quadruple * q3 = createQuadruple();
        q3->arg1  = (char *)malloc(sizeof(char) * 30);

        sprintf(q3->arg1, "l%d",l1);
        q3->op = LABEL;
        head = addToEnd(head,q3);
        if (temp->sibling!=NULL)
            temp = temp->sibling->child;
        else temp=temp->sibling;
        while(temp!=NULL){
            
            quadruple* q11 = createQuadruple();
            q11->arg1 = var;
            q11->arg2 = temp->child->unode.ln.t.lexeme;
            q11->node1 = idnode;
            q11->idt1 = idnode->idt;

            q11->node2 = temp->child;
            q11->idt2 = temp->child->idt;

            char* temp11 = (char *)malloc(sizeof(char) * 30);
            int t11 = newTemp();
            current_idt = idnode->idt;
            idSTEntry* ide = (idSTEntry *)malloc(sizeof(idSTEntry));
            sprintf(ide->lexeme,"1t%d", t11);
            ide->dataType = BOOLEAN;
            ide->isArray = 0;
            ide->width = 1;

            ide->offset = min->moduleSize;
            min->moduleSize += ide->width;
            current_idt = insertInidST(current_idt, ide);
            sprintf(temp11,"1t%d", t11);

            q11->result = temp11;
            q11->resultIdt =  current_idt;

            q11->op = NE;
            head = addToEnd(head,q11);

            int l11=newLabel();
            quadruple* check = createQuadruple();
            check->arg1 = temp11;
            check->idt1 =  current_idt;

            check->op = GOTOIFTRUE; 
            check->arg2 = (char *)malloc(sizeof(char) * 30);

            sprintf(check->arg2,"l%d",l11);
            head = addToEnd(head,check);

            head = IRcodeStmts(temp->child->sibling,head, min);

            quadruple* q21 = createQuadruple();
            q21->op = GOTO;
            q21->arg2 = (char *)malloc(sizeof(char) * 30);

            sprintf(q21->arg2,"l%d",end);
            head = addToEnd(head,q21);

            quadruple * q31 = createQuadruple();
            q31->arg1  = (char *)malloc(sizeof(char) * 30);

            sprintf(q31->arg1, "l%d",l11);
            q31->op = LABEL;
            head = addToEnd(head,q31);

            if (temp->sibling!=NULL)
                temp = temp->sibling->child;
            else break;
        }
        quadruple * q4 = createQuadruple();
        q4->arg1  = (char *)malloc(sizeof(char) * 30);
        sprintf(q4->arg1, "l%d",end);
        q4->op = LABEL;
        head = addToEnd(head,q4);
    }
    
    return head;
}





quadruple * IRcodeStmts(astNode * node, quadruple * head, moduleSTEntry * min){
    astNode * temp = node ;
    while(temp != NULL){
        if(temp->isLeaf == 0 && temp->unode.nt.symbol == declareStmt){
            head = addToEnd(head,IRcodeDeclareStmt(temp,min));
        }
        else if(temp->isLeaf == 0 && temp->unode.nt.symbol == moduleReuseStmt){
            head = addToEnd(head,IRcodeModuleReuse(temp,min));
        }

        else if(temp->isLeaf == 0 && temp->unode.nt.symbol == conditionalStmt){
            head = IRcodeConditional(temp,head,min);
        }

        else if(temp->isLeaf == 0 && temp->unode.nt.symbol == iterativeStmt){
            head = addToEnd(head,IRcodeIterative(temp,min));
        }
        
        else if(temp->isLeaf == 0 && temp->unode.nt.symbol == assignmentStmt){
            head = addToEnd(head,IRcodeAssign(temp,min));
        }
        
        else if(temp->isLeaf == 0 && temp->unode.nt.symbol == ioStmt){
            head = addToEnd(head,IRcodeIoStmt(temp,min));
        }
        temp = temp->sibling;
    }


    return head;
}

quadruple *IRcodeModuleReuse(astNode *node, moduleSTEntry * min){
    

      //1. Place actual parameters
      //2. Call function (saves return address)

      //On entering function
      //take input parameters?
      //1. push ebp; mov ebp,esp
      //2. update esp, AR size + 2 reserved locations
      //On call instruction
      //3. place parameters
      //4. call function

      //On returning from function
      //place output parameters
      //1. mov ebp, [ebp-1]
      //2. Add to esp, size of callee AR

        //push, call, pop parameters (module Reuse)

      moduleST * mt = getmt();
      quadruple *head = NULL;
      astNode *temp = node->child; //moduleReuse
      astNode * modTemp = temp->sibling;
      char *check;
     
      //checking Optional
      quadruple *outplist = NULL;
      astNode *takeout=temp;
      if(temp->isLeaf == 0 && temp->unode.nt.symbol == idList){
            temp =  temp->child; //ID ;
            while(temp != NULL){ //output actual parameters
                check = temp->unode.ln.t.lexeme;
                
                quadruple *outparam = createQuadruple();
                outparam->op = ACTUAL_OUT_PARAMS;
                outparam->arg1 = check;
                outparam->idt1 = temp->idt;
                outplist = addToEnd(outplist,outparam);

                temp = temp->sibling;
                if(temp != NULL)
                    temp = temp->child;   
              }
        
            
            outplist = reverse(outplist);

            head = addToEnd(head,outplist);
            temp = modTemp;
            //checking for module name and idList
            quadruple *moduleCall = createQuadruple();
            if(temp->isLeaf == 1 && temp->unode.ln.t.term == ID){ //module name;
                check = temp->unode.ln.t.lexeme ;
                moduleSTEntry *currModule = lookupmoduleST(mt,check);
                
                moduleCall->op = CALL;
                moduleCall->arg1 = check;

                }

                temp = temp->sibling;
                quadruple *inplist = NULL, *remlist = NULL;
                if(temp != NULL && temp->isLeaf == 0 && temp->unode.nt.symbol == idList){
                temp =  temp->child ;//ID ;
                    
                    while(temp != NULL){//input actual parameters
                        check = temp->unode.ln.t.lexeme;
                        

                        quadruple *inparam = createQuadruple();
                        quadruple *reminparam = createQuadruple();
                        inparam->op = ACTUAL_IN_PARAMS;
                            pNode * in = min->inParams;

                        while(in != NULL){
                            if(strcmp(in->lexeme,check)==0){
                                idST * inputHandleIDT = lookUpID(check,temp->idt);
                                idSTEntry *inputHandle = lookupidST(inputHandleIDT,check);
                                if(min->idt == inputHandleIDT &&inputHandle->isArray ==1 ){

                                    inparam->arg2 = (char*)malloc(sizeof(char)*30);
                                    strcpy(inparam->arg2,"array");
                                    reminparam->arg2 = (char*)malloc(sizeof(char)*30);
                                    strcpy(reminparam->arg2,"array");
                                    // break;
                                }
                            }   
                            in = in->next;
                        }
                        inparam->arg1 = check;
                        inparam->idt1 = temp->idt;
                        inplist = addToEnd(inplist,inparam);

                        
                        reminparam->op = POP_ACTUAL_IN_PARAMS;
                        reminparam->arg1 = check;
                        reminparam->idt1 = temp->idt;
                        remlist = addToEnd(remlist,reminparam);
                        
                        
                        temp = temp->sibling;
                        if(temp != NULL){
                            temp = temp->child;
                        }
                    }

                    inplist = reverse(inplist);
                    head = addToEnd(head,inplist);
                }

                head = addToEnd(head, moduleCall);
                head = addToEnd(head, remlist);

                quadruple *olist = NULL;
                takeout =  takeout->child; //ID ;
                while(takeout != NULL){ //output actual parameters
                    check = takeout->unode.ln.t.lexeme;
                    
                    quadruple *outparam = createQuadruple();
                    outparam->op = POP_ACTUAL_OUT_PARAMS;
                    outparam->arg1 = check;
                    outparam->idt1 = takeout->idt;
                    olist = addToEnd(olist,outparam);

                    takeout = takeout->sibling;
                    if(takeout != NULL)
                        takeout = takeout->child;   
              }
                head = addToEnd(head, olist);
            }
        
        //checking when no <Optional>
        else if(temp->isLeaf == 1 && temp->unode.ln.t.term == ID){ //module name;
            check = temp->unode.ln.t.lexeme ;
            
            quadruple *moduleCall = createQuadruple();
            moduleCall->op = CALL;
            moduleCall->arg1 = check;
            
            temp = temp->sibling;
            quadruple *inplist = NULL, *remlist = NULL;
            if(temp != NULL && temp->isLeaf == 0 && temp->unode.nt.symbol == idList){//input actual parameters
            temp =  temp->child ;//ID ;
                while(temp != NULL){
                    check = temp->unode.ln.t.lexeme;
                    
                    quadruple *inparam = createQuadruple();
                    quadruple *reminparam = createQuadruple();
                    pNode * in = min->inParams;
                    while(in != NULL){
                            if(strcmp(in->lexeme,check)==0){
                                idST * inputHandleIDT = lookUpID(check,temp->idt);
                                idSTEntry *inputHandle = lookupidST(inputHandleIDT,check);
                                if(min->idt == inputHandleIDT && inputHandle->isArray ==1 ){
                                    inparam->arg2 = (char*)malloc(sizeof(char)*30);
                                    strcpy(inparam->arg2,"array");
                                    reminparam->arg2 = (char*)malloc(sizeof(char)*30);
                                    strcpy(reminparam->arg2,"array");
                                }
                            }   
                            in = in->next;
                    }       

                    inparam->op = ACTUAL_IN_PARAMS;
                    inparam->arg1 = check;
                    inparam->idt1 = temp->idt;
                    head = addToEnd(head,inparam);

                    
                    reminparam->op = POP_ACTUAL_IN_PARAMS;
                    reminparam->arg1 = check;
                    reminparam->idt1 = temp->idt;
                    remlist = addToEnd(remlist,reminparam);
                        
                    temp = temp->sibling;
                    if(temp != NULL){
                        temp = temp->child;
                    }
                }
            }

            head = addToEnd(head, moduleCall);
            head = addToEnd(head, remlist);
        }
    
    return head;
}


quadruple * moduleTraverse(astNode * node){
   quadruple * head = NULL; 
   astNode *  root  = node->child;
   if(root->unode.nt.symbol== moduleDeclarations)
   root = node->child->sibling;
   moduleST * mt = getmt();
   while(root != NULL ){
        if(root->unode.nt.symbol == otherModules ||root->unode.nt.symbol == module ){
            astNode * moduleroot;
            if(root->unode.nt.symbol != otherModules)
                moduleroot= root ;
            else moduleroot = root->child;
            while(moduleroot != NULL){
                if(moduleroot->isLeaf ==0 && moduleroot->unode.nt.symbol == module){
                    quadruple* new = createQuadruple();
                    astNode * moduleID = moduleroot->child;    
                    new->arg1 = moduleID->unode.ln.t.lexeme;
                    moduleSTEntry * min = lookupmoduleST(mt,new->arg1);

                    new->op = LABEL;  
                    head = addToEnd(head , new);

                    //init
                    quadruple *initmod = createQuadruple();
                    initmod->op = INIT;
                    head = addToEnd(head, initmod);

                    //addToEsp
                    quadruple *modbegin = createQuadruple();
                    modbegin->op = SUBESPMOD;
                    modbegin->arg1 =  moduleID->unode.ln.t.lexeme;
                    head = addToEnd(head, modbegin);

                    //inParams
                    pNode *tmp = min->inParams;
                    while(tmp!=NULL)
                    {
                        quadruple *inparam = createQuadruple();
                        inparam->op= FORMAL_IN_PARAMS;
                        inparam->arg1 = tmp->lexeme;
                        inparam->idt1 = min->idt;
                        inparam->arg2 = min->lexeme_mod;
                        head = addToEnd(head, inparam);
                        tmp = tmp->next;
                    }   


                    astNode * ret1 = moduleID->sibling->sibling, *moduleD;
                    if(ret1->unode.nt.symbol == ret)
                        moduleD = ret1->sibling;
                    else
                        moduleD = ret1;
                    head = generateIntermediateCodeHelper(head ,moduleroot,min);

                    tmp = min->outParams;
                    while(tmp!=NULL)
                    {
                        quadruple *outparam = createQuadruple();
                        outparam->op= FORMAL_OUT_PARAMS;
                        outparam->arg1 = tmp->lexeme;
                        outparam->idt1 = min->idt;
                        outparam->arg2 = min->lexeme_mod;
                        head = addToEnd(head, outparam);
                        tmp = tmp->next;
                    }   

                    quadruple *modend = createQuadruple();
                    modend->op = ADDESPMOD;
                    modend->arg1 =  moduleID->unode.ln.t.lexeme;
                    head = addToEnd(head, modend);

                    quadruple * returnback =  createQuadruple();
                    returnback->op = RETURN;
                    head = addToEnd(head,returnback);
                }
                 moduleroot =  moduleroot->sibling;
            }
        }
        else if(root->isLeaf==0 && root->unode.nt.symbol==driverModule){
            astNode * driverroot = root->child ;
            quadruple* new = createQuadruple();
            new->arg1 = (char*)malloc(sizeof(char)*30);
            strcpy(new->arg1,"123driver");
            moduleSTEntry * min = lookupmoduleST(mt,new->arg1);

            new->op = LABEL;  
            head = addToEnd(head , new);
            quadruple *initmod = createQuadruple();
            initmod->op = INIT;
            head = addToEnd(head, initmod);

            quadruple *modbegin = createQuadruple();
            modbegin->op = SUBESPMOD;
            modbegin->arg1 =  new->arg1;
            head = addToEnd(head, modbegin);

            head = generateIntermediateCodeHelper(head ,root,min);

            quadruple *modend = createQuadruple();
            modend->op = ADDESPMOD;
            modend->arg1 =  new->arg1;
            head = addToEnd(head, modend);

            quadruple * returnback =  createQuadruple();
            returnback->op = RETURN;
            head = addToEnd(head,returnback);
        }
     if(root->unode.nt.symbol == module)
     break;
     root = root->sibling  ;
   }
    return head;        
}

quadruple* reverse(quadruple * in)
{
    quadruple  *prev = NULL, *cur = NULL;

    if(in != NULL)
    {
        prev = in;
        cur = in->next;
        in = in->next;

        prev->next = NULL;

        while(in != NULL)
        {
            in = in->next;
            cur->next = prev;

            prev = cur;
            cur = in;
        }

        in = prev; 
    }
return in;
}



quadruple * IRcodeDeclareStmt(astNode * temp,moduleSTEntry *min){
    quadruple * head = NULL;
    astNode * idlist = temp->child; //ID wali series;
    astNode * dataty = temp->child->sibling;
    if(dataty->isLeaf == 0){
        astNode * rangearr = dataty->child;
        astNode * first = rangearr->child;
        astNode * second = rangearr->child->sibling;
        if(first->unode.ln.t.term == ID || second->unode.ln.t.term == ID){
            
            while(idlist != NULL){
               
                astNode * temp= idlist->child;
                quadruple * extract = createQuadruple();
                extract->op = DYARR;
                extract->result = temp->unode.ln.t.lexeme;
                extract->nodeResult = temp;
                extract->resultIdt = temp->idt;
                if(first->unode.ln.t.term == ID ){
                    extract->arg1 = first->unode.ln.t.lexeme;
                    extract->node1 = first;
                    extract->idt1 = first->idt;
                    
                }
                else{
                    extract->arg1 = first->unode.ln.t.lexeme;
                    extract->node1 = first;
                    extract->idt1 = NULL;
                }
                if(second->unode.ln.t.term == ID ){
                    extract->arg2 = second->unode.ln.t.lexeme;
                    extract->node2 = second;
                    extract->idt2 = second->idt;
                }

                else{
                    extract->arg2 = second->unode.ln.t.lexeme;
                    extract->node2 = second;
                    extract->idt2 = NULL;
                }
                head = addToEnd(head,extract);
                idlist  = temp->sibling;

            }

        }
        else{

            while(idlist != NULL){
               
                astNode * temp= idlist->child;
                quadruple * extract = createQuadruple();
                extract->op = STATARR;
                extract->result = temp->unode.ln.t.lexeme;
                extract->nodeResult = temp;
                extract->resultIdt = temp->idt;
                head = addToEnd(head,extract);
                idlist  = temp->sibling;

            }

        }
    }
    return head;
}