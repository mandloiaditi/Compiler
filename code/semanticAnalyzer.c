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
#include "semanticAnalyzer.h"

//SEMANTIC RULES
/*
1.DONE : An identifier cannot be declared multiple times in the same scope. 
2.DONE : An identifier must be declared before its use.
3.DONE :The types and the number of parameters returned by a function must be the same as that of the parameters used in invoking the function.
4.DONE : The parameters being returned by a function must be assigned a value. If a parameter does not get a value assigned within the function definition, it should be reported as an error. 
5.DONE : The function that does not return any value, must be invoked appropriately.
6.DONE : Function input parameters passed while invoking it should be of the same type as those used in the function definition.
7.DONE : A switch statement with an integer typed identifier associated with it, can have case statement with case keyword followed by an integer only and the case statements must be followed by a default statement.
8.DONE : A switch statement with an identifier of type real is not valid and an error should be reported.
9.DONE : A switch statement with a boolean type identifier can have the case statements with labels true and false only. The switch statement then should not have a default statement.
10.DONE : Function overloading is not allowed.
11.DONE : A function declaration for a function being used (say F1) by another (say F2) must precede the definition of the function using it(i.e. F2), only if the function definition of F1 does not precede the definition of F2.
12.DONE : If the function definition of F1 precedes function definition of F2(the one which uses F1), then the function declaration of F1 is redundant and is not valid.
13.DONE : A for statement must not redefine the variable that participates in the iterating over the range.
14.DONE : The function cannot be invoked recursively.
15.DONE : An identifier used beyond its scope must be viewed as undefined
/*

/*
TYPE CHECKING:
The Static type checking rules are:
1. DONE : The type of an identifier is the type appearing while declaring the variable.
2. DONE : The type of NUM is integer.
3. DONE : The type of RNUM is real.
4. DONE : The type of true or false value is boolean.
5. DONE : The type of an array variable A of type array [12..20] of real (say) is defined as an expression <real, 12, 20>.  The type of an array element A[13] (say) is real if 13 is within the bounds [12, 20]
6. The type of a simple expression (say E) of the form expression(say E1) <operator>

Expression(say E2)

DONE :is integer, if both expressions are of type integer and the operator is arithmetic operator.
DONE :is real, if both the expressions are of type real and the operator is arithmetic operator.
DONE :is boolean, if both expressions are of type integer and the operator is a relational operator.
DONE :is boolean, if both expressions are of type real and the operator is relational.
DONE :is boolean, if both expressions are of type boolean and the operator is logical.
DONE :The type of the expression is ERROR, if the above rules do not derive the type of E appropriately.


Type checking rules for array construct are as follows:
1.DONE : The operations +, -, *, / and all relational and logic operators cannot be applied on array variables of type <type, 12, 20> (say)
2.DONE :The assignment operator applied to two array variables of the same type is valid. For example, if A and B are the array variables of type array[12..20] of real, then A:= B; is a valid statement. This applies to dynamic arrays of type array[a..b] of real as well.
3.DONE :Consider array elements with index represented by integer identifier say A[k]. Here type checking of variable k is done at compile time. If the type of k is integer, then it is valid else it is reported as an error. Also, the type checking of A[13], for type of index (NUM), is done at compile time.
4.DONE :The bound checking of A[13] where A is a static array is done at compile time. If A is a dynamic array, then bound checking of A[13] is done at run time [see below]
5.DONE :The type of an identifier or an expression is computed by traversing the abstract syntax tree.

// ADDITIONAL CHECKS:
1. Expression in while condition should be boolean
2. In A[k], A should be array variable
3. Ensured start index <= end index (for static array declaration)
*/

isError errArr[5000];
int errorIndex = 0;


void semanticAnalyzer(astNode * root, moduleST * mt, int errorFlag){
	int c = 1;
    if(root==NULL) return;
    if(root->isLeaf == 0 && root->unode.nt.symbol == moduleReuseStmt){
    	checkModuleReuse(root, mt, errorFlag);
    }

    else if(root->isLeaf == 0 && root->unode.nt.symbol == conditionalStmt){
    	c = checkConditional(root, mt, errorFlag);
		if(c==0){
			root = root->sibling;
		}
    }

    else if(root->isLeaf == 0 && root->unode.nt.symbol == iterativeStmt){
    	checkIterative(root, mt, errorFlag);
    }
    
	else if(root->isLeaf == 0 && root->unode.nt.symbol == assignmentStmt){
    	checkAssign(root, mt, errorFlag);
    }
    if(root->child){
    semanticAnalyzer(root->child,mt,errorFlag);
    astNode* temp=root->child;
        
    if(temp!=NULL)
        temp=temp->sibling;
    while(temp != NULL){
        semanticAnalyzer(temp,mt, errorFlag);
        temp=temp->sibling;
	}
  }
}

