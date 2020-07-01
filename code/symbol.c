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
extern  moduleST  * functionST;
int OFFSET =0;
int hasSemanticError = 0;

idST * createidST(){
    idST * newidt = (idST *)malloc(sizeof(idST));
    newidt->idHashTable = createIDHashTable();
    newidt->parent = NULL;
    newidt->sibling = NULL;
    newidt->child = NULL;
    newidt->nesting =0;
    return newidt;
}

idSTEntry * lookupidST( idST * idt, char * lex){
    if( idt == NULL){
        return NULL ;
    }
    return searchIDHashTable(lex,idt->idHashTable);
}

idST * insertInidST(idST * idt, idSTEntry * in){
    insertInIDHashTable(in,idt->idHashTable);
    return idt;
}

// Functions for Funct Symbol Table;
moduleST * createmoduleST(){
    moduleST* newmt = (moduleST*)malloc(sizeof(moduleST));
    newmt->modHashTable = createModHashTable();
    return newmt;
}

moduleSTEntry * lookupmoduleST( moduleST * mt, char* lex){
    return searchModuleHashTable(lex,mt->modHashTable);

}

moduleST * insertInmoduleST( moduleST * mt, moduleSTEntry * me){
    insertInModuleHashTable(me,mt->modHashTable);
    return mt ;

}

void populateParameterlist(astNode * node ,moduleSTEntry * mSTentry,int errorFlag){ //populate list and number of parameters
    
    //check input plist and ret
    pNode * new = (pNode*)malloc(sizeof(pNode));
    if(node == NULL){
        return ;
    }
    
    
    pNode * head = new, * prev = new;
    astNode * temp = node->child;
    idSTEntry * temp2 ;

    while(temp != NULL){
        temp2= (idSTEntry *)malloc(sizeof(idSTEntry));
        strcpy(new->lexeme,temp->unode.ln.t.lexeme);
        temp->idt = mSTentry->idt;
        strcpy(temp2->lexeme,temp->unode.ln.t.lexeme);
        temp2->t = temp->unode.ln.t ;
        if(temp->sibling->isLeaf == 1){
            new->dataType = temp->sibling->unode.ln.t.term;
            new->isArray =0;
            new->isAssigned=0;
            temp2->dataType = temp->sibling->unode.ln.t.term;
            if(temp2->dataType == INTEGER){
                temp2->offset = OFFSET;
                temp2->width = 2;
                OFFSET +=2 ;
            }
            else if(temp2->dataType == REAL){
                temp2->offset = OFFSET;
                temp2->width = 4;
                OFFSET +=4 ;
            }
            else if(temp2->dataType == BOOLEAN){
                temp2->offset = OFFSET;
                OFFSET +=1 ;
                temp2->width = 1;
            }
        }
        else {
            if(node->unode.nt.symbol==output_plist){
                if(errorFlag==1){
                    printf("ERROR: ouput can not be array\n");}
            }
            else{
                new->dataType = temp->sibling->unode.nt.dataType;
                new->isArray =1;
                temp2->dataType = new->dataType;
                temp2->isArray = 1;
                temp2->isStatic =0;
                temp2->end_pop =0;
                temp2->start_pop = 0;
                astNode * next = temp->sibling->child->child;
                if(next->unode.ln.t.term == NUM){
                    temp2->start_pop = 1;
                    temp2->start.num = next->unode.ln.t.value.v2;
                }
                else{
                    idSTEntry *in = (idSTEntry *)malloc(sizeof(idSTEntry));
                    strcpy(in->lexeme,next->unode.ln.t.lexeme);
                    in->t = next->unode.ln.t;
                    in->offset = OFFSET +1;
                    temp2->start_pop = 0;
                    temp2->start.entry = in;
                }
                
                int firstType = next->unode.ln.t.term;
                next = next->sibling;
                if(next->unode.ln.t.term == NUM){
                    temp2->end_pop = 1;
                    temp2->end.num = next->unode.ln.t.value.v2;
                }
                else{

                    idSTEntry *in = (idSTEntry *)malloc(sizeof(idSTEntry));
                    strcpy(in->lexeme,next->unode.ln.t.lexeme);
                    in->t = next->unode.ln.t;
                    in->dataType = INTEGER;
                    in->offset = OFFSET + 3;
                    temp2->end_pop = 0;
                    temp2->end.entry = in;
                }
                

                if(firstType == NUM && next->unode.ln.t.term == NUM){
                    temp2->isStatic = 1;
                    if(temp2->end.num < temp2->start.num){
                        if(errorFlag==1){
                        printf("LINE NO. : %d :ERROR: ",next->unode.ln.t.lineno);
                        printf("end index %d less than start index %d of array\n", temp2->end.num, temp2->start.num);}
                        hasSemanticError = 1;
                    temp2->isStatic = 0;
                    }

                }
                    temp2->width = 5 ;
                    temp2->offset = OFFSET;
                    OFFSET += temp2->width;
            }
        }

        temp2->dataType = new->dataType;
        insertInidST(mSTentry->idt , temp2);
        temp->idt = mSTentry->idt ;
        prev->next = new;
        prev = new;
        new = (pNode*)malloc(sizeof(pNode));
        if(temp->sibling->sibling != NULL)
            temp = temp->sibling->sibling->child;
        else 
            break;
    }
    
    prev->next = NULL;
    if(node->isLeaf==0 && node->unode.nt.symbol==input_plist){
        mSTentry->inParams =  head;
        pNode *tmp = mSTentry->inParams;
        while(tmp!=NULL)
        {
            tmp = tmp->next;
        }   
    }
    
    else if(node->isLeaf==0 && node->unode.nt.symbol==output_plist){
        mSTentry->outParams =  head; 
        pNode *tmp = mSTentry->outParams;
        while(tmp!=NULL)
        {
            tmp = tmp->next;
        }   
    }
    mSTentry->decOffset = OFFSET;

}

