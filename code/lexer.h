// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH
#include <stdlib.h>
#include <stdio.h>
#include "lexerDef.h"
#ifndef _LEXER_H
#define _LEXER_H

token* getNextToken(int errFlag);
void addEntry(char* ptr, int val);
token* fillToken(char* lex, Term t, unsigned int lineno, int isKey, val value, int tag);
void retract(int val);
void removeComments(char* readfrom);
void initializeLexer(char *fname, int errFlag);
void initializeLexerForParser(char *fname);
char* give_lexeme();
int isKey(char *lexeme);
char getNextChar();

#endif