void checkAssign(astNode * node, moduleST * mt, int errorFlag)
{
	if(node==NULL)
		return;
	astNode *tmp = node->child;//ID;
	astNode *whichST = node->child->sibling;//whichStmt
	Term lhs ;
	idSTEntry * search = lookupidST(tmp->idt,tmp->unode.ln.t.lexeme);
	if(search != NULL){
		lhs = search->dataType;
	}
	else {
		lhs = ERROR;
		return ;
	}

	//finding out dataType of RHS
	if(whichST->unode.nt.symbol == lvalueARRStmt)
	{
		astNode *temp=whichST;
		Term t,tp;
		if(temp->unode.nt.symbol == lvalueARRStmt){
            //checking for <index>
            if(temp->child->unode.ln.t.term == ID){
                idSTEntry * insearch = lookupidST(temp->child->idt,temp->child->unode.ln.t.lexeme);
				if(search != NULL){
					t = insearch->dataType;
				}
				else
					t = ERROR;
				if(search != NULL && search->isArray!=1){
					if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",tmp->unode.ln.t.lineno);
					printf("array indexing on non array type\n");}
					hasSemanticError = 1;
				}
			
				else if(search != NULL && search->isArray ==1 && search->isStatic==1 && insearch->dataType != INTEGER){
					if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",temp->child->unode.ln.t.lineno);
					printf("array indexed using a non integer variable %s\n",insearch->lexeme);}
					hasSemanticError = 1;
				}				
				else if(search != NULL && search->isArray ==1)
				{
					// dynamic array
					if(lhs == INTEGER)
						lhs = DINT;
					if(lhs == REAL)
						lhs = DREAL;
					if(lhs == BOOLEAN)
						lhs = DBOOL;
				}

            }

            else{
            	if(search != NULL && search->isArray ==1 && search->isStatic==1 && temp->child->unode.ln.dataType == INTEGER && temp->child->unode.ln.t.term == NUM){
					int arrayInd = temp->child->unode.ln.t.value.v2;
					if(search->start.num > arrayInd || search->end.num < arrayInd){
					if(errorFlag==1){	
						printf("LINE NO. : %d :ERROR: ",temp->child->unode.ln.t.lineno);
						printf("element of array %s is out of bound\n",search->lexeme);}
						hasSemanticError = 1;
					}
				}
				else
				{
					if(search != NULL && search->isArray ==1 && search->isStatic==0)
					{
						if(lhs == INTEGER)
						lhs = DINT;
						if(lhs == REAL)
							lhs = DREAL;
						if(lhs == BOOLEAN)
							lhs = DBOOL;
					}
				}
				
            	if(search != NULL && search->isArray!=1){
					if(errorFlag==1){	
						printf("LINE NO. : %d :ERROR: ",tmp->unode.ln.t.lineno);
						printf("array indexing on non array type\n");}
						hasSemanticError = 1;
				}
            }

            
            temp = temp->child->sibling ; //<expression>
            if(temp->unode.nt.symbol == U)
			{
				//temp is Unary op node
				tp = U_type(temp,tmp->unode.ln.t.lineno,errorFlag);
			}        
			else 
			{	//temp is expression
				tp = ariBoolTrav(temp,tmp->unode.ln.t.lineno,errorFlag);
			}
			

			if(lhs == INTEGER && tp == DINT)
        	tp = INTEGER;
	        if(lhs == DINT && tp == INTEGER)
	        	lhs = INTEGER;
	        if(lhs == DINT && tp == DINT)
	        {
	        	lhs = INTEGER;
	        	tp = INTEGER;
	        }

	        
	        if(lhs == REAL && tp == DREAL)
	        	tp = REAL;
	        if(lhs == DREAL && tp == REAL)
	        	lhs = REAL;
	        if(lhs == DREAL && tp == DREAL)
	        {
	        	lhs = REAL;
	        	tp = REAL;
	        }

	        if(lhs == BOOLEAN && tp == DBOOL)
	        	tp = BOOLEAN;
	        if(lhs == DBOOL && tp == BOOLEAN)
	        	lhs = BOOLEAN;
	        if(lhs == DBOOL && tp == DBOOL)
	        {
	        	lhs = BOOLEAN;
	        	tp = BOOLEAN;
	        }

	        int flah=0;
	        if(tp == DINT || tp == DREAL || tp == DBOOL)
	        {
	        	errArr[errorIndex].line = tmp->unode.ln.t.lineno;
	        	errArr[errorIndex].type=0;
	        	errorIndex++;
	        	flah=1;
	        }
	        else if(lhs == DINT || lhs == DREAL || lhs == DBOOL)
	        {
	        	errArr[errorIndex].line = tmp->unode.ln.t.lineno;
	        	errArr[errorIndex].type=0;
	        	errorIndex++;
	        	flah=1;
	        }



           if(flah == 0 && lhs!=tp && tp!=ERROR && lhs!=ERROR){
				if(errorFlag==1){	
				printf("LINE NO. : %d :ERROR: ",tmp->unode.ln.t.lineno);
				printf("type mismatch error:lhs type = %s rhs type = %s\n",enumToString(lhs),enumToString(tp));}
				hasSemanticError = 1;
			
			}
    	}
	}
	else
	{
		
		if(search->isArray ==1){
			if(whichST->unode.nt.symbol == factor){
				if(whichST->child->unode.nt.symbol == var_id_num ){
					if(whichST->child->child->sibling == NULL){
						idSTEntry * s = lookupidST(whichST->child->child->idt,whichST->child->child->unode.ln.t.lexeme);
						if(s != NULL && s->isArray !=1){
						if(errorFlag==1){	
							printf("LINE NO. : %d :ERROR: ",whichST->child->child->unode.ln.t.lineno);
							printf("type mismatch error: array variable assigned %s variable\n",enumToString(s->dataType));}
							hasSemanticError = 1;

						}
						else{
							if(s==NULL)
							{	//identifier in expression is undeclared
								;//printf("SEMANTIC UNDECLARED ERROR LINE : %d SYMBOL : %s\n",whichST->child->child->unode.ln.t.lineno,whichST->child->child->unode.ln.t.lexeme);
							}
							else if(search->dataType == s->dataType){
								if(search->isStatic == s->isStatic  && s->isStatic ==1 ){
									if(s->start.num != search->start.num || s->end.num != search->end.num){
									if(errorFlag==1){	
										printf("LINE NO. : %d :ERROR: ",whichST->child->child->unode.ln.t.lineno);
										printf("type mismatch error: bound mismatch between arrays\n");}
										hasSemanticError = 1;
									}
								}
							}
							else{

								if(s->isStatic==0||search->isStatic==0)
								{
									errArr[errorIndex].line = whichST->child->child->unode.ln.t.lineno;
									errArr[errorIndex].type = 0;
									errorIndex++;
								}
								else
								{
									if(errorFlag==1){	
										printf("LINE NO. : %d :ERROR: ",whichST->child->child->unode.ln.t.lineno);
										printf("type mismatch error: array variables of different type\n");
									}
									hasSemanticError = 1;
								}
							}
						}
						return;
					}
					else{
						if(errorFlag==1){	
							printf("LINE NO. : %d :ERROR: ",tmp->unode.ln.t.lineno);
							printf("type mismatch error: array variable assigned non-array variable\n");}
						hasSemanticError = 1;
					}
				}
			}
		}
		
		Term t;
		if(whichST->unode.nt.symbol == U)
        {
        	//temp is Unary op node
            t = U_type(whichST,tmp->unode.ln.t.lineno,errorFlag);
        }        
        else 
        { 	//temp is expression
            t = ariBoolTrav(whichST,tmp->unode.ln.t.lineno,errorFlag);
                
        }

        if(lhs == INTEGER && t == DINT)
        	t = INTEGER;
             
        if(lhs == REAL && t == DREAL)
        	t = REAL;
        

        if(lhs == BOOLEAN && t == DBOOL)
        	t = BOOLEAN;
        

        int flah=0;
        if(t == DINT || t == DREAL || t == DBOOL)
        {
        	errArr[errorIndex].line = tmp->unode.ln.t.lineno;
        	errArr[errorIndex].type=0;
        	errorIndex++;
        	flah=1;
        }
        
		if(flah == 0 && lhs!=t && t!=ERROR && lhs!=ERROR)
		{		
				if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",tmp->unode.ln.t.lineno);
					printf("type mismatch error:lhs type = %s rhs type = %s\n",enumToString(lhs),enumToString(t));}
				hasSemanticError = 1;
			
		}
			
	}
}

