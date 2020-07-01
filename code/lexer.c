// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>
#include"utils.h"
#include"lexer.h"


// TWIN-BUFFER SCHEME USED
extern int eof;
extern FILE *fptr;
extern char buffer1[BUFF_SIZE + 1];
extern char buffer2[BUFF_SIZE + 1];
extern char *lexStart, *fwd;
extern int buffptr1, buffptr2, lexSize, line;
extern hashmap keyWordTable[N];

Term keywordEnum(char*str){
    return stringToEnum(str,keyWordTable);    
}


token* fillToken(char* lex, Term t, unsigned int lineno, int isKey, val value, int tag){
    
    token* res = (token*) malloc(sizeof(token));
    res->lexeme = (char*) malloc(sizeof(char) * strlen(lex));
    strcpy(res->lexeme, lex);
    res->term = t; // fill enum here
    res->lineno = lineno;
    res->isKeyword = isKey;
    res->value = value;
    res->tag = tag;
    return res;
}

float convertToFloat(char* str){
    
    float ans = 0.0;
    ans = atof(str);
    return ans;
}

void removeComments(char* readfrom){    //remove comments with preserving line numbers
    FILE *fptr = fopen(readfrom,"r");
    char buffer1[BUFF_SIZE + 1];
    char buffer2[BUFF_SIZE + 1];
    char *lexStart, *fwd;
    int buffptr1, buffptr2,lexSize,line,eof=0;
    int siz = fread(buffer1, sizeof(char), BUFF_SIZE, fptr);
    
    buffer1[siz] = EOF;
    lexStart = buffer1;
    fwd = buffer1;
    lexSize = 0;
    line = 1;
    int start = 0;
    int ch = 1;
    int prs=siz;
    while(1){
        if(*fwd == EOF)
        {
            if(fwd == buffer1 + BUFF_SIZE)
            {
                int siz = fread(buffer2, sizeof(char), BUFF_SIZE, fptr);
                buffer2[siz] = EOF;
                fwd = buffer2;
                ch = 2;
                if(start == 1 && *fwd == '*'){
                    buffer1[BUFF_SIZE - 1] = ' ';
                    *fwd = ' ';
                    start = 2;
                }
                else if(start == 3 && *fwd == '*'){
                    *fwd = ' ';
                    start = 0;
                }
                else if(start == 3){
                    start = 2;
                    if(*fwd != '\n') *fwd = ' ';
                }
                else if(start ==1){
                    start = 0;
                }
                else if(start == 2){
                    if(*fwd == '*'){
                        start = 3;
                        if(*fwd != '\n') *fwd=' ';
                    }
                    else{
                        if(*fwd != '\n') *fwd=' ';
                    }
                }
                else {
                    if(*fwd == '*')start = 1;
                }
                for(int i=0;i<prs;i++)
                    printf("%c",buffer1[i]);
                prs = siz;
                if(*fwd == EOF)break;
            }
            else if(fwd == buffer2 + BUFF_SIZE)
            {
                int siz = fread(buffer1, sizeof(char), BUFF_SIZE, fptr);
                buffer1[siz] = EOF;
                fwd = buffer1;
                ch = 1;
                if(start == 1 && *fwd == '*'){
                    buffer2[BUFF_SIZE - 1] = ' ';
                    *fwd = ' ';
                    start = 2;
                }
                else if(start == 3 && *fwd == '*'){
                    start = 0;
                    if(*fwd != '\n') *fwd = ' ';
                }
                else if(start == 3){
                    start = 2;
                    if(*fwd != '\n') *fwd = ' ';
                }
                else if(start == 1){
                    start = 0;
                }
                else if(start == 2){
                    if(*fwd == '*'){
                        start = 3;
                        if(*fwd != '\n') *fwd=' ';
                    }
                    else{
                        if(*fwd != '\n') *fwd=' ';
                    }
                }
                else if(start == 3){
                    if(*fwd == '*') start = 0;
                    if(*fwd != '\n') *fwd = ' ';
                }
                else {
                    if(*fwd=='*')start = 1;
    
                }
            
                for(int i=0;i<prs;i++)
                    printf("%c",buffer2[i]);
                prs = siz;
                if(*fwd == EOF)break;
            }
            else{
                eof = 1;
                if(ch == 1){
                    
                  
                    for(int i=0;i<prs;i++)
                        printf("%c",buffer1[i]);
                }
                else {
                   
                    for(int i=0;i<prs;i++)
                        printf("%c",buffer2[i]);
                }
                break;
            }
            fwd++;
        }
        else{
            char c = *fwd;
            if(c == EOF){
                break;
            }
            if(start == 0){
                if(c == '*'){
                    start = 1;
                }
            }
            else if(start == 1){
                if(c == '*'){
                    start = 2;
                    *(fwd - 1) = ' ';
                    *fwd = ' ';
                }
                else{
                    start = 0;
                }
    
            }
            else if(start == 2){
                if(c == '*'){
                    if(*fwd != '\n') *fwd =' ';
                    start = 3 ;
                }
                else{
                    if(*fwd != '\n') *fwd = ' ';
                }
                
            }
            else if(start == 3){
                if(c == '*'){
                    start = 0;
                    if(*fwd != '\n') *fwd = ' ';
                }
                else{
                    start = 2;
                    if(*fwd != '\n') *fwd = ' ';
                }
            }
            fwd++;
        }
    }
}

