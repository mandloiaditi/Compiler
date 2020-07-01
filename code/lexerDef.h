// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH
#include "utils.h"
#ifndef _LEXERD_H
#define _LEXERD_H

typedef union choice{
    int v2;
    float v1;
}val;


typedef struct tok{
    char* lexeme;
    Term term;
    unsigned int lineno;
    int isKeyword;
    val value;
    int tag;
    
}token;
#endif