Term U_type(astNode* whichST, int line,int errorFlag)
{
	
	astNode *newNT = whichST->child->sibling;//newNt
	
	if(newNT->unode.nt.symbol == var_id_num){//ID whichID		
		astNode *temp = newNT->child;//ID
		idSTEntry * search = lookupidST(temp->idt,temp->unode.ln.t.lexeme);
		if(search != NULL){
			return search->dataType;
		}
		else
		{	//variable in expression undeclared
			//printf("SEMANTIC UNDECLARED ERROR LINE : %d    %d\n",line,temp->unode.ln.t.lineno);
			return ERROR;
		}
	}
	else if(newNT->isLeaf==1)
	{
		return newNT->unode.ln.dataType;
	}	
	else{
		return trmTrav(newNT,line,errorFlag);
	}
}

Term anyTerm1(astNode *temp,int line, int errorFlag)
{
    
    if(temp==NULL)
    {
    	return ERROR;
    }
    if(temp->isLeaf==1)
    {
        Term a;
        a=temp->unode.ln.dataType;
        return a;
    }    
    else {
       Term a;
       a=ariExp1(temp,line,errorFlag);
       return a;
    }
    
}

Term ariExp1(astNode *temp,int line, int errorFlag)
{
    if(temp->unode.nt.symbol==relationalOp)
    {
        //N7 NULL and N8 not NULL here
        Term dt1,dt2;
		dt1=trmTrav(temp->child->sibling,line,errorFlag);
        dt2=trmTrav(temp->child->sibling->sibling,line,errorFlag);
		if(dt1==BOOLEAN || dt2==BOOLEAN)
		{
			if(errorFlag==1){	
				printf("LINE NO. : %d :ERROR: ",line);
				printf("type mismatch error: arithmetic operator applied on boolean variable\n");}
				hasSemanticError = 1;
				return ERROR;
		}
		
		if(dt1==UNASSIGNED)
			return BOOLEAN;
		if(dt2==UNASSIGNED)
			return BOOLEAN;

		if(dt1==ERROR||dt2==ERROR)
			return ERROR;

		if(dt1==DINT && dt2==INTEGER)
			return BOOLEAN;
		if(dt1==INTEGER && dt2==DINT)
			return BOOLEAN;
		if(dt1==DINT && dt2==DINT)
		{
			return BOOLEAN;
		}
		if(dt1==DINT)
		{
			errArr[errorIndex].line = line;
			errArr[errorIndex].type = 0;
			errorIndex++;
			return BOOLEAN;
		}
		if(dt2==DINT)
		{
			errArr[errorIndex].line = line;
			errArr[errorIndex].type = 0;
			errorIndex++;
			return BOOLEAN;
		}


		if(dt1==DREAL && dt2==REAL)
			return BOOLEAN;
		if(dt1==REAL && dt2==DREAL)
			return BOOLEAN;
		if(dt1==DREAL && dt2==DREAL)
		{
			return BOOLEAN;
		}
		if(dt1==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type = 0;
			errorIndex++;
			return BOOLEAN;
		}
		if(dt2==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type = 0;
			errorIndex++;
			return BOOLEAN;
		}


		if(dt1==DBOOL && dt2==BOOLEAN)
			return ERROR;
		if(dt1==BOOLEAN && dt2==DBOOL)
			return ERROR;
		if(dt1==DBOOL && dt2==DBOOL)
		{
			errArr[errorIndex].line = line;
			errArr[errorIndex].type = 1;
			errorIndex++;
			return DBOOL;
		}
		if(dt1==DBOOL)
		{
			errArr[errorIndex].line = line;
			errArr[errorIndex].type = 1;
			errorIndex++;
			return dt2;
		}
		if(dt2==DBOOL)
		{
			errArr[errorIndex].line = line;
			errArr[errorIndex].type = 1;
			errorIndex++;
			return dt1;
		}

		if(dt1==dt2)
			return BOOLEAN;
		else{			
			if(errorFlag==1){	
				printf("LINE NO. : %d :ERROR: ",line);
				printf("type mismatch error\n");
			}
			hasSemanticError = 1;
			return ERROR;
		}
    }
    else {
        Term a=trmTrav(temp,line, errorFlag);
        return a;
    }
   
}

