// GROUP NO :30
//ID: 2017A7PS0050P   NAME : JIVAT NEET KAUR
//ID: 2017A7PS0088P   NAME : VAISHNAVI KOTTURU
//ID: 2017A7PS0104P   NAME : SARGUN SINGH
//ID: 2017A7PS0160P   NAME : ADITI MANDLOI
//ID: 2017A7PS0227P   NAME : JASPREET SINGH



#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#define NUM_NT 57
#define MAX_RHSRULES 10

extern hashmap genericMappingTable[N];
extern treeNode * treeHead;
extern int num_T;
int hasSyntacticError =0;
int ptMemUsage = 0;

int numOfTerminals(){
	return DOLLAR-NUM_NT;
}


// FUNCTIONS FOR STACK 
stackNode * initialize_stack(){
    stackNode * initial = (stackNode * )malloc(sizeof(stackNode));
    (initial->position) = (treeNode *)malloc(sizeof(treeNode));
    (initial->position)->isTerminal = 1;
    (initial->position)->unode.ln.t.term=DOLLAR;
    initial->next = NULL;
    initial->position->unode.ln.t.lineno=-1;
    initial->position->unode.ln.t.isKeyword=0;
    (initial->position->unode).ln.child=NULL;
    (initial->position->unode).ln.sibling=NULL;
    
    return initial;
}
stackNode * push(stackNode *s,stackNode *head){
    s->next=head;
    head=s;
    return head;
}
stackNode * pop(stackNode *head){
    if(head==NULL)
    return head;//    
    
    stackNode * tmp = head;
    head=head->next;
    return head;
      
}


// FUNCTIONS FOR PARSE TREE

//print parse tree in order
void inorder(treeNode *treeHead,FILE * out, int errorFlag){
    if(treeHead==NULL)
        return;
    if(treeHead->isTerminal==1)
    {
        printTN(treeHead,out,errorFlag);
        return ;
    }
    else
    {
        inorder(treeHead->unode.nt.child,out,errorFlag);
        printTN(treeHead,out,errorFlag);
        treeNode *tmp = treeHead->unode.nt.child;
        if(tmp==NULL)
            return;
        if(tmp->isTerminal==1)
        {
            tmp=tmp->unode.ln.sibling;
        }
        else
        {
            tmp=tmp->unode.nt.sibling;
        }
        while(tmp!=NULL)
        {
            inorder(tmp,out,errorFlag);
            if(tmp->isTerminal==1)
            {
                tmp=tmp->unode.ln.sibling;
            }
            else
            {
                tmp=tmp->unode.nt.sibling;
            }

        }
    }
}
treeNode *child(treeNode *n)
{
    if(n->isTerminal==1)
    {
        return NULL;
    }
    else
    {
        return n->unode.nt.child;
    }
}

