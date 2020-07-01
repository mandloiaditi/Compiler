// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "utils.h"
#include "ast.h"
#include "symbol.h"
#include "semanticAnalyzer.h"
#include "intermediateCode.h"
#include "codeGen.h"

int main(int argc, char *argv[]){

	printf("========Implementation Status========\n");
	printf("LEVEL 4:\n\n");
	printf("(a) FIRST and FOLLOW sets automated\n");
	printf("(b) Both lexical and syntax analysis modules implemented\n");
	printf("(c) Parse tree construction completed\n");
	printf("(d) AST construction completed\n");
	printf("(e) Symbol Table population completed\n");
	printf("(f) Semantic rules checking completed\n");
	printf("(g) Type checking completed\n");
	printf("(h) Handled static and dynamic arrays in type checking and code generation\n\n");


	if(argc!=3){
		printf("Incorrect number of arguments\n");
		return 0;
	}
	
	char *testFile = argv[1];
	char *codeGenOut = argv[2];
	 
	int input=0;
		
		
	while(1){
		


		printf("\n\n\n===========Option Choices===========\n");
		printf("(0) Exit\n");
		printf("(1) Print token list\n");
		printf("(2) Print Parse Tree\n");
		printf("(3) Print Abstract Syntax Tree (AST)\n");
		printf("(4) Print Memory usage for Parse tree and AST\n");
		printf("(5) Print Symbol Table\n");
		printf("(6) Print Activation record size\n");
		printf("(7) Print Static and Dynamic arrays\n");
		printf("(8) Verify Syntactical and Semantic Correctness (print compiling time)\n");
		printf("(9) Assembly Code Generation\n");
		
		printf("Enter input:\n");
		scanf("%d", &input);
		if(input==0){
			printf("\n\ninput:%d EXITED\n\n",input);
			return 0;
		}
	
		else if(input==1){
			printf("\ninput:%d ONLY LEXER\n\n",input);		//print token list on console
			initializeLexer(testFile, 0);
			
		}
		
		else if(input==2){
			printf("\ninput:%d LEXER AND PARSER\n\n",input);		//print all errors lexical and syntactic, line number wise on console, print parse tree in argv[2]
			initializeParser(testFile,codeGenOut,0,1,0);
		}
		
		else if(input==3){
			printf("\ninput:%d AST\n\n",input);		//print abstract syntax tree
			initializeParser(testFile,codeGenOut,0,0,0);

			if(hasSyntacticError == 1){

				printf("Code has syntactic errors. Unable to create the AST.\n");
			}

			else{
				printf("\nPrinting in-order traversal of the AST. Parent of each node has been printed alongside for easier reference.\n\n");
				astNode *root = generateAST(codeGenOut, 1);
			}	
		}
		
		else if(input==4){
			printf("\ninput:%d MEMORY USAGE FOR PARSE TREE AND AST\n\n",input);		//print memory usage
			initializeParser(testFile,codeGenOut,1,0,0);

			if(hasSyntacticError == 1){

				printf("Code has syntactic errors. Unable to create the AST.\n");
			}

			else{
				//printf("\nPrinting in-order traversal of the AST. Parent of each node has been printed alongside for easier reference.\n\n");
				astNode *root = memUsage(codeGenOut);
				float diff = ptMemUsage - astMemUsage;
				float compressionPercentage = (diff/ptMemUsage)*100;
				printf("compression Percentage = %f%%\n", compressionPercentage);
				//print memory usage 
			}	
		
			
		}
		
		
		else if(input==5){
			printf("\ninput:%d SYMBOL TABLE\n\n",input);		//print Symbol Table
			initializeParser(testFile,codeGenOut,0,0,0);

			if(hasSyntacticError == 1){

				printf("Code has syntactic errors. Unable to create the AST.\n");
				continue;
			}

			astNode *root = generateAST(codeGenOut, 0);
			populateSymbolTable(root, 0);
			moduleST * mt = getmt();
			printf("\nPrinting Symbol Table:\n\n");
			printSymbolTable(mt);

		}

		else if(input==6){
			printf("\ninput:%d ACTIVATION RECORD SIZE\n\n",input);		//print Activation Record sizes
			initializeParser(testFile,codeGenOut,0,0,0);

			if(hasSyntacticError == 1){

				printf("Code has syntactic errors. Unable to create the AST.\n");
				continue;
			}

			astNode *root = generateAST(codeGenOut, 0);
			populateSymbolTable(root, 0);
			moduleST * mt = getmt();
			printActivationRecSize(mt);

		}

		else if(input==7){
			printf("\ninput:%d STATIC AND DYNAMIC ARRAYS\n\n",input);	//print static and dynamic arrays
			initializeParser(testFile,codeGenOut,0,0,0);

			if(hasSyntacticError == 1){

				printf("Code has syntactic errors. Unable to create the AST.\n");
				continue;
			}

			astNode *root = generateAST(codeGenOut, 0);
			populateSymbolTable(root, 0);
			moduleST * mt = getmt();
			printArrays(mt);

		}

		else if(input==8){
			printf("\ninput:%d ERROR REPORTING AND TOTAL COMPILING TIME\n\n",input);		//print Semantic errors
			

			clock_t start_time, end_time;
			double total_CPU_time, total_CPU_time_in_seconds;
			start_time = clock();

            initializeParser(testFile,codeGenOut,0,0,1);       // invoke your compiler here
            if(hasSyntacticError == 1){

				printf("Code has syntactic errors. Unable to create the AST.\n");
				continue;
			}


			astNode *root = generateAST(codeGenOut, 0);
			populateSymbolTable(root, 1);
			moduleST * mt = getmt();
			semanticAnalyzer(root,mt, 1);

			if(hasSemanticError == 0){
				printf("Code compiled successfully.\n");
			}

			end_time = clock();
			total_CPU_time  =  (double) (end_time - start_time);
			total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

			// Print both total_CPU_time and total_CPU_time_in_seconds 
			
			printf("\ntotal_CPU_time :%lf\ntotal_CPU_time_in_seconds:%lf\n\n",total_CPU_time, total_CPU_time_in_seconds);

		}


		else if(input==9){
			printf("\ninput:%d CODE GENERATION\n\n",input);		//Code generation
			initializeParser(testFile,codeGenOut,0,0,0);

			if(hasSyntacticError == 1){

				printf("Code has syntactic errors. Unable to create the AST.\n");
				continue;
			}

			astNode *root = generateAST(codeGenOut, 0);
			populateSymbolTable(root, 0);
			moduleST * mt = getmt();
			semanticAnalyzer(root,mt, 0);

			if(hasSemanticError == 1){
				printf("Code has semantic errors. Cannot proceed to Code Generation.\n");
				continue;
			}

			
			quadruple *head =moduleTraverse(root);
			printf("Generating Assembly Code\n");
			codeGeneration(head, codeGenOut);

		}


		else {

			printf("\nIncorrect option. Try again\n\n");

		}
		
	
	}

}