Term trmTrav(astNode *node, int line,int errorFlag)
{
    if(node->unode.nt.symbol==op1) //op1 - PLUS/MIUS node
    {
        
		Term dt1,dt2;
		dt1=trmTrav(node->child->sibling, line,errorFlag);
		dt2=factTrav(node->child->sibling->sibling, line,errorFlag);
		
		
		astNode *temp=node;

		if(dt1==BOOLEAN||dt2==BOOLEAN)
		{
			if(errorFlag==1){	
				printf("LINE NO. : %d :ERROR: ",line);
				printf("type mismatch error: arithmetic operator applied on boolean variable\n");}
			hasSemanticError = 1;
			return ERROR;
		}
		if(dt1==UNASSIGNED)
			return dt2;
		if(dt2==UNASSIGNED)
			return dt1;

		if(dt1==ERROR||dt2==ERROR)
			return ERROR;

		if(dt1==DINT && dt2==INTEGER)
			return INTEGER;
		if(dt1==INTEGER && dt2==DINT)
			return INTEGER;
		if(dt1==DINT && dt2==DINT)
		{
			return DINT;
		}
		if(dt1==DINT)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt2;
		}
		if(dt2==DINT)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt1;
		}


		if(dt1==DREAL && dt2==REAL)
			return REAL;
		if(dt1==REAL && dt2==DREAL)
			return REAL;
		if(dt1==DREAL && dt2==DREAL)
		{
			return DREAL;
		}
		if(dt1==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt2;
		}
		if(dt2==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt1;
		}


		if(dt1==DBOOL && dt2==BOOLEAN)
			return BOOLEAN;
		if(dt1==BOOLEAN && dt2==DBOOL)
			return BOOLEAN;
		if(dt1==DBOOL && dt2==DBOOL)
		{
			return DBOOL;
		}
		if(dt1==DBOOL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt2;
		}
		if(dt2==DBOOL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt1;
		}
		if(dt1!=dt2)
		{
			if(errorFlag==1){	
				printf("LINE NO. : %d :ERROR: ",line);
				printf("type mismatch error\n");}
			hasSemanticError = 1;
			return ERROR;
		}
		
		
    }    
    else 
    {
            
       Term t ;
       t= factTrav(node,line,errorFlag);
        
        return t;
    }
    
}

Term fact1(astNode *node,int line,int errorFlag)
{
	if(node==NULL)
	{
		return ERROR;
	}
	if(node != NULL){
		if(node->child->unode.nt.symbol==var_id_num)
		{

			astNode * temp = node->child->child;
			
			idSTEntry * search = lookupidST(temp->idt,temp->unode.ln.t.lexeme);
			
			if(search != NULL){
				if(temp->sibling != NULL && search->isArray == 0 ){
					if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
					printf("array indexing on non array element\n");}
					hasSemanticError = 1;
					return ERROR;

				}
				else if(search->isArray == 1 && temp->sibling == NULL  ){
					if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
					printf("type mismatch error: array variable %s used in expression\n", search->lexeme);}
					hasSemanticError = 1;
						
					return ERROR;
				}
				if(search->isArray ==1 && search->isStatic ==1){
					return checkArray(node->child,errorFlag);
					
				}
				if(search->isArray == 1 && search->isStatic ==0)
				{

					if(search->dataType == INTEGER)
					{
						return DINT;
					}
					if(search->dataType == REAL)
					{
						return DREAL;
					}
					if(search->dataType == BOOLEAN)
					{
						return DBOOL;
					}

				}
				return search->dataType;
				}
			else
				{
					//variable used in expression undeclared
					//printf("SEMANTIC UNDECLARED ERROR LINE : %d    %s\n",line,temp->unode.ln.t.lexeme);
					return ERROR;
			}

		}
		if(node->child->isLeaf==1)
		{
			return node->child->unode.ln.dataType;
		}
		else
		{
			return ariBoolTrav(node->child,line,errorFlag);
		}
	}
}