treeNode *sibling(treeNode *n)
{
    if(n->isTerminal==1)
    {
        return n->unode.ln.sibling;
    }
    else
    {
        return n->unode.nt.sibling;
    }
}
int count2 ;
void printTN(treeNode *n, FILE * out, int errorFlag)
{
    if(n==NULL){
    	return;
    }
    if(n->isTerminal==1)
    {
    	token temp =  n->unode.ln.t ;
    	count2 ++;
    	if(temp.term != e){
        // printf("%-28s%-10d%-16s",temp.lexeme, temp.lineno, enumToString(temp.term));
    		if(errorFlag == 1)
    			printf("%-28s%-10d%-16s",temp.lexeme, temp.lineno, enumToString(temp.term));}
    	else{
    	// printf("---%-25s---%-7s%-16s","","", enumToString(temp.term));
    		if(errorFlag == 1)
    			printf("---%-25s---%-7s%-16s","","", enumToString(temp.term));}
        if(temp.term == RNUM){
        	// printf("%-13.10f", temp.value.v1);
        	if(errorFlag == 1)
        		printf("%-13.10f", temp.value.v1);
        }

        else if(temp.term == NUM){
        	// printf("%-13d", temp.value.v2);
        	if(errorFlag == 1)
        		printf("%-13d", temp.value.v2);
        }

        else{
        	// printf("---%-10s","");
        	if(errorFlag == 1)
        		printf("---%-10s","");
        }

        // printf("%-28s%-11s%-25s\n",enumToString((n->unode.ln.parent)->unode.nt.value),"YES",enumToString(temp.term));
        if(errorFlag == 1)
        	printf("%-28s%-11s%-25s\n",enumToString((n->unode.ln.parent)->unode.nt.value),"YES",enumToString(temp.term));

    }
    else
    {	count2 ++;
    	if(n->unode.nt.parent!= NULL){
        // printf("---%-25s---%-7s---%-13s---%-10s%-28s%-11s%-25s\n","","","","",enumToString((n->unode.nt.parent)->unode.nt.value),"NO",enumToString(n->unode.nt.value));
        	if(errorFlag == 1)
        		printf("---%-25s---%-7s---%-13s---%-10s%-28s%-11s%-25s\n","","","","",enumToString((n->unode.nt.parent)->unode.nt.value),"NO",enumToString(n->unode.nt.value));
    	}
    	else {
    	// printf("---%-25s---%-7s---%-13s---%-10s---%-25s%-11s%-25s\n","","","","","","NO",enumToString(n->unode.nt.value));
    		if(errorFlag == 1)
    			printf("---%-25s---%-7s---%-13s---%-10s%-28s%-11s%-25s\n","","","","","ROOT","NO",enumToString(n->unode.nt.value));
    }	}
}

//read Grammar from grammar.txt
grammar readGrammar(FILE * ptr){
	grammar g;
	char line[150];
	char rhs[20];
	for(int  i  = 0 ; i < NUM_NT ; i++){
		g.head[i].numRules = 0;
		g.head[i].isEps =0;
	}
	while(fscanf(ptr,"%s %[^\n]",rhs,line) != EOF){
		Term new = stringToEnum(rhs,genericMappingTable);
		lhs* temp = &(g.head[new]);
		temp->value = new;
		int n =  temp->numRules ;
		node * rule_head = temp->rhs[n];
		char* currentToken = strtok(line, ",");
		node * prev ;
		if(currentToken != NULL){
			temp->rhs[n]  = (node *)malloc(sizeof(node));
			node * next_2 = temp->rhs[n];
			next_2->symbol = stringToEnum(currentToken, genericMappingTable);
			if(next_2->symbol >= NUM_NT)
				next_2->isTerminal = 1;
			if(next_2->symbol == e)
				temp->isEps = 1;
			prev = next_2;
		}
		while((currentToken = strtok(NULL,","))!=NULL){
			Term next  = stringToEnum(currentToken, genericMappingTable);
			node * next_1  = (node *)malloc(sizeof(node));
			next_1->symbol = next;
			if(next >= NUM_NT)
				next_1->isTerminal = 1;
			else next_1->isTerminal =0;
			prev->next = next_1;
			prev = next_1;
		}
		node * hare = g.head[temp->value].rhs[temp->numRules];
		while(hare!= NULL){
			hare = hare->next;
		}
		prev->next = NULL ;
		temp->numRules ++;
	}
	g.first= (long long *)malloc(sizeof(long long)*NUM_NT);
	g.follow= (long long *)malloc(sizeof(long long)*NUM_NT);
	return g;
}

void computeFirstAndFollowSets(grammar *G){
	int visited[NUM_NT];
	int i;
	for( i=0;i < NUM_NT;i++)
	{
		visited[i] = 0 ;
	}
	lhs tmp;
	for( i=0;i < NUM_NT;i++)
	{
		if(visited[i] == 0)
		{
			tmp = G->head[i];
			G->first[i] = calcFirst(G,i,visited);
		}
	}
	for(int i=0;i < NUM_NT;i++)
	{
		visited[i] = 0 ;
	}
	for(int i= 0 ;i < NUM_NT ;i++)
	{	
		if(i == 0){
			G->follow[i] |= (long long)1<<DOLLAR-NUM_NT;
		}
		if(visited[i] == 0)
		{
			calcFollow(G,i,visited);
		}
		long long tempo =~((long long)1 << (e-NUM_NT));
		G->follow[i] &= tempo ;
	}
}

