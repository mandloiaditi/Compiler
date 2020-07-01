----ERPLAG COMPILER----

##Pre-requisites
Ubuntu 16.04
GCC version 5.4.0 
NASM 2.14.02 (64-bit)

##Modules
The project has been implemented in a pipelined manner with the modules consisting of:

1. Lexer (lexer.c)
2. Parser (parser.c)
3. AST creation (ast.c)
4. Symbol Table construction, Type extraction and Scoping (symbol.c)
5. Semantic Analysis and Type checking (semanticAnalyzer.c)
6. Intermediate Code Generation (intermediateCode.c)
7. Code Generation in Assembly (codeGen.

##Instructions for running testcases

##Use make to compile the compiler C-code

make
./compiler testcase.txt code.asm

##Compile and run asm file using NASM

# a. Ubuntu 16.04

nasm -f elf64 -o output.o code.asm
gcc output.o
./a.out

# b. Ubuntu 18.04 and higher versions

nasm -f elf64 -o output.o code.asm
gcc output.o -no-pie
./a.out