Term factTrav(astNode *node,int line,int errorFlag)
{
	astNode * temp = node;
	if(temp == NULL){
		return ERROR;
	}

    if(temp->unode.nt.symbol==op2)
    {
    
        //N7 NULL and N8 NULL and N4 NULL and N5 not NULL here
        
		Term dt1,dt2;
		
		dt1=factTrav(node->child->sibling,line,errorFlag);
		dt2 = fact1(node->child->sibling->sibling,line,errorFlag);
		if(dt1==BOOLEAN||dt2==BOOLEAN)
		{
			if(errorFlag==1){	
				printf("LINE NO. : %d :ERROR: ",line);
				printf("type mismatch error: BOOLEAN IN RELATIONAL EXPR\n");}
			hasSemanticError = 1;
			return ERROR;
		}
		if(dt1==UNASSIGNED)
			return dt2;
		if(dt2==UNASSIGNED)
			return dt1;

		if(dt1==ERROR||dt2==ERROR)
			return ERROR;

		if(dt1==DINT && dt2==INTEGER)
			return INTEGER;
		if(dt1==INTEGER && dt2==DINT)
			return INTEGER;
		if(dt1==DINT && dt2==DINT)
		{
			return DINT;
		}
		if(dt1==DINT)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt2;
		}
		if(dt2==DINT)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt1;
		}


		if(dt1==DREAL && dt2==REAL)
			return REAL;
		if(dt1==REAL && dt2==DREAL)
			return REAL;
		if(dt1==DREAL && dt2==DREAL)
		{
			return DREAL;
		}
		if(dt1==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt2;
		}
		if(dt2==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt1;
		}


		if(dt1==DBOOL && dt2==BOOLEAN)
			return BOOLEAN;
		if(dt1==BOOLEAN && dt2==DBOOL)
			return BOOLEAN;
		if(dt1==DBOOL && dt2==DBOOL)
		{
			return DBOOL;
		}
		if(dt1==DBOOL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt2;
		}
		if(dt2==DBOOL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt1;
		}


		if(dt1!=dt2)
		{
			if(errorFlag==1){	
				printf("LINE NO. : %d :ERROR: ",line);
				printf("type mismatch error\n");}
			hasSemanticError = 1;
			return ERROR;
		}		
        return dt2;
        
       
        
    }

	else return fact1(temp,line,errorFlag);
    
}


Term ariBoolTrav(astNode *temp,int line, int errorFlag)
{
 
    if(temp==NULL)
        return -1;
    if(temp->isLeaf==1)
    {
        return temp->unode.ln.dataType;
        //N5 not NULL wont be a leaf - same case for N4 and N8 and N7
    }
    else{    
       
        if(temp->unode.nt.symbol == logicalOp)
        {
            
			Term dt1,dt2;
			
			dt1 = ariBoolTrav(temp->child->sibling,line,errorFlag);
			dt2 = anyTerm1(temp->child->sibling->sibling,line,errorFlag);
			
			if(dt1==ERROR||dt2==ERROR)
			{
				return ERROR;
			}
			else if(dt1!=BOOLEAN||dt2!=BOOLEAN)
			{
				if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",line);
					printf("type mismatch error: %s type used in logical expression\n", enumToString( dt2));}
				hasSemanticError = 1;
				return ERROR;
			}


			if(dt1==UNASSIGNED)
			return BOOLEAN;
			if(dt2==UNASSIGNED)
			return BOOLEAN;

		
		if(dt1==DINT && dt2==DINT)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=2;
			errorIndex++;
			return DINT;
		}
		if(dt1==DINT)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=2;
			errorIndex++;
			return dt2;
		}
		if(dt2==DINT)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=2;
			errorIndex++;
			return dt1;
		}


		
		if(dt1==DREAL && dt2==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=2;
			errorIndex++;
			return DREAL;
		}
		if(dt1==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=2;
			errorIndex++;
			return dt2;
		}
		if(dt2==DREAL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=2;
			errorIndex++;
			return dt1;
		}


		if(dt1==DBOOL && dt2==BOOLEAN)
			return BOOLEAN;
		if(dt1==BOOLEAN && dt2==DBOOL)
			return BOOLEAN;
		if(dt1==DBOOL && dt2==DBOOL)
		{
			return BOOLEAN;
		}
		if(dt1==DBOOL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt2;
		}
		if(dt2==DBOOL)
		{
			errArr[errorIndex].line=line;
			errArr[errorIndex].type=0;
			errorIndex++;
			return dt1;
		}
			
			
			return BOOLEAN;
            
        }
        else {
        
        	Term a=anyTerm1(temp,line,errorFlag);
            return a;
        }
    
    }
   
}