long long calcFirst(grammar * G, int NT_index, int *visited){
	long long ans=0, bitset=0,iseps=0;
	lhs tmp;
	tmp=G->head[NT_index];
	visited[NT_index]=1;
	int i;
	for( i=0; i<	tmp.numRules; i++){
		node * hare =  tmp.rhs[i];
		if(hare != NULL && hare->isTerminal==1){
			bitset = (long long)1 << (hare->symbol - NUM_NT);
			ans |= bitset;
			if(ans&1)
				iseps=1;
		}
		else {
			if(visited[hare->symbol]==1){
				ans |= (long long )G->first[hare->symbol];			}
			else {
				long long tempans = calcFirst(G, hare->symbol, visited);
				G->first[hare->symbol] = tempans;
				ans |= (long long )G->first[hare->symbol];
			}
			int termflag=0;
			while(G->head[hare->symbol].isEps == 1 && hare->next!=NULL)
			{
			    hare = hare->next;termflag=0;
				if(hare->isTerminal==1){
					bitset = (long long)1 << hare->symbol - NUM_NT;
					ans |= (long long )bitset;
					termflag=1;
					break;
				}
				if(visited[hare->symbol]==1){
					ans |= (long long )G->first[hare->symbol];
				}
				else {
					long long tempans = calcFirst(G, hare->symbol, visited);
					G->first[hare->symbol] = (long long )tempans;
					ans |= (long long )G->first[hare->symbol];
				}
			}
			if((termflag==0) && (G->head[hare->symbol].isEps == 1 && hare->next==NULL))
			{
				iseps=1;
			}
		}
	}
	if(iseps==1)
		ans|=e-NUM_NT;
	else
		{
			ans &= ~((long long)1);
		}
	return ans;
}

void calcFollow(grammar *G, int NT_index, int visited[]){
	lhs tempLhs;
	node *tempRule;
	visited[NT_index]=1;
	for(int i=0; i<NUM_NT; i++){
		tempLhs = G->head[i];
		for(int j=0; j<tempLhs.numRules; j++){
			tempRule = tempLhs.rhs[j];
			while(tempRule!=NULL){
				while(tempRule && tempRule->symbol != NT_index){
					tempRule = tempRule -> next;
				}
				if(!tempRule)
					continue;
				if(tempRule -> next && (tempRule->next)->isTerminal){
					G->follow[NT_index] |= (long long)1 << (tempRule->next)->symbol-NUM_NT;
					tempRule = tempRule -> next;	
				}
				else if(tempRule -> next && (tempRule->next)->isTerminal==0){
					G->follow[NT_index] |= G->first[(tempRule->next)->symbol];
					int terminalFlag=0;
					tempRule = tempRule -> next;
					while(G->head[(tempRule)->symbol].isEps==1 && (tempRule)->next!=NULL)
					{
						tempRule=tempRule->next;
						if((tempRule)->isTerminal){
							G->follow[NT_index] |= (long long)1 << (tempRule)->symbol-NUM_NT;
							terminalFlag=1;
							break;
						}
						G->follow[NT_index] |= G->first[(tempRule)->symbol];
					}
					if(terminalFlag == 0 && (G->head[(tempRule)->symbol].isEps==1 && (tempRule)->next==NULL))
					{
						if(visited[tempLhs.value]==1)
						{
							G->follow[NT_index] |= G->follow[tempLhs.value];
						}
						else
						{
							calcFollow(G,tempLhs.value,visited);
							G->follow[NT_index] |= G->follow[tempLhs.value];
						}
					}
				}
				else if(tempRule->next ==  NULL){
						if(visited[tempLhs.value]==1)
						{
							G->follow[NT_index] |= G->follow[tempLhs.value];
						}
						else
						{
							calcFollow(G,tempLhs.value,visited);
							G->follow[NT_index] |= G->follow[tempLhs.value];
						}
						tempRule = tempRule -> next;
				}
			}
		}
	}
	return;
}

