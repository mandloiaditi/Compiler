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
#include "intermediateCode.h"

#ifndef _CG_H
#define _CG_H

void codeGeneration(quadruple *head, char *outfile);


#endif