void checkModuleReuse(astNode * node, moduleST * mt, int errorFlag){
	if(node->child->isLeaf == 0 && node->child->unode.nt.symbol == idList){
		//optional part of module Reuse Stmt
		moduleSTEntry  * en = lookupmoduleST(mt,node->child->sibling->unode.ln.t.lexeme); // module ID
		
		if(en == NULL || en->moduleDefined==0){
			if(errorFlag==1){
				printf("LINE NO. : %d :ERROR: ",node->child->sibling->unode.ln.t.lineno);
				printf("module %s not defined\n", node->child->sibling->unode.ln.t.lexeme);
			}
			hasSemanticError = 1;
		}
		
		else
			
			{
			pNode * out = en->outParams;
			if(out == NULL){
				if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",node->child->sibling->unode.ln.t.lineno);
					printf("void function invoked inappropriately\n");
				}
				hasSemanticError = 1;
			}

			pNode * in = en->inParams;
							 
		    //moduleRuse -> optional->idList->ID->N3->ID	
			astNode *temp = node->child->sibling->sibling; // input variables
			temp = temp->child;
			
			while(temp != NULL){
				idST * idt =  temp->idt;
				if(idt != NULL){
					idSTEntry * id  =  lookupidST(idt,temp->unode.ln.t.lexeme);
					idSTEntry * inparam  =  lookupidST(en->idt,in->lexeme);
					
					if(in!= NULL && id->dataType != in->dataType){
						if(errorFlag==1){	
							printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
							printf("Datatype mismatch in function input parameters\n");
							printf("%-23sdatatype of %s:%s, dataype expected:%s\n","",id->lexeme, enumToString(id->dataType), enumToString(in->dataType) );
						}
						hasSemanticError = 1;
					}

					else if(inparam->isArray == 1){
						if(id->isArray == 0){
							if(errorFlag==1){	
								printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
								printf("Datatype mismatch in function input parameters\n");
							}
							hasSemanticError = 1;
						}

						else if(id->isArray==1 && id->isStatic == 1 && inparam->isStatic == 1){

							//actual and formal parameter both are static arrays
							if((id->start.num != inparam->start.num) || (id->end.num != inparam->end.num)){
								if(errorFlag==1){	
								printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
								printf("Datatype mismatch in function input parameters\n");
								}
								hasSemanticError = 1;
							   }
							}

						else if(id->isArray==1){

							//start index is NUM
							if(id->start_pop == 1 && inparam->start_pop == 1 && (id->start.num != inparam->start.num)){
								if(errorFlag==1){	
								printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
								printf("Datatype mismatch in function input parameters\n");
								}
								hasSemanticError = 1;
							}
							
							//end index is NUM
							else if(id->end_pop == 1 && inparam->end_pop == 1 && (id->end.num != inparam->end.num)){
								if(errorFlag==1){	
								printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
								printf("Datatype mismatch in function input parameters\n");
								}
								hasSemanticError = 1;
							}
						}
					}
				}
					
				if(in == NULL){
					if(errorFlag==1){	
						printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
						printf("No. of module %s input parameters less than actual parameters\n",en->lexeme_mod);}
					hasSemanticError = 1;
					}
				
				if(in != NULL)
					in = in->next;
				
				if(temp->sibling){
					temp = temp->sibling->child;
					}
					
				else{
					//sibling is NULL
					temp=temp->sibling;
					}

			}
			
			if(in != NULL){
				if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",node->child->sibling->unode.ln.t.lineno);
					printf("No. of module %s input parameters greater than actual parameters\n",en->lexeme_mod);}
				hasSemanticError = 1;
				}
		
			
			temp =  node->child->child; //ID
		
			astNode * nextid;
			while(temp != NULL){
				idST * idt =  temp->idt;
				
				if(idt != NULL){
					idSTEntry * id  =  lookupidST(idt,temp->unode.ln.t.lexeme);
					if(out!=NULL && id->dataType != out->dataType){
						if(errorFlag==1){	
							printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
							printf("Datatype mismatch in function output parameters\n");
							printf("%-23sdatatype of %s:%s, dataype expected:%s\n","",id->lexeme, enumToString(id->dataType), enumToString(out->dataType) );}
						hasSemanticError = 1;
					}

					else if(id->isArray == 1){
						if(errorFlag==1){	
							printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
							printf("Datatype mismatch in function output parameters\n");
							printf("%-23soutput parameter cannot be array\n","");}
						hasSemanticError = 1;
						//printf("datatype of %s:%s, dataype expected:%s\n",id->lexeme, enumToString(id->dataType), enumToString(out->dataType) );
					}
				}
				
				if(out == NULL){
					if(errorFlag==1){	
						printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
						printf("number of output parameters does not match with that of formal parameters\n");}
					hasSemanticError = 1;
					}
				
				
				if(out)
					out = out->next;

				if(temp->sibling){
						//N3 exists
					temp = temp->sibling->child;
					}
				else{
						//sibling is NULL
					temp=temp->sibling;
					}
					
			}
			
		if(out != NULL){
			if(errorFlag==1){	
				printf("LINE NO. : %d :ERROR: ",node->child->sibling->unode.ln.t.lineno);
				printf("number of output parameters does not match with that of formal parameters\n");}
		}

	
		return;
		}
	}
}


int checkConditional(astNode * node, moduleST * mt, int errorFlag){
	if(node->isLeaf == 0 && node->unode.nt.symbol == conditionalStmt){
		int endScope = node->idt->end;
		astNode *temp = node->child; //ID
		idST *idTable = temp->idt;

		if(idTable != NULL){
			idSTEntry * id  =  lookupidST(idTable,temp->unode.ln.t.lexeme);


			if(id->dataType == REAL || id->isArray == 1){
				if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);

					if(id->isArray == 0)
						printf("switch variable cannot be of type real\n");
					else
						printf("switch variable cannot be of type array\n");
					}
					hasSemanticError = 1;
					return 0;

				}

			else if(id->dataType == INTEGER){
				
				astNode *tmp = node->child->sibling->child; //caseSingle

				while(tmp != NULL){	//caseSingle

					astNode *valTmp = tmp->child;
					if(valTmp->isLeaf==1 && valTmp->unode.ln.t.term!=NUM){
						if(errorFlag==1){	
							printf("LINE NO. : %d :ERROR: ",valTmp->unode.ln.t.lineno);
							printf("case value is incorrect as condition variable type is integer\n");}
						hasSemanticError = 1;

					}

					if(tmp->sibling)
						tmp = tmp->sibling->child;
					else
						tmp = tmp->sibling;
				}

				tmp = node->child->sibling->sibling; //default
				if(tmp == NULL){
					if(errorFlag==1){	
						printf("LINE NO. : %d :ERROR: ",endScope);
						printf("default statement is missing- the type of switch variable is integer\n");}
					hasSemanticError = 1;
				}

			}

				else if(id->dataType == BOOLEAN){
					astNode *tmp = node->child->sibling->child; //caseSingle

					while(tmp != NULL){	//caseSingle

						astNode *valTmp = tmp->child;
						if(valTmp->isLeaf==1 && (valTmp->unode.ln.t.term!=TRUE && valTmp->unode.ln.t.term!=FALSE)){
							if(errorFlag==1){	
								printf("LINE NO. : %d :ERROR: ",valTmp->unode.ln.t.lineno);
								printf("case value is incorrect as condition variable type is boolean\n");}
							hasSemanticError = 1;

						}

						if(tmp->sibling)
							tmp = tmp->sibling->child;
						else
							tmp = tmp->sibling;
					}

					tmp = node->child->sibling->sibling; //default
					if(tmp != NULL){
						if(errorFlag==1){	
							printf("LINE NO. : %d :ERROR: ",tmp->child->unode.ln.t.lineno);
							printf("presence of default statement is incorrect as condition variable type is boolean\n");}
						hasSemanticError = 1;
					}

			}

			else{

				if(errorFlag==1){	
					printf("LINE NO. : %d :ERROR: ",temp->unode.ln.t.lineno);
					printf("switch statement with identifier of invalid dataype\n");}
				hasSemanticError = 1;
				return 0;

			}

		}

	}
	return 1;


}