long long calcFirstRule(grammar * G, int NT_index, int numRule){
	lhs ruleLhs;
	ruleLhs = G->head[NT_index];
	node *hare = ruleLhs.rhs[numRule];
	long long int ans = 0, bitset=0;	
	int iseps =0;
	int i=numRule;
		if(hare != NULL && hare->isTerminal==1){
			bitset = (long long)1 << (hare->symbol - NUM_NT);
			ans |= bitset;
			if(ans&1)
				iseps=1;
		}
		else {
			
			ans |= (long long )G->first[hare->symbol];
			int termflag=0;
			while(G->head[hare->symbol].isEps == 1 && hare->next!=NULL)
			{	
			    hare = hare->next; termflag=0;
				if(hare->isTerminal==1){
					bitset = (long long)1 << hare->symbol - NUM_NT;
					ans |= (long long )bitset;
					termflag=1;
					break;
				}
				ans |= (long long )G->first[hare->symbol];
			}
			if((termflag==0) && (G->head[hare->symbol].isEps == 1 && hare->next==NULL))
			{
				iseps=1;
			}
	}
	if(iseps==1)
			ans|=e-NUM_NT;
	else
		{
			ans &= ~((long long)1);
		}
	return ans;
}

void printFirst(grammar * G){
	int i,j;
	long long tmp;
	for(i=0;i<NUM_NT;i++)
	{
			printf("%s   ->  {",enumToString(i));
			tmp = G->first[i];
			for(j=0;j<DOLLAR-NUM_NT;j++)
			{
				if(tmp%2==1)
				{
					printf("%s, ",enumToString(j+NUM_NT));
				}
				tmp /= 2;
			}
			printf(" } \n");
	}
}

void printFirstRule(grammar * G, int num){
	int i,j;
	long long tmp;
	for(i=num;i<num+1;i++)
	{
			printf("{") ;
			tmp = G->first[i];
			for(j=0;j<DOLLAR-NUM_NT;j++)
			{
				if(tmp%2==1)
				{	
					if((j + NUM_NT) == e)
					continue;
					printf("%s, ",enumToString(j+NUM_NT));
				}
				tmp /= 2;
			}
			printf("} \n");
	}
}

void printFollow(grammar * G){
	int i,j;
	long long tmp;
	for(i=0;i<NUM_NT;i++)
	{
			printf("%s   ->  {",enumToString(i));
			tmp=G->follow[i];
			for(j=0;j<=DOLLAR-NUM_NT;j++)
			{
				if(tmp%2==1)
				{
					printf("%s, ",enumToString(j+NUM_NT));
				}
				tmp /= 2;
			}
			printf(" } \n");
	}
}

void createParseTable(grammar *G, parseTableEntry T[][DOLLAR-NUM_NT]){
	lhs tempLhs;
	node *tempRule;
	long long tempfirst, tempfollow;
	parseTableEntry entry;
	for(int i=0; i<NUM_NT; i++){	
		entry.whichNT = i;
		tempLhs = G->head[i];
		for(int j=0; j<tempLhs.numRules; j++){
			long long int eps=0;
			tempRule = tempLhs.rhs[j];
			entry.ruleIndex = j;
			if(tempRule->isTerminal){
				if(tempRule->symbol!=e)
					T[i][tempRule->symbol-NUM_NT-1] = entry;
				else
					{
                        eps=1;
                    }
			}
			else{
				tempfirst =0;
				tempfirst = calcFirstRule(G,i,j);
				eps = tempfirst & 1;
				tempfirst /= 2;
				for (int k=0 ; k<num_T ; k++){
					if(tempfirst & 1 == 1){
						T[i][k]=entry;
						}
					tempfirst /=2;
				}
			}
			if (eps == 1){
				tempfollow = G->follow[i];
				if((tempfollow >> (num_T)) & 1){
					T[i][num_T-1]=entry;
				}
				tempfollow = G->follow[i];
				for (int k=0 ; k<num_T ; k++){
					if(tempfollow & 1 == 1){
						T[i][k-1]=entry;
					}
					tempfollow = tempfollow >> 1;
				 }
			}
		}
	}

	long long int tmp=0;
	for(int i=0;i<NUM_NT;i++)	//populating SYN sets in parse table
	{
		for(int j=0;j<DOLLAR-NUM_NT;j++)
		{
			tmp = (long long)1 << (j+1);
			if((G->follow[i] & tmp) > 0)
			{
				if(T[i][j].whichNT==ERROR)
				{
					T[i][j].whichNT=SYN;
					T[i][j].ruleIndex=-2;
				}
			}
		}
	}
}