void initializeLexerForParser(char *fname)
{

    initialize_lookup_map1(keyWordTable);
    initialize_lookup_map2(genericMappingTable);
    fptr = fopen(fname, "r");
    assert(fptr != NULL);
    int siz = fread(buffer1, sizeof(char), BUFF_SIZE, fptr);
    buffer1[siz] = EOF;
    lexStart = buffer1;
    fwd = buffer1;
    lexSize = 0;
    line = 1;
    eof = 0;
    buffptr1 = buffptr2 = 1;
}

void initializeLexer(char *fname, int errFlag)
{
    initialize_lookup_map1(keyWordTable);
    initialize_lookup_map2(genericMappingTable);
    fptr = fopen(fname, "r");
    assert(fptr != NULL);
    int siz = fread(buffer1, sizeof(char), BUFF_SIZE, fptr);
    buffer1[siz] = EOF;
    lexStart = buffer1;
    fwd = buffer1;
    lexSize = 0;
    line = 1;
    eof = 0;
    buffptr1 = buffptr2 = 1;
    token *c1 ;
    printf("%-10s%-30s%-20s\n","LINE NO.","LEXEME","TOKEN NAME");
    while((c1= getNextToken(errFlag))!=NULL){
    
        printf("%-10d%-30s%-20s\n",c1->lineno,c1->lexeme,enumToString(c1->term));
    }
    fclose(fptr);
}

void retract(int val){
    
    lexSize -= val;
    if(fwd >= buffer1 && fwd < buffer1 + val)
    {
        int tmp = val - (fwd - buffer1);
        fwd = buffer2 + BUFF_SIZE - tmp;
        buffptr1 = 0;
    }
    else if(fwd >= buffer2 && fwd < buffer2 + val)
    {
        int tmp = val - (fwd - buffer2);
        fwd = buffer1 + BUFF_SIZE - tmp;
        buffptr2 = 0;
    }
    else
        fwd -= val;
}

char* give_lexeme(){
    
    retract(1);
    char* truelexeme = (char*) malloc(sizeof(char)*32);
    char* cur = lexStart;
    int i = 0;
    while( cur != fwd ){
        if(cur == (buffer2 + BUFF_SIZE)){
            cur = buffer1;
        }
        else if(cur == (buffer1 + BUFF_SIZE)){
            cur = buffer2;
        }
        if(cur == fwd)break;
        truelexeme[i++] = *cur;
        cur++;
    }
    truelexeme[i] = '\0';
    lexStart = fwd;
    return truelexeme;
}