idST  * lookUpID(char * lex,idST * idt){
    idSTEntry * res = NULL;

    if(lex == NULL) {
        return NULL;
    }
    idST * temp = idt;
    while(temp != NULL){
        res = lookupidST(temp,lex);
        if(res != NULL)
            break;
        temp = temp->parent;
    }
    if(res==NULL)
        return NULL;
    else 
        return temp;
   
}

void populateSTStmt(astNode *node, idST *idt,moduleST * mt, moduleSTEntry * min, int errorFlag)
{   
    if(node == NULL ) return;
    
    if(node->unode.nt.symbol==declareStmt) //declareStmt
    {
        
        astNode *list = node->child->child;//leaf node
        astNode *type = node->child->sibling;//dataType non terminal
        while(list!=NULL)
        {
            idSTEntry *in = (idSTEntry *)malloc(sizeof(idSTEntry));
            strcpy(in->lexeme,list->unode.ln.t.lexeme);
            in->t = list->unode.ln.t;
            in->isAssigned = 0;
            in->isArray =0;
            in->dataType = type->unode.ln.t.term;
            idSTEntry *ide = lookupidST(idt, in->lexeme);
            
            pNode * input  = min->outParams;

            while(input != NULL){

                if(strcmp(input->lexeme,in->lexeme)==0) {
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",list->unode.ln.t.lineno);
                    printf("redeclaration of output parameter %s\n", in->lexeme);}
                    hasSemanticError = 1;
                    //return
                }

                input = input->next;
            }
            
            if(ide != NULL){
                
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",list->unode.ln.t.lineno);
                    printf("redeclaration of %s\n", in->lexeme);}
                    hasSemanticError = 1;
            }
            else{
                if(type->isLeaf == 1){
                 //
                if(in->dataType == INTEGER){
                    in->offset = OFFSET;
                    in->width = 2;
                    OFFSET +=2 ;
                }
                else if(in->dataType == REAL){
                    in->offset = OFFSET;
                    in->width = 4;
                    OFFSET +=4 ;
                }
                else if(in->dataType == BOOLEAN){
                    in->offset = OFFSET;
                    in->width = 1;
                    OFFSET +=1 ;
                }
            }
            else{
                in->dataType = type->unode.nt.dataType;
                in->isArray = 1;
                in->isStatic = 0 ;
                in->end_pop = 0;
                astNode * next = type->child->child;
                if(next->unode.ln.t.term == NUM){
                    in->start_pop = 1;
                    in->start.num = next->unode.ln.t.value.v2;
                }
                else{
                    idST * rangeIDT =  lookUpID(next->unode.ln.t.lexeme,idt);
                    idSTEntry * entry= lookupidST(rangeIDT,next->unode.ln.t.lexeme);
                    if(entry == NULL){
                        if(errorFlag == 1){
                        printf("LINE NO. : %d :ERROR: ",next->unode.ln.t.lineno);
                        printf("array range variable %s undeclared\n", next->unode.ln.t.lexeme);
                        }
                        hasSemanticError = 1;
                    }
                    else{
                        in->start_pop = 0;
                        in->start.entry = entry;
                    }

                }

                int firstType = next->unode.ln.t.term ;
                
                next = next->sibling;
                if(next->unode.ln.t.term == NUM){
                    in->end_pop = 1;
                    in->end.num = next->unode.ln.t.value.v2;

                }
                else{
                    idST * rangeIDT =  lookUpID(next->unode.ln.t.lexeme,idt);
                    idSTEntry * entry= lookupidST(rangeIDT,next->unode.ln.t.lexeme);
;                    if(entry == NULL){
                        if(errorFlag==1){
                            printf("LINE NO. : %d :ERROR: ",next->unode.ln.t.lineno);
                            printf("array range variable %s undeclared\n", next->unode.ln.t.lexeme);
                        }
                        hasSemanticError = 1;
                    }
                    else{
                        in->end_pop = 0;
                        in->end.entry = entry;
                    }
                }
                if(firstType == NUM && next->unode.ln.t.term == NUM){
                    in->isStatic =1 ;
                    if(in->end.num < in->start.num){
                        if(errorFlag==1){
                        printf("LINE NO. : %d :ERROR: ",next->unode.ln.t.lineno);
                        printf("end index %d less than start index %d of array\n", in->end.num, in->start.num);}
                        hasSemanticError = 1;
                        in->isStatic = 0;
                    }
                }
               

                if(in->isStatic==1){

                    int breadth = in->end.num - in->start.num +1;
                    if(in->dataType == INTEGER){
                        in->offset = OFFSET;
                        in->width  = (2* breadth)+1 ;
                        OFFSET +=in->width  ;
                    }
                    else if(in->dataType == REAL){
                        in->offset = OFFSET;
                        in->width  = (4* breadth)+1;
                        OFFSET += in->width  ;
                    }
                    else if(in->dataType == BOOLEAN){
                        in->offset = OFFSET;
                        in->width  = ( breadth)+1;
                        OFFSET += in->width ;
                    }
                }
                else
                {
                    in->width = 1;
                    in->offset = OFFSET;
                    OFFSET += in->width;
                }

            }
                insertInidST(idt, in);
                list->idt = idt;                  
                ide = lookupidST(idt,in->lexeme);

            }
            list=list->sibling;
            if(list!=NULL) 
                list=list->child;
        }


        astNode * rangearr = node->child->sibling->child;
        if(node->child->sibling->isLeaf == 0){
            astNode * first = rangearr->child;
            astNode * second = rangearr->child->sibling;
            idST * srch ;//= lookUpID(first->unode.ln.t.lexeme,idt);
            if(first->unode.ln.t.term == ID){
                srch = lookUpID(first->unode.ln.t.lexeme,idt);
                first->idt = srch;
            }
            if(second->unode.ln.t.term == ID){
                srch = lookUpID(second->unode.ln.t.lexeme,idt);
                second->idt = srch;
            }
        }
    }
    
    else if(node->unode.nt.symbol==iterativeStmt){ //iterativeStmt
        astNode * it ;
        it  = node->child;
        idSTEntry * ide = NULL ;
        if(it->isLeaf ==1 && it->unode.ln.t.term == ID){ 
            idST * i = lookUpID( it->unode.ln.t.lexeme, idt);
            if(i == NULL){
               //ERR0R
            if(errorFlag==1){
            printf("LINE NO. : %d :ERROR: ",it->unode.ln.t.lineno);
            printf("variable %s undeclared\n", it->unode.ln.t.lexeme);}
            hasSemanticError = 1;
            
            }
            else{
                it->idt = i;
                ide = lookupidST(i,it->unode.ln.t.lexeme);
                ide->isAssigned += 100;                    
            }
        }

        else{

            ariBool(node->child,idt,mt,errorFlag);
            
        }
    
        if(node->ruleNum ==1)
            it = node->child->sibling;//statements
        else
            it = node->child->sibling->sibling;//statements
        
        idST *tmp= createidST(),*tmp1;
        tmp->start=node->idt->start;
        tmp->end=node->idt->end;

        if(idt->child==NULL)
            idt->child=tmp;
        else
        {
            tmp1=idt->child;
            while(tmp1->sibling!=NULL)
                tmp1=tmp1->sibling;
            
            tmp1->sibling=tmp;
        }
          
        tmp->parent=idt;
        while(it!=NULL){
            populateSTStmt(it,tmp,mt,min,errorFlag);
            it=it->sibling;
        }
        if(ide != NULL) ide->isAssigned -= 100;
    }
    
    else if(node->unode.nt.symbol==conditionalStmt) //conditionalStmt
    {   
        astNode * tmp1 = node->child;
        idST * i = lookUpID( tmp1->unode.ln.t.lexeme, idt);
        if(i == NULL){
           //ERR0R
           if(errorFlag==1){
            printf("LINE NO. : %d :ERROR: ",tmp1->unode.ln.t.lineno);
            printf("variable %s undeclared\n", tmp1->unode.ln.t.lexeme);}
            hasSemanticError = 1;
           
        }
        else{
            tmp1->idt = i;
        }    
        idSTEntry* newt = lookupidST(i,tmp1->unode.ln.t.lexeme);
        if(newt->dataType != INTEGER && newt->dataType != BOOLEAN){
            return;
        }
        tmp1=node->child->sibling;//<caseStmts>
        astNode *tmp2=tmp1->sibling;//default
        
        idST *tmpid=NULL;
        astNode *iter=tmp1->child; //<caseSingle>

        idST *tempo;
        idST *tmp= createidST();

        tmp->start=node->idt->start;
        tmp->end=node->idt->end;
        tempo = idt;

        if(idt->child==NULL)
            idt->child=tmp;
        else
        {
            tempo=idt->child;
            while(tempo->sibling!=NULL)
                tempo=tempo->sibling;
            
            tempo->sibling=tmp;
        }
          
        tmp->parent=idt;

        tempo = tmp;
        
        astNode *it = iter->child->sibling; //<statements>
        
        while(it!=NULL){
            populateSTStmt(it,tempo,mt,min,errorFlag);
            it=it->sibling;
        }
        
        if(iter!=NULL && iter->sibling!=NULL)
        {
            iter=iter->sibling->child; //
            while(iter!=NULL)
            {
                if(iter->child != NULL){
                    it = iter->child->sibling;
                    while(it!=NULL){
                         populateSTStmt(it,tempo,mt,min,errorFlag);
                         it=it->sibling;
                    }
                }
                if(iter->sibling != NULL)
                    iter=iter->sibling->child;
                else break;
            }
        }
        
        if(tmp2!=NULL){
            tmp2 = tmp2->child->sibling;
            while(tmp2!=NULL){
                populateSTStmt(tmp2,tempo,mt,min,errorFlag);
                tmp2 = tmp2->sibling;
            }
        }
        
    }

    //ioStmt
    else if(node->unode.nt.symbol==ioStmt){
        astNode * temp ;
        char * check ;
        if(node->child->isLeaf == 1 && node->child->unode.ln.t.term == ID){
            temp =  node->child;
            check =  temp->unode.ln.t.lexeme;
            if(lookUpID(check,idt) == NULL){
                //ERROR
                if(errorFlag==1){
                printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                printf("variable %s undeclared\n", check);}
                hasSemanticError = 1;
            }
            else {

                if(lookupidST(lookUpID(check,idt),check)->isAssigned >= 100){
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                    printf("reassignment of loop variable %s invalid\n", check);}
                    hasSemanticError = 1;
                    // return;
                }
                else{

                    pNode * ret = min->outParams;
                    while(ret != NULL){
                        if(strcmp(ret->lexeme,check)==0){
                            ret->isAssigned =1;
                            break;
                        }
                        ret = ret->next;
                    }
                    temp->idt = lookUpID(check,idt);
                }
                //ERROR
            }
        }
               
        
        else if(node->child->isLeaf == 0){
            temp =  node->child;
            if(temp->isLeaf == 0 && temp->unode.nt.symbol == var_id_num){
                varIdNum( temp, idt ,mt,errorFlag);
            }
            
        }
    }
    
    
    //module Reuse Stmt
    else if(node->unode.nt.symbol==moduleReuseStmt){
        astNode * temp =  node->child;
        astNode * modTemp = temp->sibling;
        char * check ;
        
        //checking for <Optional>
        if(temp->isLeaf == 0 && temp->unode.nt.symbol == idList){
            temp =  temp->child; //ID ;
            while(temp != NULL){
                check = temp->unode.ln.t.lexeme;
                if(lookUpID(check,idt) == NULL){
                    if(errorFlag==1){
                        printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                        printf("variable %s undeclared\n", check);
                    }
                    hasSemanticError = 1;
                }
                else{
                    temp->idt = lookUpID(check,idt);
                    idSTEntry *lookup = lookupidST(temp->idt, check);
                    if(lookup->isAssigned >= 100){
                        if(errorFlag==1){
                        printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                        printf("reassignment of loop variable %s invalid\n", lookup->lexeme);}
                        hasSemanticError = 1;
                    }

                    else{

                        pNode * ret = min->outParams;
                        while(ret != NULL){
                            if(strcmp(ret->lexeme,check)==0){
                                ret->isAssigned =1;
                                break;
                            }
                            ret = ret->next;
                        }
                    }

                }

                temp = temp->sibling;
                if(temp != NULL){
                    temp = temp->child;
                }
            }
            
            temp = modTemp;
            //checking for module name and idList
            if(temp->isLeaf == 1 && temp->unode.ln.t.term == ID){ //module ka naam;
                check = temp->unode.ln.t.lexeme ;
                moduleSTEntry *currModule = lookupmoduleST(mt,check);
                if(currModule == NULL){
                    //ERROR
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                    printf("module %s undeclared\n", check);}
                    hasSemanticError = 1;
                }

                else if(strcmp(check,min->lexeme_mod)==0){
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                    printf("module %s invoked recursively.\n", check);}
                    hasSemanticError = 1;

                }

                if(currModule!=NULL && currModule->moduleDefined==0 && currModule->moduleDeclared==1){
                    currModule->isDecValid = 1;

                }

                temp = temp->sibling;
                if(temp != NULL && temp->isLeaf == 0 && temp->unode.nt.symbol == idList){
                temp =  temp->child ;//ID ;
                    while(temp != NULL){
                        check = temp->unode.ln.t.lexeme;
                        if(lookUpID(check,idt) == NULL){
                            if(errorFlag==1){
                            printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                            printf("variable %s undeclared\n", check);}
                            hasSemanticError = 1;
                        }
                        else{
                            temp->idt = lookUpID(check,idt);
                        }
                        temp = temp->sibling;
                        if(temp != NULL){
                            temp = temp->child;
                        }
                    }
                }

            }
            

       }
        
        //checking when no <Optional>
        else if(temp->isLeaf == 1 && temp->unode.ln.t.term == ID){ //module ka naam;
            check = temp->unode.ln.t.lexeme ;
            moduleSTEntry *currModule = lookupmoduleST(mt,check);
            if(currModule == NULL){
                //ERROR
                if(errorFlag==1){
                printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                printf("module %s undeclared\n", check);}
                hasSemanticError = 1;
            }

            else if(strcmp(check,min->lexeme_mod)==0){
                if(errorFlag==1){
                printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                printf("module %s invoked recursively.\n", check);}
                hasSemanticError = 1;

            }

            if(currModule!=NULL && currModule->moduleDefined==0 && currModule->moduleDeclared==1){
                currModule->isDecValid = 1;
            }

            temp = temp->sibling;
            if(temp != NULL && temp->isLeaf == 0 && temp->unode.nt.symbol == idList){
            temp =  temp->child ;//ID ;
                while(temp != NULL){
                    check = temp->unode.ln.t.lexeme;
                    if(lookUpID(check,idt) == NULL){
                        if(errorFlag==1){
                        printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                        printf("variable %s undeclared\n", check);}
                        hasSemanticError = 1;
                    }
                    else{
                        temp->idt = lookUpID(check,idt);
                    }
                    temp = temp->sibling;
                    if(temp != NULL){
                        temp = temp->child;
                    }
                }
            }

        }
    }
    
    //assignmentStmt
    else if(node->unode.nt.symbol==assignmentStmt){
        astNode * temp = node->child;
        char * check ;
        check = temp->unode.ln.t.lexeme ;//ID
    
        if(lookUpID(check,idt) != NULL){
            // printf("FINDINF IN IDT %s\n",check );
            idST * t =  lookUpID(check,idt);
            idSTEntry * lookup = lookupidST(t,check);
            temp->idt = t;
            if(lookup->isAssigned >= 100){
                if(errorFlag==1){
                printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                printf("reassignment of loop variable %s invalid\n", check);}
                hasSemanticError = 1;
            }
            else{

                pNode * ret = min->outParams;
                while(ret != NULL){
                    if(strcmp(ret->lexeme,check)==0){
                        ret->isAssigned =1;
                        break;
                    }
                    ret = ret->next;
                }
            }
        }

        else{
            //ERROR 
            if(errorFlag==1){
            printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
            printf("variable %s undeclared\n", check);}
            hasSemanticError = 1;
        }
        
        temp =  temp->sibling; //<whichStmt>
        if(temp->unode.nt.symbol == lvalueARRStmt){
            //checking for <index>
            if(temp->child->unode.ln.t.term == ID){
                check = temp->child->unode.ln.t.lexeme;
                if(lookUpID(check,idt) == NULL){
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",temp->child->unode.ln.t.lineno);
                    printf("variable %s undeclared\n", check);}
                    hasSemanticError = 1;
                }
                else{
                    temp->child->idt = lookUpID(check,idt);
                }
            }
            // ERRORS AUR BHI
            
            temp = temp->child->sibling ; //<expression>
            if(temp->unode.nt.symbol == U){
                astNode *newNT = temp->child->sibling;//newNt
                if(newNT->unode.nt.symbol == var_id_num){
                    temp = newNT->child;//ID
                    check = temp->unode.ln.t.lexeme ;//ID
                    if(lookUpID(check,idt) != NULL){
                        idST * t =  lookUpID(check,idt);
                        idSTEntry * insert = lookupidST(t,check);
                        temp->idt = t;
                    }
                    else{
                        //ERROR 
                        if(errorFlag==1){
                        printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                        printf("variable %s undeclared\n", check);}
                        hasSemanticError = 1;
                    }
                    temp=temp->sibling;//whichid;
                }
            }
            else
            {
                //ArithmeticOrBool
                ariBool(temp,idt,mt,errorFlag);
            }           
           
        }
        
        
        else if(temp->unode.nt.symbol == U)
        {
            astNode *newNT = temp->child->sibling;//newNt
            if(newNT->unode.nt.symbol == var_id_num){
                temp = newNT->child;//ID
                check = temp->unode.ln.t.lexeme ;//ID
                if(lookUpID(check,idt) != NULL){ 
                    idST * t =  lookUpID(check,idt);
                    idSTEntry * insert = lookupidST(t,check);
                    temp->idt = t;
                }
                else{
                    //ERROR 
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
                    printf("variable %s undeclared\n", check);}
                    hasSemanticError = 1;
                }
                temp=temp->sibling;//whichid;
            }

            else if(newNT->isLeaf == 1){
                ;
            }

            else{
                trm(newNT,idt,mt,errorFlag);
            }
        }
        
        else 
        { //lvalueIDStmt -> expr
            ariBool(temp,idt,mt,errorFlag);
           
        }
    }
}


