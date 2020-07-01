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
#include "symbolTableDef.h"



moduleSTEntry* insertInModuleHashTable(moduleSTEntry* val, hashTable_func* mappingTable){
    char * ptr  = val->lexeme_mod;
    ll f = computehash(ptr);
    if(mappingTable[f].str==NULL){
        mappingTable[f].str =(char*) malloc(strlen(ptr)+1);
        strcpy(mappingTable[f].str,ptr);
        strcpy(mappingTable[f].data.lexeme_mod,val->lexeme_mod);
        mappingTable[f].data = *val;
        return &mappingTable[f].data;

    }
    else{
        if(mappingTable[f].next==NULL ){
            mappingTable[f].next = (hashTable_func*)malloc(sizeof(hashTable_func));
            hashTable_func* tmp=mappingTable[f].next;
            tmp->next=NULL;
        tmp->str =(char*) malloc(strlen(ptr)+1);
        strcpy(tmp->str,ptr);
        tmp->data = *val;
        return &(tmp->data);
        }
        else{
            hashTable_func* tmp = mappingTable[f].next;
            while(1){
                if(tmp->next==NULL )break;
                tmp = tmp->next;
            }
            tmp -> next = (hashTable_func*)malloc(sizeof(hashTable_func));
            tmp = tmp->next;
            tmp->next=NULL;
        tmp->str =(char*) malloc(strlen(ptr)*sizeof(char)+1);
        strcpy(tmp->str,ptr);
        tmp->data = *val;
        return &(tmp->data);
        }
    }
}



moduleSTEntry * searchModuleHashTable(char* str, hashTable_func* mappingTable){
    int ha = computehash(str);
    hashTable_func* ans  = (hashTable_func*)malloc(sizeof(hashTable_func));
    if(mappingTable[ha].str==NULL)return NULL;
    if(mappingTable[ha].next==NULL){
        ans = &mappingTable[ha];
        if(strcmp(ans->str,str)==0)
                return &(ans->data);
        else
            return NULL;
    }
    hashTable_func* tmp = &mappingTable[ha];
    while(tmp){
        if(strcmp(tmp->str,str)==0){
        return &(tmp->data);
        }
        tmp = tmp->next;
    }
    return NULL;
}

hashTable_func* createModHashTable(){
    ll i=0;
    hashTable_func* mappingTable = (hashTable_func*) malloc(sizeof(hashTable_func)*N);
    for(i=0;i<N;i++){
        mappingTable[i].str = NULL ;
        mappingTable[i].next = NULL;
    }
    return mappingTable;
}