void printStack(stackNode *head)	//additional function to print stack contents
{
	stackNode *tmp=head;
	printf("++++++++++++++++++\n\nStack IS :\n");
	int i=1;
	while(tmp!=NULL)
	{
		if(tmp->position->isTerminal==1)
        printf("%d   :  %d   : %s\n",i,1,enumToString(tmp->position->unode.ln.t.term));
        else
        {
			printf("%d   :  %d   : %s\n",i,0,enumToString(tmp->position->unode.nt.value));
		}
		i++;
		tmp=tmp->next;
	}
	printf("\n++++++++++++++++++++\n");
}


void parseInputFile(FILE * fptr, parseTableEntry Table[][DOLLAR-NUM_NT],grammar g, char *output, int printErr){
    stackNode *head  = (stackNode *)malloc(sizeof(stackNode));
    head = initialize_stack();
    int finalPrint = 0;
    stackNode *n =  (stackNode *)malloc(sizeof(stackNode));
    (n->position) = (treeNode *)malloc(sizeof(treeNode));
    n->position->isTerminal = 0;
    n->position->unode.nt.value=program;    
    n->position->unode.nt.child=NULL;
    n->position->unode.nt.sibling=NULL;
    treeHead  = n->position;
    head = push(n,head);
    token * newToken;
    
    //node topOfStack;
    int tokenTake=1,k=0,errorFlag=0;
    int eRule=0;
    int printError = 0;
    while(1)
    {
        if(tokenTake==1)
        {
        	newToken =  getNextToken(printErr);
    	}
        tokenTake=0;
        if(eRule){
        	if(head->position->isTerminal==1)
        	{
            	if(head->position->unode.ln.t.term==DOLLAR)	//end of the stack reached
	        	{	
	        		
	        		if(finalPrint == 0){
		        		if(printErr == 1)
		        			printf("\nPARSING SUCCESSFUL : Input source code is syntactically correct\n");
		        	}	
		        	else{

		        		if(printErr == 1)
		        			printf("\nPARSING SUCCESSFUL : Performed Error Recovery successfully\n");
		        	}
		        	break;
	        	}
		        else{
		        	printf("Output ended with terminal at top of stack!\n");
		        	break;
		        }
            }
            else
            {	if(printErr == 1)
            		printf("\nPARSING SUCCESSFUL : Performed Error Recovery successfully\n");
            	while(head!=NULL)
            		head=pop(head);
            	break;
            }
        }
  		if(newToken==NULL){
        	newToken = (token*)malloc(sizeof(token));
        	newToken->term = DOLLAR ;
        	eRule=1;
    	}
        if(head->position->isTerminal==1)
        {	
			if((newToken->term == head->position->unode.ln.t.term) && (head->position->unode.ln.t.term != DOLLAR)){
				head->position->unode.ln.t = *newToken ;
		        head  = pop(head);
		        tokenTake=1;
		        errorFlag=0;
		        continue;
		    }
		    else {
		    	if(errorFlag == 0){
		    		if(printErr == 1)
		    			printf("Line %d: SYNTACTIC Error in the input as expected token is %s\n",newToken->lineno,enumToString(head->position->unode.ln.t.term)); 
		    		finalPrint = 1;
		    		hasSyntacticError = 1;
		    	}
		    	while(head!=NULL && head->position->isTerminal==1)
            	{
            		head=pop(head);
            	}
		    	errorFlag=1;
		    }
		}
        else{	
            if(Table[head->position->unode.nt.value][newToken->term -NUM_NT -1].ruleIndex >=0){
                errorFlag=0;
                Term whichNT = Table[head->position->unode.nt.value][newToken->term -NUM_NT -1].whichNT;
                int ruleIndex = Table[head->position->unode.nt.value][newToken->term -NUM_NT -1].ruleIndex ;
                treeNode * Tr = head->position;
                head = pop(head);
                stackNode *head1 = (stackNode *)malloc(sizeof(stackNode)),*back;   
                (head1->position) = (treeNode *)malloc(sizeof(treeNode));             
                treeNode *Trtmp = (treeNode *)malloc(sizeof(treeNode));
                node * temp =  g.head[whichNT].rhs[ruleIndex];
                if(temp->symbol==e)	
                {
                	Trtmp->isTerminal = 1;
					Trtmp->unode.ln.t.term=e;
					Trtmp->unode.ln.t.lineno=-1;
					Trtmp->unode.ln.t.isKeyword=0;
					Trtmp->unode.ln.child=NULL;
					Trtmp->unode.ln.sibling=NULL;
					Trtmp->unode.ln.parent=NULL;
					if(Tr->isTerminal==1)
					{
						(Tr->unode.ln).child=Trtmp;
						Trtmp->unode.ln.parent=Tr;
					}
					else
					{
						(Tr->unode.nt).child=Trtmp;	
						Trtmp->unode.ln.parent=Tr;
					}
                	continue;
                }
                if(temp->isTerminal==1)
                {

					Trtmp->isTerminal = 1;
					Trtmp->unode.ln.t.term=temp->symbol;
					Trtmp->unode.ln.t.lineno=newToken->lineno;
					Trtmp->unode.ln.t.lexeme=newToken->lexeme;
					Trtmp->unode.ln.t.value=newToken->value;
					Trtmp->unode.ln.t.isKeyword=0;
					Trtmp->unode.ln.child=NULL;
					Trtmp->unode.ln.sibling=NULL;
					Trtmp->unode.ln.parent=NULL;
					head1->position = Trtmp;
					if(Tr->isTerminal==1)
					{
						(Tr->unode.ln).child=Trtmp;
						Trtmp->unode.ln.parent=Tr;
					}
					else
					{
						(Tr->unode.nt).child=Trtmp;
						Trtmp->unode.ln.parent=Tr;	
					}
                }
                else{
                	Trtmp->isTerminal = 0;
					Trtmp->unode.nt.value=temp->symbol;    
					Trtmp->unode.nt.child=NULL;
					Trtmp->unode.nt.sibling=NULL;
					head1->position = Trtmp;
					if(Tr->isTerminal==1)
					{
						(Tr->unode.ln).child=Trtmp;
						Trtmp->unode.nt.parent=Tr;
					}
					else
					{
						(Tr->unode.nt).child=Trtmp;	
						Trtmp->unode.nt.parent=Tr;
					}
                }
                head1->next = NULL;;
                back=head1;
                temp=temp->next;
                while(temp != NULL){
                    stackNode *stmp = (stackNode *)malloc(sizeof(stackNode));             
                    (stmp->position) = (treeNode *)malloc(sizeof(treeNode));
                    treeNode *tmp1 = (treeNode *)malloc(sizeof(treeNode));
                    if(temp->isTerminal==1)
		            {
						tmp1->isTerminal = 1;
						tmp1->unode.ln.t.term=temp->symbol;
						tmp1->unode.ln.t.isKeyword=newToken->isKeyword;
						tmp1->unode.ln.child=NULL;
						tmp1->unode.ln.sibling=NULL;
						tmp1->unode.ln.parent=NULL;
						stmp->position = tmp1;
						if(Trtmp->isTerminal==1)
						{
							(Trtmp->unode.ln).sibling=tmp1;
							tmp1->unode.ln.parent=(Trtmp->unode.ln).parent;
							Trtmp=(Trtmp->unode.ln).sibling;
						}
						else
						{
							(Trtmp->unode.nt).sibling=tmp1;
							tmp1->unode.ln.parent=(Trtmp->unode.nt).parent;
							Trtmp=(Trtmp->unode.nt).sibling;
						}
		            }
		            else{
		            	tmp1->isTerminal = 0;
						tmp1->unode.nt.value=temp->symbol;
						tmp1->unode.nt.child=NULL;
						tmp1->unode.nt.sibling=NULL;
						tmp1->unode.nt.parent=NULL;
						stmp->position = tmp1;
						if(Trtmp->isTerminal==1)
						{
							(Trtmp->unode.ln).sibling=tmp1;
							tmp1->unode.nt.parent=(Trtmp->unode.ln).parent;
							Trtmp=(Trtmp->unode.ln).sibling;
						}
						else
						{
							(Trtmp->unode.nt).sibling=tmp1;
							tmp1->unode.nt.parent=(Trtmp->unode.nt).parent;
							Trtmp=(Trtmp->unode.nt).sibling;
						}
		            }
					head1->next=stmp;
					head1=stmp;
                    temp = temp->next;
                    
                }
                head1->next=head;
                head=back;
            }
            else if(Table[head->position->unode.nt.value][newToken->term -NUM_NT -1].ruleIndex ==-2){
            	//treeNode *tmp= head->position;
            	if(errorFlag==0)
            	{
            		//errorFlag=1;
            		if(printErr == 1){
            			printf("Line %d: SYNTACTIC Error in the input as expected token is :",newToken->lineno);
            			printFirstRule(&g,head->position->unode.nt.value); 
            		}
            		finalPrint = 1;
            		hasSyntacticError = 1;
            	}
            	head = pop(head);
            	// treeNode *tmp1=(treeNode *)malloc(sizeof(treeNode));
            	// tmp1->isTerminal=1;
            	// tmp1->unode.ln.t.term=ERROR;
            	// tmp1->unode.ln.t.isKeyword=0;
				// tmp1->unode.ln.child=NULL;
				// tmp1->unode.ln.sibling=NULL;
				// tmp1->unode.ln.parent=tmp;
				// tmp->unode.nt.child=tmp1;

            }

            else{ 

            if(errorFlag == 0){
            		if(printErr == 1){
			    		printf("Line %d: SYNTACTIC Error in the input as expected token is ",newToken->lineno); 
			    		printFirstRule(&g,head->position->unode.nt.value); 
			    	}
		    		finalPrint = 1;
		    		hasSyntacticError =1;
		    	}
	    	errorFlag=1;
	    	tokenTake=1;
    		}
    
	    }
    }
    // printf("%-28s%-10s%-16s%-13s%-28s%-11s%-25s\n\n\n","LEXEME","LINENO","TOKENNAME","VALUELF","PARENTNODE","ISLEAFNODE","NODE");
  	
}