void anyTerm(astNode *temp, idST *idt , moduleST * mt, int errorFlag)
{
    if(temp->isLeaf==1)
    {
        if(temp->unode.ln.t.term==TRUE)
        {
            ;
        }
        else if(temp->unode.ln.t.term==FALSE)
        {
            ;
        }
    }
    
    else {
        ariExp(temp,idt,mt,errorFlag);
    }
    
}

void ariExp(astNode *temp, idST *idt , moduleST * mt, int errorFlag)
{
    if(temp->unode.nt.symbol==relationalOp)
    {
        //N7 null and N8 not.
        trm(temp->child->sibling,idt,mt,errorFlag);
        trm(temp->child->sibling->sibling,idt,mt,errorFlag);
    }
    else {
        trm(temp,idt,mt,errorFlag);
    }
    
}

void trm(astNode *temp, idST *idt , moduleST * mt, int errorFlag)
{
    if(temp->unode.nt.symbol==op1)
    {
        //N7 null and N8 NULL and N4 not.
        while(temp->child->sibling->unode.nt.symbol!=factor)
        {
            fact(temp->child->sibling->sibling,idt,mt,errorFlag);
            temp=temp->child->sibling;
        }
        fact(temp->child->sibling,idt,mt,errorFlag);
        fact(temp->child->sibling->sibling,idt,mt,errorFlag);
    }
    
    else{
        fact(temp,idt,mt,errorFlag);
    }
}