void checkIterative(astNode * node, moduleST * mt, int errorFlag){

	//FOR case
	int flag =0;
	if(node->ruleNum ==0){
		;
	}

	//WHILE case
	else{
		
		astNode *tmp = node->child;//aribool
		if(tmp->unode.nt.symbol!=logicalOp && tmp->unode.nt.symbol!=relationalOp)
		{
			astNode *whilenode = findWhileEntry(tmp);
			if(whilenode == NULL) flag =1;
			int line;
			if(whilenode != NULL && whilenode->isLeaf==1)
				line = whilenode->unode.ln.t.lineno;

			if(errorFlag==1 && flag ==0){	
				printf("LINE NO. : %d :ERROR: ",line);
				printf("while condition is not a boolean expression\n");}
				
			if(flag==0) hasSemanticError = 1;
		}
		astNode *stmt = node->child->sibling;//stmts;

		travAndSet(tmp,mt,errorFlag);
		if(correct(stmt,mt,errorFlag)==1)
		{
			; //atleast one variable in while condition assigned value

		}
		else {
			
			astNode *whilenode = findWhileEntry(tmp);
			if(whilenode == NULL) flag =1;
			int line;
			if(whilenode != NULL && whilenode->isLeaf==1)
				line = whilenode->unode.ln.t.lineno;
			if(errorFlag==1 && flag ==0){	
				printf("LINE NO. : %d :ERROR: ",line);
				printf("no conditional variable in while loop assigned value\n");}
			if(flag ==0)hasSemanticError = 1;
		}
		travAndSet1(tmp,mt,errorFlag);

	}

}

int correct(astNode *tmp,moduleST *mt,int errorFlag)
{
	if(tmp==NULL)
		return 0;
	if(tmp->unode.nt.symbol==assignmentStmt)
	{
		idSTEntry *lookup =  lookupidST(tmp->child->idt, tmp->child->unode.ln.t.lexeme);
		if(lookup==NULL)
		{
			// if(errorFlag==1)	
				hasSemanticError = 1;//printf("SEMANTIC ERROR WHILE TRAV&SET VAR NOT DECLARED : %s  LINE: %d\n",tmp->unode.ln.t.lexeme,tmp->unode.ln.t.lineno);}
		}
		else
		{
			if((lookup->isAssigned%100)>=10)
				return 1;
			//printf("LHS : %s IS assigned : %d\n",tmp->child->unode.ln.t.lexeme,tmp->child->unode.ln.t.lineno);
		}
	}
	if(tmp->unode.nt.symbol==moduleReuseStmt)
	{
		astNode * temp =  tmp->child;
        astNode * modTemp = temp->sibling;
        
        //checking for <Optional> part of moduleReuse stmt
        if(temp->isLeaf == 0 && temp->unode.nt.symbol == idList){
            temp =  temp->child; //ID ;
            while(temp!=NULL && temp->idt!=NULL){

				idSTEntry *lookup =  lookupidST(temp->idt, temp->unode.ln.t.lexeme);
				if(lookup==NULL)
				{
					hasSemanticError = 1;
					//printf("SEMANTIC ERROR WHILE TRAV&SET VAR NOT DECLARED : %s  LINE: %d\n",tmp->unode.ln.t.lexeme,tmp->unode.ln.t.lineno);
				}

				else
				{
					if((lookup->isAssigned%100)>=10)
						return 1;
					//printf("LHS : %s IS assigned : %d\n",temp->unode.ln.t.lexeme,temp->unode.ln.t.lineno);
				}

				temp = temp->sibling;
                if(temp != NULL){
                    temp = temp->child;
                }
            }
        }
	}	
	if(tmp->unode.nt.symbol==ioStmt && tmp->child->isLeaf == 1 && tmp->child->unode.ln.t.term == ID)
	{
		idSTEntry *lookup =  lookupidST(tmp->child->idt, tmp->child->unode.ln.t.lexeme);
		if(lookup==NULL)
		{
			hasSemanticError = 1;
			//printf("SEMANTIC ERROR WHILE TRAV&SET VAR NOT DECLARED : %s  LINE: %d\n",tmp->unode.ln.t.lexeme,tmp->unode.ln.t.lineno);
		}
		
		else
		{
			if((lookup->isAssigned%100)>=10)
				return 1;
			// printf("LHS : %s IS assigned : %d\n",tmp->child->unode.ln.t.lexeme,tmp->child->unode.ln.t.lineno);
		}
	}
	if(tmp->unode.nt.symbol==iterativeStmt)
	{
		if(tmp->ruleNum==0)
		{
			if(correct(tmp->child->sibling->sibling,mt,errorFlag)==1)
				return 1;
			
		}
		else
		{
			if(correct(tmp->child->sibling,mt,errorFlag)==1)
				return 1;
		}
	}
	if(tmp->unode.nt.symbol==conditionalStmt)
	{
		astNode *node = tmp->child->sibling->child;//caseSingle;
		astNode *node1 = tmp->child->sibling->sibling;//default;
		if(node!=NULL&&correct(node->child->sibling,mt,errorFlag)==1)
			return 1;

		while(node->sibling!=NULL&&node->sibling->child!=NULL)
		{
			node=node->sibling->child;
			if(node!=NULL&&correct(node->child->sibling,mt,errorFlag)==1)
			return 1;
		}

		if(node1!=NULL&&correct(node1->child,mt,errorFlag)==1)
			return 1;

	}
	
	while(tmp->sibling!=NULL)	
	{
		if(correct(tmp->sibling,mt,errorFlag)==1)
			return 1;
		tmp=tmp->sibling;
	}
	return 0;
}

