// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH

//ast.h
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"


// # function prototypes

astNode *createNewLeaf(token t){
    astNode * newLeaf = (astNode *)malloc(sizeof(astNode));
    newLeaf->isLeaf  = 1;
    newLeaf->ruleNum = 0;
    newLeaf->unode.ln.t = t;
    newLeaf->parent = NULL;
    newLeaf->child = NULL;
    newLeaf->sibling = NULL;
    newLeaf->idt = NULL;
    astMemUsage += sizeof(astNode);
    numAstNodes +=1;


    return newLeaf ;
}

astNode * createNewNode(Term term, astNode *node){

    astNode *newNode = (astNode *)malloc(sizeof(astNode));
    newNode->isLeaf=0;
    newNode->ruleNum = 0;
    newNode->unode.nt.symbol = term;
    newNode->parent = NULL;
    newNode->child = node;
    newNode->sibling = NULL;
    newNode->idt = NULL;
    astMemUsage += sizeof(astNode);
    numAstNodes +=1;
    return newNode ;
}

astNode * populateAST(treeNode * root){
    if(root != NULL && root->isTerminal== 0){
    // rule 1
        //"<program> → <moduleDeclarations> <otherModules> <driverModule> <otherModules>"

        if(root->unode.nt.value == program){
           treeNode * child1 = root->unode.nt.child;
           treeNode * child2 = child1->unode.nt.sibling;
           treeNode * child3 = child2->unode.nt.sibling;
           treeNode * child4 = child3->unode.nt.sibling; 
           populateAST(child1);
           populateAST(child2);
           populateAST(child3);
           populateAST(child4);

           if(child1->anode==NULL)
           {
               if(child2->anode==NULL)
               {    
                    if(child3->anode!=NULL){
                        root->anode = createNewNode(program,child3->anode);
                        child3->anode->sibling = child4->anode ;
                    }
                    else
                    root->anode = createNewNode(program,child4->anode);   
               }
               else
               {
                   astNode *tmp = createNewNode(otherModules, child2->anode);
                   root->anode = createNewNode(program,tmp);
                   root->anode->child->sibling = child3->anode ;
                   if(child3->anode!=NULL)
                        child3->anode->sibling = child4->anode ;
                   else
                        root->anode->child->sibling = child4->anode ;

               }

           }            
           else
           {
               astNode *tmp = createNewNode(moduleDeclarations, child1->anode);
               root->anode = createNewNode(program,tmp);
               if(child2->anode==NULL){
                   root->anode->child->sibling=child3->anode;  
                   if(child3->anode !=NULL){             
                   child3->anode->sibling=child4->anode;
                    }
                   else{
                    tmp->sibling =child4->anode ;
                   }
               }
               else                   
               {
                   tmp = createNewNode(otherModules, child2->anode);
                   root->anode->child->sibling = tmp;
                   tmp->sibling=child3->anode;
                if(child3->anode != NULL)
                   child3->anode->sibling=child4->anode;
                else
                      tmp->sibling =child4->anode ;
               }

           }
        }

        //<moduleDeclarations> → <moduleDeclaration> <moduleDeclarations>
        else if(root->unode.nt.value == moduleDeclarations && root->unode.nt.child->unode.nt.value==moduleDeclaration){
             treeNode * child1 = root->unode.nt.child;
             treeNode * child2 = child1->unode.nt.sibling;
             populateAST(child1);
             populateAST(child2);
             root->anode = child1->anode;
             child1->anode->sibling = child2->anode;
        }

       //<moduleDeclarations> → e
        else if(root->unode.nt.value == moduleDeclarations && root->unode.nt.child->unode.ln.t.term==e){
                root->anode=NULL;
        }

        //<moduleDeclaration> → DECLARE MODULE ID SEMICOL
        else if(root->unode.nt.value == moduleDeclaration)
        {
            treeNode *child1=root->unode.nt.child;
            treeNode *child2=child1->unode.ln.sibling;
            treeNode *child3=child2->unode.ln.sibling;

            root->anode = createNewLeaf(child3->unode.ln.t);

        }

       //<otherModules> → <module> <otherModules>
        else if(root->unode.nt.value == otherModules && root->unode.nt.child->unode.nt.value==module){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 =  child1->unode.nt.sibling;
            populateAST(child1);
            populateAST(child2);

            // module never null
            root->anode = child1->anode;
            child1->anode->sibling = child2->anode;

        }

        //<otherModules> → e
        else if(root->unode.nt.value == otherModules && root->unode.nt.child->unode.ln.t.term==e){
            root->anode=NULL;
        }

        //<driverModule> → DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
        else if(root->unode.nt.value == driverModule){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 = child1->unode.ln.sibling;
            treeNode * child3 = child2->unode.ln.sibling;
            treeNode * child4 = child3->unode.ln.sibling;
            treeNode * child5 = child4->unode.ln.sibling;
            populateAST(child5);

            root->anode = createNewNode(driverModule, child5->anode);


            root->anode->idt = createidST();
            root->anode->idt->start = (child5->unode.nt.child)->unode.ln.t.lineno;
            root->anode->idt->end = (((child5->unode.nt.child)->unode.ln.sibling)->unode.nt.sibling)->unode.ln.t.lineno;//end

        }

        //<module> → DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
        else if(root->unode.nt.value==module){
            treeNode *child1 = root->unode.nt.child;//DEF
            treeNode *child2=child1->unode.ln.sibling;//MODULE
            treeNode *child3=child2->unode.ln.sibling;//ID
            treeNode *child4=child3->unode.ln.sibling;//ENDDEF
            treeNode *child5=child4->unode.ln.sibling;//TAKES
            treeNode *child6=child5->unode.ln.sibling;//INPUT
            treeNode *child7=child6->unode.ln.sibling;//SQBO
            treeNode *child8=child7->unode.ln.sibling;//<iplist>
            treeNode *child9=child8->unode.nt.sibling;//SQBC
            treeNode *child10=child9->unode.ln.sibling;//SEMICOL
            treeNode *child11=child10->unode.ln.sibling;//<ret>
            treeNode *child12=child11->unode.nt.sibling;//<md>

            astNode *tmp = createNewLeaf(child3->unode.ln.t);
            populateAST(child8);
            populateAST(child11);
            populateAST(child12);
            root->anode = createNewNode(module, tmp);
            root->anode->child->sibling = child8->anode;
            child8->anode->sibling = child11->anode;
            if(child11->anode!=NULL)
                child11->anode->sibling = child12 -> anode;
            else 
                child8->anode->sibling = child12 -> anode;


            root->anode->idt = createidST();
            root->anode->idt->start = (child12->unode.nt.child)->unode.ln.t.lineno;
            root->anode->idt->end = (((child12->unode.nt.child)->unode.ln.sibling)->unode.nt.sibling)->unode.ln.t.lineno;//end
        }

        //<ret> → RETURNS SQBO <output_plist> SQBC SEMICOL
        else if(root->unode.nt.value == ret && root->unode.nt.child->unode.ln.t.term==RETURNS){
             treeNode * child1 = root->unode.nt.child; //RETURNS
             treeNode * child2 = child1->unode.ln.sibling; //SQBO
             treeNode * child3 = child2->unode.ln.sibling; // output_plist
             treeNode * child4 = child3->unode.nt.sibling; //SQBC
             treeNode * child5 = child4->unode.ln.sibling; //SEMICOL

             populateAST(child3);
             root->anode = createNewNode(ret,child3->anode);

        }
        //<ret> → e
        else if(root->unode.nt.value == ret && root->unode.nt.child->unode.ln.t.term==e){
            root->anode= NULL;
        }

        //<input_plist> → ID COLON <dataType> <N1>
        else if(root->unode.nt.value == input_plist){
             treeNode * child1 = root->unode.nt.child; //ID
             treeNode * child2 = child1->unode.ln.sibling; //COLON
             treeNode * child3 = child2->unode.ln.sibling; // dataType
             treeNode * child4 = child3->unode.nt.sibling; //N1

            astNode *tmp = createNewLeaf(child1->unode.ln.t);
            populateAST(child3);
            populateAST(child4);

            root->anode = createNewNode(input_plist, tmp);
            root->anode->child->sibling = child3->anode;
            child3->anode->sibling = child4->anode;

        }

        //<N1> → COMMA ID COLON <dataType> <N1>
        else if(root->unode.nt.value == N1 && root->unode.nt.child->unode.ln.t.term==COMMA){
             
             treeNode * child1 = root->unode.nt.child; //COMMA
             treeNode * child2 = child1->unode.ln.sibling; //ID
             treeNode * child3 = child2->unode.ln.sibling; // COLON
             treeNode * child4 = child3->unode.ln.sibling; //dataType
             treeNode * child5 = child4->unode.nt.sibling; //N1
            astNode *tmp = createNewLeaf(child2->unode.ln.t);
            populateAST(child4);
            populateAST(child5);
            root->anode = createNewNode(N1, tmp);
            root->anode->child->sibling = child4->anode;
            child4->anode->sibling = child5->anode;

        }

        //<N1> → e
        else if(root->unode.nt.value == N1 && root->unode.nt.child->unode.ln.t.term==e){
            root->anode= NULL;
        }

        //<output_plist> → ID COLON <type> <N2>
        else if(root->unode.nt.value == output_plist){
             treeNode * child1 = root->unode.nt.child; //ID
             treeNode * child2 = child1->unode.ln.sibling; //COLON
             treeNode * child3 = child2->unode.ln.sibling; // dataType
             treeNode * child4 = child3->unode.nt.sibling; //N2

            astNode *tmp = createNewLeaf(child1->unode.ln.t);
            populateAST(child3);
            populateAST(child4);

            root->anode = createNewNode(output_plist, tmp);
            root->anode->child->sibling = child3->anode;
            child3->anode->sibling = child4->anode;

        }

        //<N2> → COMMA ID COLON <type> <N2>
        else if(root->unode.nt.value == N2 && root->unode.nt.child->unode.ln.t.term==COMMA){
             treeNode * child1 = root->unode.nt.child; //COMMA
             treeNode * child2 = child1->unode.ln.sibling; //ID
             treeNode * child3 = child2->unode.ln.sibling; // COLON
             treeNode * child4 = child3->unode.ln.sibling; //dataType
             treeNode * child5 = child4->unode.nt.sibling; //N2

            astNode *tmp = createNewLeaf(child2->unode.ln.t);
            populateAST(child4);
            populateAST(child5);

            root->anode = createNewNode(N2, tmp);
            root->anode->child->sibling = child4->anode;
            child4->anode->sibling = child5->anode;

        }

        //<N2> → e
        else if(root->unode.nt.value == N2 && root->unode.nt.child->unode.ln.t.term==e){
            root->anode= NULL;
        }

        //<dataType> → ARRAY SQBO <range_arrays> SQBC OF <type>
        else if(root->unode.nt.value == dataType && root->unode.nt.child->unode.ln.t.term==ARRAY){
            treeNode * child1 = root->unode.nt.child; //ARRAY
            treeNode * child2 = child1->unode.ln.sibling; //SQBO
            treeNode * child3 = child2->unode.ln.sibling; // <ra>
            treeNode * child4 = child3->unode.nt.sibling; //SQBC
            treeNode * child5 = child4->unode.ln.sibling; //OF
            treeNode * child6 = child5->unode.ln.sibling; //<type>

            populateAST(child3);
            populateAST(child6);

            root->anode = createNewNode(dataType, child3->anode);
            root->anode->child->sibling = child6->anode;
            root->anode->ruleNum=0;
            root->anode->unode.nt.dataType = child6->anode->unode.ln.t.term;

        }

        //<dataType> → INTEGER
        else if(root->unode.nt.value == dataType && root->unode.nt.child->unode.ln.t.term==INTEGER){
            treeNode * child1 = root->unode.nt.child; //INTEGER

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = INTEGER;
            root->anode->ruleNum = 1; 

        }

        //<dataType> → REAL
        else if(root->unode.nt.value == dataType && root->unode.nt.child->unode.ln.t.term==REAL){
            treeNode * child1 = root->unode.nt.child; //REAL

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = REAL;
            root->anode->ruleNum = 2;

        }

        //<dataType> → BOOLEAN
        else if(root->unode.nt.value == dataType && root->unode.nt.child->unode.ln.t.term==BOOLEAN){
            treeNode * child1 = root->unode.nt.child; //BOOLEAN           

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = BOOLEAN;
            root->anode->ruleNum = 3;

        }

        //<range_arrays> → <index>1 RANGEOP <index>2
        else if(root->unode.nt.value == range_arrays ){
            treeNode * child1 = root->unode.nt.child; //<index>
            treeNode * child2 = child1->unode.nt.sibling; //RANGEOP
            treeNode * child3 = child2->unode.ln.sibling; // <index>

            populateAST(child1);
            populateAST(child3);

            root->anode = createNewNode(range_arrays, child1->anode);
            root->anode->child->sibling = child3->anode;

        }

        //<type> → INTEGER
        else if(root->unode.nt.value == type && root->unode.nt.child->unode.ln.t.term == INTEGER){
            treeNode * child1 = root->unode.nt.child; //INTEGER           
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = INTEGER;
            root->anode->ruleNum =   0;

        }

        //<type> → REAL
        else if(root->unode.nt.value == type && root->unode.nt.child->unode.ln.t.term == REAL){
            treeNode * child1 = root->unode.nt.child; //REAL           

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = REAL;
            root->anode->ruleNum = 1;

        }

        //<type> → BOOLEAN
        else if(root->unode.nt.value == type && root->unode.nt.child->unode.ln.t.term==BOOLEAN){
            treeNode * child1 = root->unode.nt.child; //BOOLEAN           

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = BOOLEAN;
            root->anode->ruleNum = 2;

        }

        //<moduleDef> → START <statements> END
        else if(root->unode.nt.value == moduleDef ){
            treeNode * child1 = root->unode.nt.child; //START
            treeNode * child2 = child1->unode.ln.sibling; //<statements>
            treeNode * child3 = child2->unode.nt.sibling; // END         
            populateAST(child2);
			
            root->anode = child2->anode;
			
			
			

        }

        //<statements> → <statement> <statements>
        else if(root->unode.nt.value == statements && root->unode.nt.child->unode.nt.value==statement){
             treeNode * child1 = root->unode.nt.child;
             treeNode * child2 = child1->unode.nt.sibling;
             populateAST(child1);

             populateAST(child2);
             root->anode = child1->anode;
             child1->anode->sibling = child2->anode;
        }

        //<statements> →e
        else if(root->unode.nt.value == statements && root->unode.nt.child->unode.ln.t.term==e){
            root->anode=NULL;
        }

        //<statement> → <ioStmt>
        else if(root->unode.nt.value == statement && root->unode.nt.child->unode.nt.value==ioStmt){
            treeNode * child1 = root->unode.nt.child; //<ioStmt>        
            populateAST(child1);

            root->anode = child1->anode;            
            
        }

        //<statement> → <simpleStmt>
        else if(root->unode.nt.value == statement && root->unode.nt.child->unode.nt.value==simpleStmt){
            treeNode * child1 = root->unode.nt.child; //<simpleStmt>        
            populateAST(child1);

            root->anode = child1->anode;            
        }

        //<statement> → <declareStmt>
        else if(root->unode.nt.value == statement && root->unode.nt.child->unode.nt.value==declareStmt){
            treeNode * child1 = root->unode.nt.child; //<declareStmt>        
            populateAST(child1);

            root->anode = child1->anode;            
        }

        //<statement> → <conditionalStmt>
        else if(root->unode.nt.value == statement && root->unode.nt.child->unode.nt.value==conditionalStmt){
            treeNode * child1 = root->unode.nt.child; //<conditionalStmt>        
            populateAST(child1);

            root->anode = child1->anode;            
        }

        //<statement> → <iterativeStmt>
        else if(root->unode.nt.value == statement && root->unode.nt.child->unode.nt.value==iterativeStmt){
            treeNode * child1 = root->unode.nt.child; //<iterativeStmt>        
            populateAST(child1);

            root->anode = child1->anode;            
        }

        //<ioStmt> → GET_VALUE BO ID BC SEMICOL
        else if(root->unode.nt.value == ioStmt && root->unode.nt.child->unode.ln.t.term==GET_VALUE){
            treeNode * child1 = root->unode.nt.child; //GET_VALUE
            treeNode * child2 = child1->unode.ln.sibling; //BO
            treeNode * child3 = child2->unode.ln.sibling; //ID
            treeNode * child4 = child3->unode.ln.sibling; //BC
            treeNode * child5 = child4->unode.ln.sibling; //SEMICOL

            root->anode = createNewNode(ioStmt, createNewLeaf(child3->unode.ln.t));            
            root->anode->ruleNum = 0;
        }

        //<ioStmt> → PRINT BO <var> BC SEMICOL
        else if(root->unode.nt.value == ioStmt && root->unode.nt.child->unode.ln.t.term==PRINT){
            treeNode * child1 = root->unode.nt.child; //PRINT
            treeNode * child2 = child1->unode.ln.sibling; //BO
            treeNode * child3 = child2->unode.ln.sibling; //<var>
            treeNode * child4 = child3->unode.nt.sibling; //BC
            treeNode * child5 = child4->unode.ln.sibling; //SEMICOL

            populateAST(child3);
            root->anode = createNewNode(ioStmt, child3->anode);                 
            root->anode->ruleNum = 1;
        }

        //<var> → <var_id_num>
        else if(root->unode.nt.value == var && root->unode.nt.child->unode.nt.value==var_id_num){
            treeNode * child1 = root->unode.nt.child; //<var_id_num>        

            populateAST(child1);
            root->anode = child1->anode;            
        }

        //<var> → <boolConst>
        else if(root->unode.nt.value == var && root->unode.nt.child->unode.nt.value==boolConstt){
            treeNode * child1 = root->unode.nt.child; //<boolConstt>        

            populateAST(child1);
            root->anode = child1->anode;            
        }

        //<boolConst> → TRUE
        else if(root->unode.nt.value == boolConstt && root->unode.nt.child->unode.ln.t.term==TRUE){
            treeNode * child1 = root->unode.nt.child; //TRUE           

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = BOOLEAN;
            root->anode->ruleNum = 0;

        }

        //<boolConst> → FALSE
        else if(root->unode.nt.value == boolConstt && root->unode.nt.child->unode.ln.t.term==FALSE){
            treeNode * child1 = root->unode.nt.child; //FALSE           

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = BOOLEAN;
            root->anode->ruleNum = 1;

        }

        //<var_id_num> → ID <whichId>
        else if(root->unode.nt.value == var_id_num && root->unode.nt.child->unode.ln.t.term==ID){
            treeNode * child1 = root->unode.nt.child; //ID           
            treeNode * child2 = child1->unode.ln.sibling; //<whichId>

            astNode *tmp = createNewLeaf(child1->unode.ln.t);
            populateAST(child2);
            root->anode = createNewNode(var_id_num,tmp);
            root->anode->child->sibling = child2->anode;
            root->anode->ruleNum = 0; 
            
        }

        //<var_id_num> → NUM
        else if(root->unode.nt.value == var_id_num && root->unode.nt.child->unode.ln.t.term==NUM){
            treeNode * child1 = root->unode.nt.child; //NUM         

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = INTEGER;
            root->anode->ruleNum = 1;

        }

        //<var_id_num> → RNUM
        else if(root->unode.nt.value == var_id_num && root->unode.nt.child->unode.ln.t.term==RNUM){
            treeNode * child1 = root->unode.nt.child; //RNUM         

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = REAL;
            root->anode->ruleNum = 2;

        }

        //<whichId> → SQBO <index> SQBC
        else if(root->unode.nt.value == whichId && root->unode.nt.child->unode.ln.t.term==SQBO){
            treeNode * child1 = root->unode.nt.child; //SQBO
            treeNode * child2 = child1->unode.ln.sibling; //<index>
            treeNode * child3 = child2->unode.nt.sibling; // SQBC        

            populateAST(child2);

            root->anode = child2->anode;            

        }

        //<whichId> → e
         else if(root->unode.nt.value == whichId && root->unode.nt.child->unode.ln.t.term==e){

            root->anode=NULL;
        }

        //<simpleStmt> → <assignmentStmt>
        else if(root->unode.nt.value == simpleStmt && root->unode.nt.child->unode.nt.value==assignmentStmt){
            treeNode * child1 = root->unode.nt.child; //<assignmentStmt>        

            populateAST(child1);
            root->anode = child1->anode;            
        }

        //<simpleStmt> → <moduleReuseStmt>
        else if(root->unode.nt.value == simpleStmt && root->unode.nt.child->unode.nt.value==moduleReuseStmt){
            treeNode * child1 = root->unode.nt.child; //<moduleReuseStmt>        

            populateAST(child1);
            root->anode = child1->anode;            
        }

        //<assignmentStmt> → ID <whichStmt>
        else if(root->unode.nt.value == assignmentStmt){
            treeNode * child1 = root->unode.nt.child; //ID           
            treeNode * child2 = child1->unode.ln.sibling; //<whichStmt>
            astNode *tmp = createNewLeaf(child1->unode.ln.t);
            populateAST(child2);
            root->anode = createNewNode(assignmentStmt,tmp);
            root->anode->child->sibling = child2->anode;
        }

        //<whichStmt> → <lvalueIDStmt>
        else if(root->unode.nt.value == whichStmt && root->unode.nt.child->unode.nt.value==lvalueIDStmt){
            treeNode * child1 = root->unode.nt.child; //<lvalueIDStmt>        

            populateAST(child1);
            root->anode = child1->anode;            
        }

        //<whichStmt> → <lvalueARRStmt>
         else if(root->unode.nt.value == whichStmt && root->unode.nt.child->unode.nt.value==lvalueARRStmt){
            treeNode * child1 = root->unode.nt.child; //<lvalueARRStmt>        

            populateAST(child1);
            root->anode = child1->anode;            
        }

        //<lvalueIDStmt> → ASSIGNOP <expression> SEMICOL
        else if(root->unode.nt.value == lvalueIDStmt && root->unode.nt.child->unode.ln.t.term==ASSIGNOP){
            treeNode * child1 = root->unode.nt.child; //ASSIGNOP
            treeNode * child2 = child1->unode.ln.sibling; //<expression>
            treeNode * child3 = child2->unode.nt.sibling; // SEMICOL       

            populateAST(child2);

            root->anode = child2->anode;            

        }

        //<lvalueARRStmt> → SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
        else if(root->unode.nt.value == lvalueARRStmt && root->unode.nt.child->unode.ln.t.term==SQBO){
            treeNode * child1 = root->unode.nt.child; //SQBO
            treeNode * child2 = child1->unode.ln.sibling; //<index>
            treeNode * child3 = child2->unode.nt.sibling; //SQBC
            treeNode * child4 = child3->unode.ln.sibling; //ASSIGNOP
            treeNode * child5 = child4->unode.ln.sibling; //<expression>
            treeNode * child6 = child5->unode.nt.sibling; //SEMICOL

            populateAST(child2);
            populateAST(child5);

            root->anode = createNewNode(lvalueARRStmt, child2->anode);
            root->anode->child->sibling = child5->anode;           

        }

        //<index> → NUM
        else if(root->unode.nt.value == index_ && root->unode.nt.child->unode.ln.t.term==NUM){
            treeNode * child1 = root->unode.nt.child; //NUM         

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = INTEGER;
            root->anode->ruleNum = 0;

        }

        //<index> → ID
        else if(root->unode.nt.value == index_ && root->unode.nt.child->unode.ln.t.term==ID){
            treeNode * child1 = root->unode.nt.child; //ID         

            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 1;

        }

        //<moduleReuseStmt> → <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
        else if(root->unode.nt.value == moduleReuseStmt){
            treeNode * child1 = root->unode.nt.child; //<optional>
            treeNode * child2 = child1->unode.nt.sibling; //USE
            treeNode * child3 = child2->unode.ln.sibling; //MODULE
            treeNode * child4 = child3->unode.ln.sibling; //ID
            treeNode * child5 = child4->unode.ln.sibling; //WITH
            treeNode * child6 = child5->unode.ln.sibling; //PARAMETERS
            treeNode * child7 = child6->unode.ln.sibling; //<idList>
            treeNode * child8 = child7->unode.nt.sibling; //SEMICOL

            populateAST(child1);
            astNode *tmp=createNewLeaf(child4->unode.ln.t);
            populateAST(child7);
            if(child1->anode !=NULL){
                root->anode = createNewNode(moduleReuseStmt,child1->anode);
                root->anode->child->sibling = tmp;
            }
            else
                root->anode = createNewNode(moduleReuseStmt,tmp);
            tmp->sibling = child7->anode;  

        }

        //<optional> → SQBO <idList> SQBC ASSIGNOP 
        else if(root->unode.nt.value == optional && root->unode.nt.child->unode.ln.t.term==SQBO){
            treeNode * child1 = root->unode.nt.child; //<SQBO>   
            treeNode * child2 = child1->unode.ln.sibling; //<idList>

            populateAST(child2);
            root->anode = child2->anode;           
        }

        //<optional> →e
         else if(root->unode.nt.value == optional && root->unode.nt.child->unode.ln.t.term==e){
            root->anode= NULL;
        }

        //<idList> → ID <N3>
        else if(root->unode.nt.value == idList){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 = child1->unode.ln.sibling;
            astNode *tmp = createNewLeaf(child1->unode.ln.t);
            populateAST(child2);
            root->anode = createNewNode(idList,tmp);
            root->anode->child->sibling = child2->anode;
        }
        //<N3> → COMMA ID <N3> (checked)
        else if(root->unode.nt.value == N3 && root->unode.nt.child->unode.ln.t.term==COMMA){
            treeNode * child1 = root->unode.nt.child; //COMMA
            treeNode * child2 = child1->unode.ln.sibling; //ID
            treeNode * child3 = child2->unode.ln.sibling; // <N3>

            astNode * tmp = createNewLeaf(child2->unode.ln.t);
            populateAST(child3);
            root->anode = createNewNode(N3,tmp);
            root->anode->child->sibling = child3->anode;

        }
        //<N3> → e (checked)
        else if(root->unode.nt.value == N3 && root->unode.nt.child->unode.ln.t.term==e){
            root->anode= NULL;

        }
        //<expression> → <arithmeticOrBooleanExpr> (checked)
        else if(root->unode.nt.value == expression && root->unode.nt.child->unode.nt.value==arithmeticOrBooleanExpr){
            treeNode * child1 = root->unode.nt.child;
            populateAST(child1);
            root->anode = child1->anode;
        }

        //<expression> → <U> (checked)
        else if(root->unode.nt.value == expression && root->unode.nt.child->unode.nt.value==U){
            treeNode * child1 = root->unode.nt.child;
            populateAST(child1);
            root->anode = child1->anode;
        }

        //<U> → <unary_op> <new_NT>  (checked)
        else if(root->unode.nt.value == U){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 = child1->unode.nt.sibling;
            populateAST(child1);
            populateAST(child2);

            root->anode = createNewNode(U,child1->anode);
            child1->anode->sibling = child2->anode;
        }

        //<new_NT> → BO <arithmeticExpr> BC (checked)
        else if(root->unode.nt.value == new_NT && root->unode.nt.child->unode.ln.t.term==BO){
            treeNode * child1 = root->unode.nt.child; //BO
            treeNode * child2 = child1->unode.ln.sibling; //<arithmeticExpr>
            treeNode * child3 = child2->unode.nt.sibling; // BC       

            populateAST(child2);

            root->anode = child2->anode; 
        }
        //<new_NT> → <var_id_num>
        else if(root->unode.nt.value == new_NT && root->unode.nt.child->unode.nt.value==var_id_num){
            treeNode * child1 = root->unode.nt.child;
            populateAST(child1);
            root->anode = child1->anode;
        }
        // <unary_op> → PLUS
        else if(root->unode.nt.value == unary_op && root->unode.nt.child->unode.ln.t.term==PLUS){
            treeNode * child1 = root->unode.nt.child; //PLUS           
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 0;    

        }
        // <unary_op> → MINUS
        else if(root->unode.nt.value == unary_op && root->unode.nt.child->unode.ln.t.term==MINUS){
            treeNode * child1 = root->unode.nt.child; //MINUS           
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 1;    

        }
        //<arithmeticOrBooleanExpr> → <AnyTerm> <N7> (checked)
        else if(root->unode.nt.value == arithmeticOrBooleanExpr){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 =  child1->unode.nt.sibling;
            populateAST(child1);
            child2->inh = child1->anode;
            populateAST(child2);
            root->anode = child2->anode;

        }

        //<N7> → <logicalOp> <AnyTerm> <N7> (checked)
        else if(root->unode.nt.value == N7 && root->unode.nt.child->unode.nt.value==logicalOp){
            treeNode * child1 = root->unode.nt.child; //logicalOp
            treeNode * child2 = child1->unode.nt.sibling; //AnyTerm
            treeNode * child3 = child2->unode.nt.sibling; // <N7>

            populateAST(child1);
            populateAST(child2);

            //<N7>2.inh = new Node("logicalOp", <logicalOp>.addr, <N7>1.inh, <AnyTerm>.addr)

            child3->inh = createNewNode(logicalOp, child1->anode);
            child3->inh->child->sibling = root->inh;
            root->inh->sibling = child2->anode;

            populateAST(child3);
            //<N7>1.syn = <N7>2.syn
            root->anode = child3->anode; 

        }
        //<N7> →e (checked)
        else if(root->unode.nt.value == N7 && root->unode.nt.child->unode.ln.t.term==e){
            root->anode = root->inh;
        }

        //<AnyTerm> → <arithmeticExpr> <N8> (checked)
        else if(root->unode.nt.value == AnyTerm && root->unode.nt.child->unode.nt.value==arithmeticExpr){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 =  child1->unode.nt.sibling;
            populateAST(child1);

            child2->inh = child1->anode;
            populateAST(child2);
            root->anode = child2->anode;
        }
        //<AnyTerm> → <boolConst> (checked)
        else if(root->unode.nt.value == AnyTerm && root->unode.nt.child->unode.nt.value==boolConstt){
            treeNode * child1 = root->unode.nt.child;
            populateAST(child1);
            root->anode = child1->anode;
        }
        //<N8> → <relationalOp> <arithmeticExpr> (checked)
        else if(root->unode.nt.value == N8 && root->unode.nt.child->unode.nt.value==relationalOp){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 =  child1->unode.nt.sibling;
            populateAST(child1);
            populateAST(child2);
            root->anode = createNewNode(relationalOp,child1->anode); //LABEL RELATIONALOP->LEXEME (handled in implementation, by going to child1, know which relOp)
            root->anode->child->sibling = root->inh;
            root->inh->sibling = child2->anode;

        }

        //<N8> →e (checked)
        else if(root->unode.nt.value == N8 && root->unode.nt.child->unode.ln.t.term == e){
            root->anode = root->inh;
        }

        //<arithmeticExpr> → <term> <N4>   (checked)
        else if(root->unode.nt.value == arithmeticExpr && root->unode.nt.child->unode.nt.value==term){
            treeNode * child1 = root->unode.nt.child; //term
            treeNode * child2 =  child1->unode.nt.sibling; //N4
            populateAST(child1);
            child2->inh = child1->anode;
            populateAST(child2);
            root->anode = child2->anode;
        }

        //<N4> → <op1> <term> <N4> (checked)
        else if(root->unode.nt.value == N4 && root->unode.nt.child->unode.nt.value==op1){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 =  child1->unode.nt.sibling;
            treeNode * child3 =  child2->unode.nt.sibling;
            populateAST(child1);
            populateAST(child2);
            child3->inh = createNewNode(op1,child1->anode);

            child3->inh->child->sibling = root->inh;
            root->inh->sibling = child2->anode;

            populateAST(child3);
            root->anode = child3->anode;
        }
        //<N4> → e (checked)
        else if(root->unode.nt.value == N4 && root->unode.nt.child->unode.ln.t.term==e){
            root->anode = root->inh;
        }
        //<term> → <factor> <N5> (checked)
        else if(root->unode.nt.value == term){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 =  child1->unode.nt.sibling;
            populateAST(child1);
            child2->inh = child1->anode;
            populateAST(child2);
            root->anode = child2->anode;

        }

        //<N5> → <op2> <factor> <N5> (checked)
        else if(root->unode.nt.value == N5 && root->unode.nt.child->unode.nt.value==op2){
            treeNode * child1 = root->unode.nt.child;
            treeNode * child2 =  child1->unode.nt.sibling;
            treeNode * child3 =  child2->unode.nt.sibling;
            populateAST(child1);
            populateAST(child2);
            child3->inh = createNewNode(op2,child1->anode);
            child3->inh->child->sibling = root->inh;
            root->inh->sibling = child2->anode;
            populateAST(child3);
            root->anode = child3->anode; 
            
        }
        //<N5> → e (checked)
        else if(root->unode.nt.value == N5 && root->unode.nt.child->unode.ln.t.term==e){
            root->anode = root->inh;
            
        }
        //<factor> → BO <arithmeticOrBooleanExpr> BC (checked)
        else if(root->unode.nt.value == factor && root->unode.nt.child->unode.ln.t.term==BO){
            treeNode * child1 = root->unode.nt.child; //BO
            treeNode * child2 = child1->unode.ln.sibling; //<<arithmeticOrBooleanExpr>
            treeNode * child3 = child2->unode.nt.sibling; // BC       

            populateAST(child2);

            root->anode = createNewNode(factor,child2->anode); 
            root->anode->ruleNum =0;
        }
        //<factor> → <var_id_num> (checked)
        else if(root->unode.nt.value == factor && root->unode.nt.child->unode.nt.value==var_id_num){
            treeNode * child1 = root->unode.nt.child;
            populateAST(child1);
            root->anode = createNewNode(factor,child1->anode);
            root->anode->ruleNum = 1;

        }
        //<op1> → PLUS   (checked)
        else if(root->unode.nt.value == op1 && root->unode.nt.child->unode.ln.t.term==PLUS){
            treeNode * child1 = root->unode.nt.child; //PLUS           
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 0;    

        }

        //<op1> → MINUS   (checked)
        else if(root->unode.nt.value == op1 && root->unode.nt.child->unode.ln.t.term==MINUS){
            treeNode * child1 = root->unode.nt.child; //MINUS          
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 1;    

        }

        //<op2> → MUL (checked)
        else if(root->unode.nt.value == op2 && root->unode.nt.child->unode.ln.t.term==MUL){
            treeNode * child1 = root->unode.nt.child; //MUL      
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 0;  


        }

        //<op2> → DIV  (checked)
        else if(root->unode.nt.value == op2 && root->unode.nt.child->unode.ln.t.term==DIV){
            treeNode * child1 = root->unode.nt.child; //DIV       
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 1;    

        }

        //<logicalOp> → AND (checked)
        else if(root->unode.nt.value == logicalOp && root->unode.nt.child->unode.ln.t.term==AND){
            treeNode * child1 = root->unode.nt.child; //AND         
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 0;
        }
        //<logicalOp> → OR (checked)
        else if(root->unode.nt.value == logicalOp && root->unode.nt.child->unode.ln.t.term==OR){
            treeNode * child1 = root->unode.nt.child; //OR         
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 1;
        }
        //<relationalOp> → LT (checked)
        else if(root->unode.nt.value == relationalOp && root->unode.nt.child->unode.ln.t.term==LT){
            treeNode * child1 = root->unode.nt.child; //LT        
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 0;
        }
        //<relationalOp> → LE (checked)
        else if(root->unode.nt.value == relationalOp && root->unode.nt.child->unode.ln.t.term==LE){
            treeNode * child1 = root->unode.nt.child; //LE   
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 1; 
        }
        //<relationalOp> → GT (checked)
        else if(root->unode.nt.value == relationalOp && root->unode.nt.child->unode.ln.t.term==GT){
            treeNode * child1 = root->unode.nt.child; //GT         
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 2;
        }
        //<relationalOp> → GE (checked)
        else if(root->unode.nt.value == relationalOp && root->unode.nt.child->unode.ln.t.term==GE){
            treeNode * child1 = root->unode.nt.child; //GE         
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 3;
        }
        //<relationalOp> → NE (checked)
        else if(root->unode.nt.value == relationalOp && root->unode.nt.child->unode.ln.t.term==NE){
            treeNode * child1 = root->unode.nt.child; //NE         
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 4;
        }
        //<relationalOp> → EQ (checked)
        else if(root->unode.nt.value == relationalOp && root->unode.nt.child->unode.ln.t.term==EQ){
            treeNode * child1 = root->unode.nt.child; //EQ         
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->ruleNum = 5;
        }

        //<declareStmt> → DECLARE <idList> COLON <dataType> SEMICOL  (checked)
        else if(root->unode.nt.value == declareStmt && root->unode.nt.child->unode.ln.t.term==DECLARE){
            treeNode *child1=root->unode.nt.child; //DECLARE
            treeNode *child2=child1->unode.ln.sibling; // idList
            treeNode *child3=child2->unode.nt.sibling; // COLON
            treeNode *child4=child3->unode.ln.sibling ; // dataType
            treeNode *child5=child4->unode.nt.sibling ; //SEMICOL

            populateAST(child2);
            populateAST(child4);

            root->anode = createNewNode(declareStmt,child2->anode);
            child2->anode->sibling = child4->anode;

        }
        //<conditionalStmt> → SWITCH BO ID BC START <caseStmts> <default> END  (checked)
        else if(root->unode.nt.value == conditionalStmt){
            treeNode * child1 = root->unode.nt.child; //SWITCH
            treeNode * child2 = child1->unode.ln.sibling;//BO
            treeNode * child3 = child2->unode.ln.sibling;//ID
            treeNode * child4 = child3->unode.ln.sibling;//BC
            treeNode * child5 = child4->unode.ln.sibling;//START
            treeNode * child6 = child5->unode.ln.sibling;//<caseStmts>
            treeNode * child7 = child6->unode.nt.sibling;//<default>
            treeNode * child8 = child7->unode.nt.sibling;//END

            astNode *tmp = createNewLeaf(child3->unode.ln.t);
            populateAST(child6);
            populateAST(child7);
            root->anode = createNewNode(conditionalStmt,tmp);
            root->anode->child->sibling = child6->anode;

            child6->anode->sibling = child7->anode;
			
			root->anode->idt = createidST();
			root->anode->idt->start = child5->unode.ln.t.lineno;
			root->anode->idt->end = child8->unode.ln.t.lineno;

        }
        //<caseStmts> → CASE <value> COLON <statements> BREAK SEMICOL <N9> (checked)
        else if(root->unode.nt.value == caseStmts){
            treeNode * child1 = root->unode.nt.child;//CASE
            treeNode * child2 = child1->unode.ln.sibling;//<value>
            treeNode * child3 = child2->unode.nt.sibling;//COLON
            treeNode * child4 = child3->unode.ln.sibling;//<statements>
            treeNode * child5 = child4->unode.nt.sibling;//BREAK
            treeNode * child6 = child5->unode.ln.sibling;//SEMICOL
            treeNode * child7 = child6->unode.ln.sibling;//<N9>

            populateAST(child2);
            populateAST(child4);
            populateAST(child7);
            astNode *tmp = createNewNode(caseSingle,child2->anode);
            child2->anode->sibling = child4->anode;///
            root->anode = createNewNode(caseStmts,tmp);

            tmp->sibling=child7->anode;

        }
        //<N9> → CASE <value> COLON <statements> BREAK SEMICOL <N9> (checked)
        else if(root->unode.nt.value == N9 &&  root->unode.nt.child->unode.ln.t.term== CASE){
            treeNode * child1 = root->unode.nt.child;//CASE
            treeNode * child2 = child1->unode.ln.sibling;//<value>
            treeNode * child3 = child2->unode.nt.sibling;//COLON
            treeNode * child4 = child3->unode.ln.sibling;//<statements>
            treeNode * child5 = child4->unode.nt.sibling;//BREAK
            treeNode * child6 = child5->unode.ln.sibling;//SEMICOL
            treeNode * child7 = child6->unode.ln.sibling;//<N9>

            populateAST(child2);
            populateAST(child4);
            populateAST(child7);

            astNode *tmp = createNewNode(caseSingle,child2->anode);
            child2->anode->sibling = child4->anode;///
            root->anode = createNewNode(N9,tmp);

            tmp->sibling=child7->anode;
        }

        //<N9> → e (checked)
        else if(root->unode.nt.value == N9 && root->unode.nt.child->unode.ln.t.term==e){
            root->anode= NULL;
        }

        //<value> → NUM (checked)
        else if(root->unode.nt.value == value && root->unode.nt.child->unode.ln.t.term==NUM){
            treeNode * child1 = root->unode.nt.child; //NUM        
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = INTEGER;
            root->anode->ruleNum = 0;
        }
        //<value> → TRUE (checked)
        else if(root->unode.nt.value == value && root->unode.nt.child->unode.ln.t.term==TRUE){
            treeNode * child1 = root->unode.nt.child; //TRUE        
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = BOOLEAN;
            root->anode->ruleNum = 1;
        }
        //<value> → FALSE  (checked)
        else if(root->unode.nt.value == value && root->unode.nt.child->unode.ln.t.term==FALSE){
            treeNode * child1 = root->unode.nt.child; //FALSE  
            root->anode = createNewLeaf(child1->unode.ln.t);
            root->anode->unode.ln.dataType = BOOLEAN;
            root->anode->ruleNum = 2;
        }
        //<default> → DEFAULT COLON <statements> BREAK SEMICOL  (checked)
        else if(root->unode.nt.value == default_ && root->unode.nt.child->unode.ln.t.term==DEFAULT){
            
            treeNode * child1 = root->unode.nt.child;//DEFAULT
            treeNode * child2 = child1->unode.ln.sibling;//COLON
            treeNode * child3 = child2->unode.ln.sibling;//<statements>

            populateAST(child3);
            child1->anode = createNewLeaf(child1->unode.ln.t);
            root->anode = createNewNode(default_,child1->anode);
            child1->anode->sibling = child3->anode;
        }
        //<default> → e (checked)
        else if(root->unode.nt.value == default_ && root->unode.nt.child->unode.ln.t.term==e){
            root->anode= NULL;
        }
        //<iterativeStmt> → FOR BO ID IN <range> BC START <statements> END(checked)
        else if(root->unode.nt.value == iterativeStmt && root->unode.nt.child->unode.ln.t.term==FOR){
            treeNode * child1 = root->unode.nt.child; //FOR
            treeNode * child2 = child1->unode.ln.sibling;//BO
            treeNode * child3 = child2->unode.ln.sibling;//ID
            treeNode * child4 = child3->unode.ln.sibling;//IN
            treeNode * child5 = child4->unode.ln.sibling;//<range>
            treeNode * child6 = child5->unode.nt.sibling;//BC
            treeNode * child7 = child6->unode.ln.sibling;//START
            treeNode * child8 = child7->unode.ln.sibling;//<statements>
			treeNode * child9 = child8->unode.nt.sibling;//END
            astNode *tmp=createNewLeaf(child3->unode.ln.t);
            populateAST(child5);
            populateAST(child8);
            root->anode = createNewNode(iterativeStmt,tmp);
            root->anode->ruleNum =0;
            root->anode->child->sibling = child5->anode;

            child5->anode->sibling = child8->anode;
			
			root->anode->idt = createidST();
			root->anode->idt->start = child7->unode.ln.t.lineno;
			root->anode->idt->end = child9->unode.ln.t.lineno;

        }

        //<iterativeStmt> → WHILE BO <arithmeticOrBooleanExpr> BC START <statements> END  (checked)
        else if(root->unode.nt.value == iterativeStmt && root->unode.nt.child->unode.ln.t.term==WHILE){
            treeNode * child1 = root->unode.nt.child; //WHILE
            treeNode * child2 = child1->unode.ln.sibling;//BO
            treeNode * child3 = child2->unode.ln.sibling;//<arithmeticOrBooleanExpr
            treeNode * child4 = child3->unode.nt.sibling;//BC
            treeNode * child5 = child4->unode.ln.sibling;//START
            treeNode * child6 = child5->unode.ln.sibling;//<statements>
			treeNode * child7 = child6->unode.nt.sibling;//END
            populateAST(child3);
            populateAST(child6);
            root->anode = createNewNode(iterativeStmt, child3->anode);
            root->anode->ruleNum =1;

            child3->anode->sibling = child6->anode;
			
			root->anode->idt = createidST();
			root->anode->idt->start = child5->unode.ln.t.lineno;
			root->anode->idt->end = child7->unode.ln.t.lineno;

        }
        //<range> → NUM RANGEOP NUM (checked)
        else if(root->unode.nt.value == range){
            treeNode * child1 = root->unode.nt.child; //NUM
            treeNode * child2 = child1->unode.ln.sibling;//RANGEOP
            treeNode * child3 = child2->unode.ln.sibling;//NUM
            astNode *tmp1  = createNewLeaf(child1->unode.ln.t);
            tmp1->unode.ln.dataType = INTEGER;
            astNode *tmp2  = createNewLeaf(child3->unode.ln.t);
            tmp2->unode.ln.dataType = INTEGER;
            root->anode = createNewNode(range,tmp1);
            root->anode->child->sibling = tmp2;
        }
    }

}
int count ;