void fact(astNode *temp, idST *idt , moduleST * mt, int errorFlag)
{
    if(temp->unode.nt.symbol==op2)
    {
        //N7 null and N8 NULL and N4 NULL and N5 not.
        while(temp->child->sibling->unode.nt.symbol!=factor)
        {
            if(temp->child->sibling->sibling->child->unode.nt.symbol==var_id_num)
            {
                varIdNum(temp->child->sibling->sibling->child,idt,mt,errorFlag);
            }
            else
            {
                ariBool(temp->child->sibling->sibling->child,idt,mt,errorFlag);
            }
            temp=temp->child->sibling;
            if(temp->child->sibling==NULL){
                break;
                }
        }
        
        if(temp->child->sibling->child->unode.nt.symbol==var_id_num)
        {
            varIdNum(temp->child->sibling->child,idt,mt,errorFlag);
        }
        
        else
        {
            ariBool(temp->child->sibling->child,idt,mt,errorFlag);
        }
        
        if(temp->child->sibling->sibling->child->unode.nt.symbol==var_id_num)
        {
            varIdNum(temp->child->sibling->sibling->child,idt,mt,errorFlag);
            
        }
        
        else
        {
            // printf("IN FACTOR IF else 2\n");
            ariBool(temp->child->sibling->sibling->child,idt,mt,errorFlag);
        }
        
    }
    else if(temp->child->unode.nt.symbol==var_id_num)
    {
        //N7 null and N8 NULL and N4 NULL and N5 NULL. factor->varidnum;

        varIdNum(temp->child,idt,mt,errorFlag);
    }
    else{
        ariBool(temp->child,idt,mt,errorFlag);
    }
    
}