void travAndSet(astNode *tmp,moduleST *mt,int errorFlag)
{
	if(tmp==NULL)
		return;
	if(tmp->isLeaf==1)
	{
		if(tmp->unode.ln.t.term==ID)
		{
			idSTEntry *lookup =  lookupidST(tmp->idt, tmp->unode.ln.t.lexeme);
			if(lookup==NULL)
			{
				//printf("SEMANTIC ERROR WHILE TRAV&SET VAR NOT DECLARED : %s  LINE: %d\n",tmp->unode.ln.t.lexeme,tmp->unode.ln.t.lineno);
				hasSemanticError = 1;
			}

			else{
				if(lookup->isAssigned%100<10)
				{
					lookup->isAssigned+=10;	
				}
			}
		}
		
	}
	else
	{
		tmp=tmp->child;
		while(tmp!=NULL){
		travAndSet(tmp,mt,errorFlag);
		tmp=tmp->sibling;
		}
	}
}

void travAndSet1(astNode *tmp,moduleST *mt, int errorFlag)
{
	if(tmp==NULL)
		return;
	if(tmp->isLeaf==1)
	{
		
		if(tmp->unode.ln.t.term==ID)
		{
			idSTEntry *lookup =  lookupidST(tmp->idt, tmp->unode.ln.t.lexeme);
			if(lookup==NULL)
			{
				hasSemanticError = 1;
				//variable in while condition undeclared
				//printf("SEMANTIC ERROR WHILE TRAV&SET VAR NOT DECLARED : %s  LINE: %d\n",tmp->unode.ln.t.lexeme,tmp->unode.ln.t.lineno);
			}
			else
			if(lookup->isAssigned%100>=10)
			{
				lookup->isAssigned-=10;
			}
		}
		
	}
	else
	{
		tmp=tmp->child;
		while(tmp!=NULL){
		travAndSet1(tmp,mt,errorFlag);
		tmp=tmp->sibling;
		}
	}
}


astNode *findWhileEntry(astNode *node)
{
    if(node->isLeaf==1 && node->unode.ln.t.term==ID)
    {
        return node;
    }
    else
    {
        astNode *a;
        astNode *tmp=node->child;
        while(tmp!=NULL)
        {
        if((a=findWhileEntry(tmp))!=NULL)
           {
               return a;
           }
            tmp=tmp->sibling;
        }
    }
    return NULL;
}

Term checkArray(astNode * node, int errorFlag){//var_id_num for Rule 0

	//semantic checking for arrays
	//return ERROR if array construct has error
	astNode * temp = node->child;
	if(temp->unode.ln.t.term==ID){
		char *arrayName = temp->unode.ln.t.lexeme;
		idSTEntry *lookup =  lookupidST(temp->idt, arrayName);

	int isArray;
    int isAssigned;
    rangeNode start,end;
    int start_pop, end_pop;


    if(lookup->isArray && lookup->isStatic==1){

    	astNode *whichid = temp->sibling;
    	int arrayInd;
    	idSTEntry *lookup2 =  lookupidST(whichid->idt, whichid->unode.ln.t.lexeme);

    	if(whichid->unode.ln.t.term == NUM ){
    		arrayInd = whichid->unode.ln.t.value.v2;
    		if(lookup->start.num > arrayInd || lookup->end.num < arrayInd){
				if(errorFlag==1){	
	    			printf("LINE NO. : %d :ERROR: ",whichid->unode.ln.t.lineno);
					printf("element of array %s is out of bound\n",lookup->lexeme);}
				hasSemanticError = 1;
    			return UNASSIGNED;
    		}
    	}
    	else if(lookup2 != NULL && whichid->unode.ln.t.term == ID ){

    			if(lookup2->dataType != INTEGER){
					if(errorFlag==1){	
						printf("LINE NO %d indexed using a non integer variable %s\n",whichid->unode.ln.t.lineno,whichid->unode.ln.t.lexeme);}
					hasSemanticError = 1;
					return UNASSIGNED;
				}
				else
				{
					if(lookup->dataType==INTEGER)
						return DINT;
					if(lookup->dataType==REAL)
						return DREAL;
					if(lookup->dataType==BOOLEAN)
						return DBOOL;
				}
		}	
    	
    }
  return lookup->dataType;  
  }
  
}


