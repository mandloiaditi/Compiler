# GROUP NO :30
#ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
#ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
#ID: 2017A7PS0104P   NAME : SARGUN SINGH
#ID: 2017A7PS0160P   NAME : ADITI MANDLOI
#ID: 2017A7PS0227P   NAME : JASPREET SINGH

run	:	lexer.o parser.o ast.o utils.o	driver.o	id.o	func.o	symbol.o	semantic.o	ic.o cg.o	semanticAnalyzer.h
	gcc -o compiler utils.o lexer.o parser.o ast.o driver.o id.o	func.o	symbol.o	semantic.o	ic.o cg.o

utils.o	:	utils.c	utils.h 
	gcc -c  -o utils.o	utils.c
lexer.o	:	lexer.c lexer.h	utils.h
	gcc -c  -o lexer.o	lexer.c
parser.o	:	parser.h utils.h lexer.h parser.c
	gcc -c  -o parser.o parser.c
ast.o	:	ast.h parser.h utils.h lexer.h ast.c
	gcc -c  -o ast.o ast.c
id.o	:	idTable.c utils.h parserDef.h lexerDef.h symbolTableDef.h	semanticAnalyzer.h
	gcc -c -o id.o idTable.c
func.o	:	funcTable.c utils.h parserDef.h lexerDef.h symbolTableDef.h	semanticAnalyzer.h
	gcc -c -o func.o funcTable.c
symbol.o	:	utils.h parserDef.h lexerDef.h symbolTableDef.h	semanticAnalyzer.h	symbol.c
	gcc	-c	-o	symbol.o	symbol.c
semantic.o	:	utils.h parserDef.h lexerDef.h symbolTableDef.h	symbol.h	ast.h	semanticAnalyzer.h semanticAnalyzer.c
	gcc	-c	-o	semantic.o	semanticAnalyzer.c
ic.o	:	utils.h parserDef.h lexerDef.h symbol.h	intermediateCode.h	semanticAnalyzer.h	intermediateCode.c 
	gcc -c -o ic.o intermediateCode.c
cg.o	:	utils.h parserDef.h lexerDef.h symbol.h	intermediateCode.h codeGen.h	semanticAnalyzer.h codeGen.c
	gcc -c -o cg.o codeGen.c
driver.o	:	utils.h parser.h lexer.h ast.h symbol.h semanticAnalyzer.h intermediateCode.h codeGen.h
	gcc -c  -o driver.o driver.c