void ariBool(astNode *temp, idST *idt , moduleST * mt, int errorFlag)
{
    if(temp==NULL)
        return;
    if(temp->isLeaf==1)
    {
        temp->idt = idt;
        if(temp->unode.ln.t.term==TRUE)
        {
            //1st N7 is null anyTerm->boolConst->t/f
            ;
        }
        else if(temp->unode.ln.t.term==FALSE)
        {
            //1st N7 is null anyTerm->boolConst->t/f
            ;
        }
        else if(temp->unode.ln.t.term==NUM)
        {
            //1st N7 is null anyTerm->Arithmtic exp  and N8 NULL and N4 NULL and N5 NULL 2) get var_id_num
            //2 cases of num and rnum 3rd aint leaf.
            ;
        }
        else if(temp->unode.ln.t.term==RNUM)
        {
            ;
        }
        else 
        {
            //1st N7 is null anyTerm->Arithmtic exp vala and N8 NULL and N4 NULL and N5 NULL 1)get AriOBool back
            ariBool(temp,idt,mt,errorFlag);
        }
        //N5 not null wont be a leaf same for N4 and N8 and N7
    }
    else{
    
        if(temp->unode.nt.symbol==logicalOp)
        {
            while(temp->child->sibling->unode.nt.symbol==logicalOp)
            {
                
                //AnyTerm Node (True False Case)
                anyTerm(temp->child->sibling->sibling,idt,mt,errorFlag);
                //AnyTerm
                
                temp=temp->child->sibling;
                
            }
            
            anyTerm(temp->child->sibling,idt,mt,errorFlag);
            anyTerm(temp->child->sibling->sibling,idt,mt,errorFlag);
            
        }
        else {
            anyTerm(temp,idt,mt,errorFlag);
        }
    
    }
    
}