void parentAST(astNode *head)
{
    if(head==NULL)
        return;
    if(head->child==NULL)
        return;
    astNode *tmp=head->child;
    while(tmp!=NULL)
    {
        tmp->parent=head;
        parentAST(tmp);
        tmp=tmp->sibling;
    }
}

void printAST(astNode *head)
{
    if(head==NULL)
        return;

    else if(head->child==NULL)
    {   count ++ ;
        if(head->isLeaf==0){
        printf("child: %-20s parent:",enumToString(head->unode.nt.symbol));
        printf("%-25s \n",enumToString(head->parent->unode.nt.symbol));
        }
        else {
        printf("child: %-20s parent: ",enumToString(head->unode.ln.t.term));

        printf("%-25s ",enumToString(head->parent->unode.nt.symbol));

        if(head->unode.ln.t.term == ID){
            if(head->idt != NULL){
                idSTEntry * t ;
                if(( t =lookupidST(head->idt,head->unode.ln.t.lexeme))!= NULL)
                    ;
            }
            else{
                ;
            }

        }
        printf("\n");
        }

    }
    else
    {
        printAST(head->child);
        count ++;
        printf("child: %-20s parent: ",enumToString(head->unode.nt.symbol));

        if(head->parent != NULL)
        printf("%-25s\n",enumToString(head->parent->unode.nt.symbol));
        else printf("\n");
        astNode *temp= head->child;
        while(temp->sibling!=NULL)
        {
            printAST(temp->sibling);
            temp=temp->sibling;
        }
    }
}

astNode * generateAST(char *out, int printFlag) {
    count = 0;
    numAstNodes = 0;
    astMemUsage = 0;
    treeNode *parRoot = returnRoot(out);
    populateAST(parRoot);
    parentAST(parRoot->anode);

    if(printFlag == 1)
        printAST(parRoot->anode);
    return parRoot->anode;
}


astNode * memUsage(char *out){
    count = 0;
    numAstNodes = 0;
    astMemUsage = 0;
    treeNode *parRoot = returnRoot(out);
    populateAST(parRoot);
    parentAST(parRoot->anode);
    printf("No. of nodes in the AST: %-17d Allocated Memory: ",numAstNodes);
    printf("%d bytes\n",astMemUsage);
    return parRoot->anode;
}