void printGrammar(grammar g){
	for(int i = 0 ; i < NUM_NT ; i++){
		node * temp ;
		for(int j = 0 ; j < g.head[i].numRules ; j++){
			temp  = g.head[i].rhs[j];
			while(temp != NULL){
			printf("%s->",enumToString(temp->symbol));
			temp = temp->next;
			}
			printf("\n");	
		}
	}
}

void initializeParser(char * testFile, char *parseTreeOut, int printFlag, int printTree, int printErr){
    eof =0 ;
    initializeLexerForParser(testFile);
	FILE * ptr = fopen("grammar.txt","r");
	grammar g = readGrammar(ptr);
	computeFirstAndFollowSets(&g);
	num_T=numOfTerminals();
	parseTableEntry parseTable[NUM_NT][num_T];
	for(int i=0;i<NUM_NT;i++){
		for(int j=0; j<num_T; j++){
			parseTable[i][j].whichNT= ERROR;
			parseTable[i][j].ruleIndex= -1;
		}
	}
	count2 = 0;
	createParseTable(&g,parseTable);
    parseInputFile(ptr, parseTable ,g, parseTreeOut, printErr);
    if(printTree == 0)
    	inorder(treeHead , ptr, printTree);

    if(printFlag == 1){
	    ptMemUsage = count2* sizeof(treeNode);
	    printf("\n\n");
	    printf("No. of nodes in the Parse Tree: %-10d Allocated Memory: ",count2);
	    printf("%d bytes\n",ptMemUsage);
	}

	if(printTree ==1){
    	printf("%-28s%-10s%-16s%-13s%-28s%-11s%-25s\n\n\n","LEXEME","LINENO","TOKENNAME","VALUELF","PARENTNODE","ISLEAFNODE","NODE");
  		inorder(treeHead , ptr, printTree);
  	}

    //printf("Parsing Done\n");
}

treeNode * returnRoot(char *parseTreeOut){
	return treeHead;
}