void varIdNum(astNode *temp, idST *idt , moduleST * mt,  int errorFlag)
{
    if(temp->isLeaf == 0)
        temp = temp->child;

    if(temp->unode.ln.t.term != ID)
    {
        return;
    }
    
    astNode *val = temp;//id;
                        
    char *check = val->unode.ln.t.lexeme ;//ID
    if(lookUpID(check,idt) != NULL){
        idST * t =  lookUpID(check,idt);
        idSTEntry * insert = lookupidST(t,check);
        val->idt =t;
    }
    else{
        //ERROR 
        if(errorFlag==1){
        printf("LINE NO. : %d :ERROR: ",val->unode.ln.t.lineno);
        printf("variable %s undeclared\n", check);}
        hasSemanticError = 1;
    }
    
    astNode *val1 = temp->sibling;//whichId;                        
    
    if(val1 != NULL){
        if(val1->unode.ln.t.term==ID)
        {
            check = val1->unode.ln.t.lexeme ;//ID
            if(lookUpID(check,idt) != NULL){
                fflush(stdout);
                idST * t =  lookUpID(check,idt);
                idSTEntry * insert = lookupidST(t,check);   
                val1->idt = t;
            }
            else{
                //ERROR 
                if(errorFlag==1){
                printf("LINE NO. : %d :ERROR: ",val1->unode.ln.t.lineno);
                printf("variable %s undeclared\n", check);}
                hasSemanticError = 1;
            }
        }
    }
}

void populatemoduleST(astNode *node , moduleST * mt,int errorFlag){
    
    if(node->isLeaf==0 && node->unode.nt.symbol==moduleDeclarations){
        astNode * temp = node->child;

        while(temp!=NULL){
            moduleSTEntry * mSTentry = (moduleSTEntry *) malloc(sizeof(moduleSTEntry));
            strcpy(mSTentry->lexeme_mod,temp->unode.ln.t.lexeme);
            mSTentry->t = temp->unode.ln.t ;
            mSTentry->moduleDefined = 0;
            mSTentry->moduleDeclared = 1;
            int wid = 0;
            mSTentry->moduleSize = wid;
            insertInmoduleST( mt, mSTentry);

            temp=temp->sibling;

        }
    }
    
    //module
    else if(node->isLeaf==0 && node->unode.nt.symbol==module){
        astNode *moduleID = node->child;
        // ERROR : IF IT IS NOT ALREADY DEFINED
        moduleSTEntry * check = lookupmoduleST(mt, moduleID->unode.ln.t.lexeme);


        if( check!= NULL && check->moduleDefined == 1){ //module with same name Defined earlier
            if(errorFlag==1){
            printf("LINE NO. : %d :ERROR: ",moduleID->unode.ln.t.lineno);
            printf("module %s cannot be overloaded\n", moduleID->unode.ln.t.lexeme);}
            hasSemanticError = 1;
            return;
        }

        if(check!=NULL && check->moduleDeclared==1 && check->isDecValid==0){
            if(errorFlag==1){
            printf("LINE NO. : %d :ERROR: ",moduleID->unode.ln.t.lineno);
            printf("module %s definition and its declaration both appear before its call\n",check->lexeme_mod);}
            hasSemanticError = 1;        
        }

        
        
        if(check!= NULL && check->moduleDefined == 0){
            OFFSET =0;
            check->moduleSize = 0;
            astNode *moduleDef;
            check->t = moduleID->unode.ln.t; 
            check->idt = createidST();
            check->idt->start=node->idt->start;
            check->idt->end=node->idt->end;
            check->outParams = NULL;
            check->inParams = NULL;
            populateParameterlist(moduleID->sibling, check,errorFlag); //inParams
            
            if((moduleID->sibling->sibling!=NULL)&&(moduleID->sibling->sibling)->unode.nt.symbol==ret){
                astNode *ret= moduleID->sibling->sibling;
                populateParameterlist(ret->child, check,errorFlag); //outParams
                moduleDef = ret->sibling;
            }            
            else{
                moduleDef = moduleID->sibling->sibling;
            }
            
            check->moduleDefined = 1;
            idST * new = createidST();
            new->start=node->idt->start;
            new->end=node->idt->end;
            new->parent = check->idt;
            check->idt->child = new;
            while(moduleDef!=NULL){
                populateSTStmt(moduleDef,new,mt,check,errorFlag); 
                moduleDef=moduleDef->sibling;
            }

            int wid = 0;
            findWidth(check->idt,&wid);
            check->moduleSize = wid;
            pNode *out = check->outParams;
            while(out != NULL){
                if(out->isAssigned == 0){
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",check->idt->end);
                    printf("output parameter %s not assigned any value inside function\n",out->lexeme);}
                    hasSemanticError = 1;
                }
            
            out = out->next;
            }
        
        }
        
        else{
            OFFSET =0;
            moduleSTEntry * mSTentry = (moduleSTEntry *)malloc(sizeof(moduleSTEntry));
            mSTentry->idt = createidST();
            mSTentry->idt->start=node->idt->start;
            mSTentry->idt->end=node->idt->end;
            astNode *moduleDef;
            // Making moduleSTEntry
            strcpy(mSTentry->lexeme_mod,moduleID->unode.ln.t.lexeme); //name of module i.e key
            mSTentry->t = moduleID->unode.ln.t ;
            mSTentry->inParams = NULL;
            mSTentry->outParams = NULL;
            mSTentry->moduleSize = 0;
            populateParameterlist(moduleID->sibling, mSTentry,errorFlag); //inParams
            if((moduleID->sibling->sibling!=NULL)&&(moduleID->sibling->sibling)->unode.nt.symbol==ret){
                astNode *ret= moduleID->sibling->sibling;
                populateParameterlist(ret->child, mSTentry,errorFlag); //outParams
                moduleDef = ret->sibling;
            }            
            else{
                moduleDef = moduleID->sibling->sibling;
            }
            // Include in hashtable moduleDefined   
            mSTentry->moduleDefined = 1;
            mt = insertInmoduleST(mt, mSTentry); 
            
            idST * new = createidST();
            new->parent = mSTentry->idt;
            new->start=node->idt->start;
            new->end=node->idt->end;
            mSTentry->idt->child = new;
            while(moduleDef!=NULL){
                populateSTStmt(moduleDef,new,mt,mSTentry,errorFlag); 
                moduleDef=moduleDef->sibling;
            }
            int wid = 0;
            mSTentry = lookupmoduleST(mt,mSTentry->lexeme_mod);
            findWidth(mSTentry->idt,&wid);
            mSTentry->moduleSize = wid;

            pNode *out = mSTentry->outParams;
            while(out != NULL){
                if(out->isAssigned == 0){
                    if(errorFlag==1){
                    printf("LINE NO. : %d :ERROR: ",mSTentry->idt->end);
                    printf("output parameter %s not assigned any value inside function\n",out->lexeme);}
                    hasSemanticError = 1;
                }
            
            out = out->next;
            }
        }
    }
    
    //driver Module
    else if(node->isLeaf==0 && node->unode.nt.symbol==driverModule){
        OFFSET = 0;
        astNode *tmp = node->child;//moduledef ie stmt array;
        moduleSTEntry * mSTentry = (moduleSTEntry *)malloc(sizeof(moduleSTEntry));
        mSTentry->idt = createidST();
        mSTentry->idt->start=node->idt->start;
        mSTentry->idt->end=node->idt->end;
        mSTentry->idt->nesting = 1; 
        mSTentry->outParams = NULL;
        mSTentry->inParams = NULL;
        mSTentry->moduleDefined = 1;
        strcpy(mSTentry->lexeme_mod,"123driver"); 
        
        // ONLY ONE DRIVER MODULE IN A PROGRAM
        if(lookupmoduleST(mt, mSTentry->lexeme_mod)!=NULL){
            if(errorFlag==1){
            printf("LINE NO. : %d :ERROR: ",mSTentry->idt->start - 1);
            printf("driver Module Re-definition\n");}
            hasSemanticError = 1;
            return;
        }
        
        mt = insertInmoduleST(mt, mSTentry);
        while(tmp!=NULL){
            populateSTStmt(tmp,mSTentry->idt,mt,mSTentry,errorFlag); 
            tmp=tmp->sibling;
        }

        int wid = 0;
        mSTentry = lookupmoduleST(mt,mSTentry->lexeme_mod);
        findWidth(mSTentry->idt,&wid);
        mSTentry->moduleSize = wid;
    }
}