int isKey(char *lexeme){
    
    return fin(lexeme,keyWordTable);
}

char getNextChar()
{
    if(*fwd == EOF)
    {
        if(fwd == buffer1 + BUFF_SIZE)
        {
            if(buffptr2 == 1)
            {
                int siz = fread(buffer2, sizeof(char), BUFF_SIZE, fptr);
                buffer2[siz] = EOF;
                buffptr2 = 0;
            }
            fwd = buffer2;
            buffptr1 = 1;
        }
        else if(fwd == buffer2 + BUFF_SIZE)
        {
            if(buffptr1 == 1)
            {
                int siz = fread(buffer1, sizeof(char), BUFF_SIZE, fptr);
                buffer1[siz] = EOF;
                buffptr1 = 0;
            }
            fwd = buffer1;
            buffptr2 = 1;
        }
        else{
            eof = 1;
        }
    }

    char c = *fwd;
    fwd++;
    return c;
}

token* getNextToken(int errFlag){  //called by parser to get the next token from lexer
    
    token* null_ptr = NULL;
    if(eof == 1)
        return null_ptr;
    
    char c;
    int fin = 0, state = 0 ;
    int ptr = 0, length_id = 0;
    
    while(!fin){
        c=getNextChar();
        
        //DFA Implementation
        switch(state){
            case 0:{
                if(c == '+'){
                    state = 2;
                }
                else if(c == '-'){
                    state = 3;
                }
                else if(c == '*'){
                    state = 4;
                }
                else if(c == '/'){
                    state = 7;
                }
                else if(c == '['){
                    state = 27;
                }
                else if(c == ']'){
                    state = 28;
                }
                else if(c == '('){
                    state = 29;
                    ptr++;
                }
                else if(c == ')'){
                    state = 30;
                }
                else if(c == ','){
                    state = 26;
                }
                else if(c == ';'){
                    state = 25;
                }
                else if(c == '<'){
                    state = 8;
                }
                else if(c == '.'){
                    state = 23;
                }
                else if(c == '>'){
                    state = 12;
                }else if(c == '='){
                    state = 16;
                }
                else if(c == '!'){
                    state = 18;
                }
                else if(c == ':'){
                    state = 20;
                }
                else if(c == '\n'){
                    lexStart = fwd;
                    line++;
                }
                else if(isdigit(c)){
                    state=32;
                }
                else if(isalpha(c)){
                    state = 38;
                    length_id = 0;
                }
                else if(c == ' ' || c == '\t' || c == '\r'){

                    while(c != ' ' && c != '\t' && c != '\r')
                        c = getNextChar();
                    lexStart = fwd;
                }
                else if(eof == 1){
                    break;
                }

                else{
                    state = 0;
                    if(errFlag == 1)
                        printf("Line:%d Lexical Error: ",line);
                    fwd++;
                    if(errFlag == 1)
                        printf("%s\n",give_lexeme());
                    lexStart=fwd;
                }
            }break;
            case 2:{
                fin=1;
                Term c= PLUS;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);

            }break;
            case 3:{
                fin=1;
                Term c= MINUS;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 4:{
                if(c=='*'){
                    state=6;
                }
                else {
                    state=5;
                }
            }break;
            case 5:{
                fin=1;
                retract(1);
                Term c= MUL;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 6:{
                if(c=='*'){
                    state=39;
                }
                else{
                    if(c == '\n') line ++;
                    state=6;
                }
            }break;
            case 7:{
                fin=1;
                Term c= DIV;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 8:{
                if(c=='='){
                    state=9;
                }
                else if(c=='<'){
                    state=10;
                }
                else {
                    state=11;
                }
            }break;
            case 9:{
                fin=1;
                Term c= LE;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 10:{
                if(c=='<'){
                    state= 43;
                }
                else{
                    fin=1;
                    Term c= DEF;
                    val num_val;
                    num_val.v2=0;
                    return fillToken(give_lexeme(),c,line,0,num_val,0);

                }
            }break;
            case 11:{
                fin=1;
                retract(1);
                Term c= LT;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 12:{
                if(c=='='){
                    state=13;
                }
                else if(c=='>'){
                    state=14;
                }
                else {
                    state=15;
                }
            }break;
            case 13:{
                fin=1;
                Term c= GE;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 14:{
                if(c=='>'){
                    state=44;
                }
                else{
                    fin=1;
                    Term c= ENDDEF;
                    val num_val;
                    num_val.v2=0;
                    return fillToken(give_lexeme(),c,line,0,num_val,0);
                }

            }break;
            case 15:{
                fin=1;
                retract(1);
                Term c= GT;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 16:{
                if(c=='='){
                    state=17;
                }
                else{
                    state=0;
                    if(errFlag == 1)
                        printf("Line:%d Lexical Error: ",line);
                    retract(1);
                    fwd ++;
                    if(errFlag == 1)
                        printf(" %s\n",give_lexeme());
                    lexStart=fwd;
                }
            }break;
            case 17:{
                fin=1;
                Term c= EQ;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 18:{
                if(c=='='){
                    state=19;
                }
                else{
                    state=0;
                    if(errFlag == 1)
                        printf("Line:%d Lexical Error: ",line);
                    retract(1);
                    fwd ++;
                    if(errFlag == 1)
                        printf("%s\n",give_lexeme());
                    lexStart=fwd;
                }
            }break;
            case 19:{
                fin=1;
                Term c= NE;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 20:{
                if(c=='='){
                    state=21;
                }
                else {
                    state=22;
                }
            }break;
            case 21:{
                fin=1;
                Term c= ASSIGNOP;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 22:{
                fin=1;
                retract(1);
                val num_val;
                num_val.v2=0;
                Term c= COLON;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 23:{
                if(c=='.'){
                    state=24;
                }
                else{
                    state=0;
                    if(errFlag == 1)
                        printf("Line:%d Lexical Error: ",line);
                    retract(1);
                    fwd ++;
                    if(errFlag == 1)
                        printf("%s\n",give_lexeme());
                    lexStart=fwd;
                }
            }break;
            case 24:{
                fin=1;
                Term c= RANGEOP;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 25:{
                fin=1;
                Term c= SEMICOL;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 26:{
                fin=1;
                Term c= COMMA;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 27:{
                fin=1;
                Term c= SQBO;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 28:{
                fin=1;
                Term c= SQBC;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 29:{
                fin=1;
                Term c= BO;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 30:{
                fin=1;
                Term c= BC;
                val num_val;
                num_val.v2=0;
                return fillToken(give_lexeme(),c,line,0,num_val,0);
            }break;
            case 32:{
                if(isdigit(c)){
                    state=32;
                }
                else if(c=='.'){
                    state=33;
                }
                else if(eof==1){
                    fin=1;
                    Term c= NUM;
                    val num_val;
                    char *g= give_lexeme();
                    num_val.v2=atoi(g);
                    return fillToken(g,c,line,0,num_val,1);
                }
                else{
                    state=40;
                }
            }break;
            case 33:{
                if(isdigit(c)){
                    state=34;
                }
                else if(c=='.')
                {
                    retract(1);
                    fin=1;
                    Term c= NUM;
                    char *r = give_lexeme();
                    val num_val;
                    num_val.v2 = atoi(r);
                    return fillToken(r,c,line,0,num_val,1);
                }
                else{
                    state=0;
                    if(errFlag == 1)
                        printf("Line:%d Lexical Error: ",line);
                    retract(1);
                    fwd ++;
                    if(errFlag == 1)
                        printf("%s\n",give_lexeme());
                    lexStart=fwd;
                }
            }break;
            case 34:{
                if(isdigit(c)){
                    state=34;
                }
                else if(c=='e' || c=='E'){
                    state=35;
                }
                else if(eof==1){
                    fin=1;
                    Term c= RNUM;
                    val num_val;
                    char *y=give_lexeme();
                    num_val.v1=convertToFloat(y);
                    return fillToken(y,c,line,0,num_val,0);
                }
                else{
                    state=41;
                }
            }break;
            case 35:{
                if(isdigit(c)){
                    state=36;
                }
                else if(c=='+' || c=='-'){
                    state=37;
                }
                else{
                    state=0;
                    if(errFlag == 1)
                        printf("Line:%d Lexical Error: ",line);
                    retract(1);
                    fwd ++;
                    if(errFlag == 1)
                        printf("%s\n",give_lexeme());
                    lexStart=fwd;
                }
            }break;
            case 36:{
                if(isdigit(c)){
                    state=36;
                }
                else if(eof==1){
                    fin=1;
                    Term c= RNUM;
                    val num_val;
                    char *y=give_lexeme();
                    num_val.v1=convertToFloat(y);
                    return fillToken(y,c,line,0,num_val,0);
                }
                else{
                    state=41;
                }
            }break;
            case 37:{
                if(isdigit(c)){
                    state=36;
                }

                else{
                    state=0;
                    if(errFlag == 1)
                        printf("Line:%d Lexical Error: ",line);
                    retract(1);
                    fwd++;
                    if(errFlag == 1)
                        printf("%s\n",give_lexeme());
                    lexStart=fwd;
                }
            }break;
            case 38:{
                if(isalpha(c) || isdigit(c) || c=='_'){
                    length_id++;
                    state=38;
                    if(length_id>20){
                        
                        while(isalpha(c) || isdigit(c) || c=='_')
                            c=getNextChar();
                        if(errFlag == 1){
                            printf("Line:%d Lexical Error: ",line);
                            printf("%s\n",give_lexeme());
                        }
                        lexStart=fwd;
                        state=0;
                    }
                }
                else if(eof==1){
                    fin=1;
                    char * c= give_lexeme();
                    int g = isKey(c);
                    val num_val;
                    num_val.v2=0;
                    if(g){
                        Term cd= keywordEnum(c);
                        return fillToken(c,cd,line,g,num_val,0);
                    }
                    else{
                        Term cd= ID;
                        return fillToken(c,cd,line,0,num_val,0);
                    }
                }
                else{
                    state=42;
                }
            }break;
            case 39:{
                if(c=='*'){
                    state=31;
                }
                else{
                    if(c == '\n') line ++;
                    state=6;
                }
            }break;
            case 31:{
                retract(1); // When commment line ends;
                state=0;
                lexStart=fwd;
            }break;
            case 40:{
                retract(1);
                fin=1;
                Term c= NUM;
                val num_val;
                char *g= give_lexeme();
                num_val.v2=atoi(g);
                return fillToken(g,c,line,0,num_val,1);

            }break;
            case 41:{
                retract(1);
                fin=1;
                Term c= RNUM;
                val num_val;
                char *y=give_lexeme();
                num_val.v1=convertToFloat(y);
                return fillToken(y,c,line,0,num_val,0);

            }break;
            case 42:{
                fin=1;
                retract(1);
                char * c= give_lexeme();
                int g = isKey(c);
                val num_val;
                num_val.v2=0;
                if(g){
                    Term cd= keywordEnum(c);
                    return fillToken(c,cd,line,g,num_val,0);
                }
                else{
                    Term cd= ID;
                    return fillToken(c,cd,line,0,num_val,0);
                }
            }break;

            case 43:{
                fin=1;
                Term c= DRIVERDEF;
                char *r = give_lexeme();
                val num_val;
                num_val.v2 = 0;
                return fillToken(r,c,line,0,num_val,0);

            }break;

            case 44:{
                fin=1;
                Term c= DRIVERENDDEF;
                char *r = give_lexeme();
                val num_val;
                num_val.v2 = 0;
                return fillToken(r,c,line,0,num_val,0);
                
            }break;
        }

        if(eof==1)
            break;
    }
    return null_ptr;
}
    