void printSymbolTable(moduleST * hash)
{   
    printf("%-15s%-24s%-17s%-15s%-15s%-15s%-20s%-15s%-20s%-15s\n\n\n","variable_name","scope(module_name)","scope(line_nos)","width","isArray","static_or_dynamic","range_lexemes","type_of_element"," offset","nesting_level");

    hashTable_func * temp = hash->modHashTable;
    for(int i =0 ;i < N ; i++){
        if(temp[i].str!=NULL){
            hashTable_func  * tempmod = &temp[i];
            while(tempmod!=NULL && tempmod->str!=NULL){
                printIDT(tempmod->data.idt, tempmod->data);
                tempmod =tempmod->next;
            }
        }
    }

}

void printModule(moduleSTEntry mod)
{

    printf("Inside Module : %s\n",mod.lexeme_mod);
    printf("%d\n",mod.moduleSize );
    pNode *tmp=mod.inParams;
    while(tmp!=NULL)
    {
        printf("** %s, type:%s\n",tmp->lexeme, enumToString(tmp->dataType));
        tmp=tmp->next;
    }
    
    printf("Output Para : \n");
    tmp=mod.outParams;
    while(tmp!=NULL)
    {
        printf("** %s, datatype: %s\n",tmp->lexeme, enumToString(tmp->dataType));
        tmp=tmp->next;
    }
    //moodule ka done ab bache...    
    printIDT(mod.idt,mod);
    printf("Defined :%d\n", mod.moduleDefined);
    printf("Declared :%d\n\n", mod.moduleDeclared);
    
}

void printIDT(idST *idt, moduleSTEntry mod)
{

    if(idt==NULL){

        return;

    } 
    if(idt == mod.idt->child){
        idt->nesting =1;
    }
    char * module_name = (char*)malloc(50 * sizeof(char));
    if(strcmp("123driver",mod.lexeme_mod)==0){
        strcpy(module_name,"driver");
    }
    else{
        strcpy(module_name,mod.lexeme_mod);
    }
    hashTable_id  * val = idt->idHashTable;
    for(int i = 0 ; i< N ; i++){
        if(val[i].str != NULL){
            hashTable_id  * temp = &val[i];
            
            while(temp != NULL && temp->str!=NULL){
                printf("%-15s%-24s%-3s%-3d-%-15d%-15d",temp->str, module_name,"",idt->start,idt->end,temp->data.width);
                if(temp->data.isArray ==1){

                printf("%-15s","yes");

                if(temp->data.isStatic ==0){
                    printf("%-15s","dynamic");
                }
                else{
                    printf("%-15s","static");
                }
                if(temp->data.start_pop == 1)
                    printf("[%-3d",temp->data.start.num);
                else if(temp->data.start_pop == 0 && temp->data.start.entry!= NULL)
                    printf("[%-3s",temp->data.start.entry->t.lexeme);

                if(temp->data.end_pop == 1)
                    printf(" - %3d%-2s",temp->data.end.num,"]");
                else if(temp->data.end_pop == 0 && temp->data.end.entry!= NULL)
                    printf(" - %3s%-2s",temp->data.end.entry->t.lexeme,"]");

                }
                else{
                    printf("%-16s%-19s%-7s","no","-----","-----");
                }

                if(idt == mod.idt && idt->nesting == 0)
                printf("%10s%-15s%-15d%-15d\n\n","",enumToString(temp->data.dataType),temp->data.offset,idt->nesting);
                else{
                     printf("%10s%-15s%-15d%-15d\n\n","",enumToString(temp->data.dataType),temp->data.offset-mod.decOffset,idt->nesting);
                }
                temp=temp->next;
            }
        }
    }
    idST *tmp=idt->child;
    while(tmp!=NULL)
    {
        tmp->nesting = idt->nesting +1;
        printIDT(tmp,mod);
       
        tmp=tmp->sibling;
    }
}

void printActivationRecSize(moduleST *hash){
    printf("\n\n\n");
    char * module_name = (char*)malloc(50 * sizeof(char));
    
    hashTable_func * temp = hash->modHashTable;
    for(int i =0 ;i < N ; i++){
        if(temp[i].str!=NULL){
            hashTable_func  * tempmod = &temp[i];
            while(tempmod!=NULL && tempmod->str!=NULL){
                if(strcmp("123driver",tempmod->str)==0){
                    strcpy(module_name,"driver");
                }
                else{
                    strcpy(module_name,tempmod->str);
                }
                printf("module name:   %-28s\tactivation record size:   %-28d\n",module_name,tempmod->data.moduleSize-tempmod->data.decOffset);
                tempmod =tempmod->next;
            }
        }
    }


}

void printArrays(moduleST * hash){

    printf("%-28s%-18s%-14s%-22s%-22s%-18s\n\n\n","module_name","scope(line_nos)","variable_name","static_or_dynamic","range_lexemes","type_of_element");

    hashTable_func * temp = hash->modHashTable;
    for(int i =0 ;i < N ; i++){
        if(temp[i].str!=NULL){
            hashTable_func  * tempmod = &temp[i];
            while(tempmod!=NULL && tempmod->str!=NULL){
                arrayHelper(tempmod->data.idt, tempmod->data);
                tempmod =tempmod->next;
            }
        }
    }
    
}



void  arrayHelper(idST *idt, moduleSTEntry mod)
{

    if(idt==NULL){

        return;
    } 
    char * module_name = (char*)malloc(50 * sizeof(char));
    if(strcmp("123driver",mod.lexeme_mod)==0){
        strcpy(module_name,"driver");
    }
    else{
        strcpy(module_name,mod.lexeme_mod);
    }
    hashTable_id  * val = idt->idHashTable;
    for(int i = 0 ; i< N ; i++){
        if(val[i].str != NULL){
            hashTable_id  * temp = &val[i];
            while(temp != NULL && temp->str!=NULL){
                
                if(temp->data.isArray ==1){
                    printf("%-28s%-10s%-3d-%-15d%-10s",module_name,"",idt->start,idt->end,temp->str);
                

                    if(temp->data.isStatic ==0){
                        printf("%-18s","dynamic_array");
                    }
                    else{
                        printf("%-18s","static_array");
                    }

                    if(temp->data.start_pop == 1)
                        printf("[%-3d",temp->data.start.num);
                    else if(temp->data.start_pop == 0 && temp->data.start.entry!= NULL)
                        printf("[%-3s",temp->data.start.entry->t.lexeme);

                    if(temp->data.end_pop == 1)
                        printf(" - %3d%-2s",temp->data.end.num,"]");
                    else if(temp->data.end_pop == 0 && temp->data.end.entry!= NULL)
                        printf(" - %3s%-2s",temp->data.end.entry->t.lexeme,"]");

                     printf("%5s%-10s\n\n","",enumToString(temp->data.dataType));
                }


                temp=temp->next;
            }
        }
    }
    idST *tmp=idt->child;
    while(tmp!=NULL)
    {
        tmp->nesting = idt->nesting +1;
        arrayHelper(tmp,mod);
       
        tmp=tmp->sibling;
    }
}

void populateSymbolTable(astNode * root, int errorFlag){
    functionST = createmoduleST();
    traverseAST(root, functionST,errorFlag);
    return ;
}

void traverseAST(astNode* root, moduleST* mt, int errorFlag){

    if(root==NULL) return;
    
    if(root->child == NULL ){
        if(root->isLeaf == 0 && root->unode.nt.symbol==driverModule)
            populatemoduleST(root,mt,errorFlag);
    }
    
    else{
        traverseAST(root->child,mt,errorFlag);
        astNode* temp=root->child;
        temp=temp->sibling;
        while(temp != NULL){
            traverseAST(temp,mt,errorFlag);
            temp=temp->sibling;
        }
        if(root->isLeaf == 0 && (root->unode.nt.symbol == moduleDeclarations || root->unode.nt.symbol == module || root->unode.nt.symbol ==driverModule))
            populatemoduleST(root,mt,errorFlag);
    
    }
}

moduleST * getmt(){
    return functionST;
}

void findWidth(idST * in , int * x){

   idST * root = in;
    
    hashTable_id  * val = root->idHashTable;
    for(int i = 0 ; i< N ; i++){
        if(val[i].str != NULL){
            hashTable_id  * temp = &val[i];
            while(temp != NULL && temp->str!=NULL){

                *x  = *x + temp->data.width;
                temp=temp->next;
            }
        }
    }        
    
    idST *tmp=root->child;
    while(tmp!=NULL)
    {
        findWidth(tmp, x);
        tmp=tmp->sibling;
    } 

} 