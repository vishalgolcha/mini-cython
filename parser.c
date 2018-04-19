///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define num_sym 84 // no of terminals and non terminals currently 
// in the grammar will increasewhen you add the $ -> main rule

rules * rule_adt;
char *rule[100];
char *rule_split[100][100];
long long int first_sets[100][2];
int follow_checker[100];
long long int follow_sets[100];
int follow_matrix[100][100];
int parse_table[100][100];

char * symb_name[100] = {"eps","AND", "ASSIGNOP", "CL", "COMMA", "DIV", "ELSE", "END", "ENDIF", "EQ", "FUNCTION", "FUNID", "GE", "GT", "ID", "IF", "INT", 
"LE", "LT", "MAIN", "MATRIX", "MINUS", "MUL", "NE", "NOT", "NUM", "OP", "OR", "PLUS", "PRINT", "READ", "REAL", "RNUM", "SEMICOLON", "SIZE", "SQC", "SQO", 
"STR", "STRING",
"<ArithExpFactored>", "<ArithTermFactored>", "<MatLit>", "<RowLit>", "<arithmeticExpression>", "<arithmeticTerm>", "<assignmentStmt_type1>", 
"<assignmentStmt_type2>", "<booleanExpression>", "<constrainedVars>", "<declarationStmt>", "<factor>", "<funCallStmt>", "<functionDef>", "<ifStmt>", 
"<ifStmtFactored>", "<inputParameterList>", "<ioStmt>", "<leftHandSide_listVar>", "<leftHandSide_singleVar>", "<listVar>", "<logicalOp>", "<mainFunction>", 
"<matrix>", "<matrixElement>", "<more_ids>", "<operator_highPrecedence>", "<operator_lowPrecedence>", "<otherStmts>", "<parameter_list>", "<relationalOp>", 
"<remainingColElements>", "<remainingList>", "<rightHandSide_type1>", "<rightHandSide_type2>", "<row>", "<rows>", "<sizeExpression>", "<stmt>", "<stmtExt>", 
"<stmtOrFunctionDef>", "<stmtsAndFunctionDefs>", "<type>", "<var>", "<var_list>"};

FILE * infile;
int  * sz_rule;
int cnt;
int i;

void init_pt(){
	int i,j;
	for(i=0;i<100;i++){
		for(j=0;j<100;j++){
			parse_table[i][j]=-1;
		}
	}	
}
int find_sno(char *x){
	int i=0;
	// 70 here depends on the number of variables you have in symb_name which might change
	for(i=0;i<84;i++){
		// printf("printing symbname %s %s\n",symb_name[i],x );
		
		if(strcmp(symb_name[i],x)==0){
			// printf("matches\n");
			return i;
		}
	}
	printf("%s\n",x);
	printf("couldnt find sno\n");
	return 1;
}

void traverse(rnode x ){
	printf("%s \n",symb_name[x.symb_no]);
	rnode* y= x.next;
	while(y!=NULL){
		printf("%s \n",symb_name[y->symb_no]);
		y=y->next;
	}
}

long long int set(long long int x,long long int b){
	x |= 1LL<<b;
	return x;
}


long long int unset(long long int x,long long int b){
	x ^= 1LL<<b;
	return x;
}

void traverse_and_compute_first(rules x,int idx){
	// printf("%s \n",symb_name[x.symb_no]);
	// passing layer number in idx 

	int lft = x.lhs;

	rnode* y= &x.rhs;
	// printf("lhs: %d ",lft);
	while(y!=NULL){
		long long int z =y->symb_no;
		// printf(" %lld ",z);
		
		if(z<39){
			first_sets[lft][idx] |= set(first_sets[lft][idx],z);
		}
		else{
			first_sets[lft][idx] |=	first_sets[z][idx];
		}

		if(first_sets[z][idx]&1LL){
			y=y->next;	
		}
		else{
			break;
		}	
	}
	// printf("\n");
}

void copy_layer(){
	int i=0;
	for(i=0;i<100;i++){
		first_sets[i][0]=first_sets[i][1];
	}
}


int check_if_equal(){
	int i=0;
	for(;i<100;i++){
		if(first_sets[i][0]!=first_sets[i][1]){
			return 0;
		}
	}
	return 1;
}



void traverse_and_compute_follow(rules x){
	int lft = x.lhs;
	rnode* rule_iter = &x.rhs;
	
	while(rule_iter!=NULL){
		rnode* rule_ele =rule_iter->next;	
		int cur_sym =  rule_iter->symb_no;      // this is the current symbol whose follow is being computed 
		
		while(rule_ele!=NULL){
			int ele_rt = rule_ele->symb_no; // element to the right that is the symbol that follows
			if(first_sets[ele_rt][1]&1LL ==1LL){

				follow_sets[cur_sym] |= (first_sets[ele_rt][1]& ~1LL);
				rule_ele=rule_ele->next;
			
			}
			else{
				follow_sets[cur_sym] |= (first_sets[ele_rt][1]);
				break;	
			}
		}
		
		if(rule_ele==NULL){
			// follow set of this equals follow set of the lhs
			follow_sets[cur_sym]|=follow_sets[lft];
			follow_matrix[lft][cur_sym]=1;
			// if(follow_matrix[cur_sym][lft]==1){
			// 	// cut off this check if you need this
			// }
		}
		// update iter->next
		rule_iter =rule_iter->next;
	}
}

void traverse_and_populate_table(rules x, int rule_no){
	long long int temp=0LL;
	int lft = x.lhs;
	rnode* rule_iter = &x.rhs;
	int cur_sym =  rule_iter->symb_no;


	while(rule_iter!=NULL && first_sets[cur_sym][1]&1LL==1){	
 	// this is the current symbol whose follow is being computed 
		temp |= first_sets[cur_sym][1];
		rule_iter=rule_iter->next;
		if(rule_iter!=NULL)
			cur_sym=rule_iter->symb_no;

	}
	if(rule_iter==NULL){
		temp |= follow_sets[lft];
	}
	else{
		temp|=first_sets[cur_sym][1];
		temp =(temp & ~1LL);
	}
	int i;
	for(i=0;i<39;i++){
		if(temp & (1LL<<i) ){
			parse_table[lft][i]=rule_no;
		}
	}

}
void populate_rules(){

	infile=fopen("finalgm.txt","r");
	rule[0] = (char*)malloc(sizeof(char)*1000);
	
	i=0;
	
	while(fgets(rule[i],sizeof(char)*1000,infile)){
		// printf("%s\n",rule[i]);
		i++;
		rule[i] = (char*)malloc(sizeof(char)*1000);
	}
	
	i--;
	cnt=i;

	sz_rule =( int* )malloc(sizeof(int)*(cnt+1));  // error tha phle idhar 
	// printf("print  no of element %d \n",i);

	while(i>=0){
		char *token,*str;
		str = strdup(rule[i]);  // We own str's memory now.
		// printf("%d\n",i);
		int j=0;

		// printf("%s \n",str);
		
		rule_split[i][j] = (char*)malloc(sizeof(char)*100);
		while ((rule_split[i][j] = strsep(&str, " "))) {
			// printf("%s\n",rule_split[i][j]);
			char * temp = rule_split[i][j];
			
			// manipulate string to not get \n int the end of certain symbols
			if(temp[strlen(temp)-1]=='\n'){
				temp[strlen(temp)-1]='\0';
				
			}

			j++;
			rule_split[i][j] = (char*)malloc(sizeof(char)*100);
			
		}
		// printf("***done***\n");		
		sz_rule[i]=j;
		i--;
		// free(str);
	}	

	// ******************* debugging 

	// printf("size of lhs of rule 73 %d\n",sz_rule[73]);

	// printf("rule 73 %s\n",rule_split[73][0]);
	// printf("rule 73 %s\n",rule_split[73][1]);
	// printf("rule 73 %s\n",rule_split[73][2]);

	
	// ************************************

	//was i<=cnt

	rule_adt = (rules*)malloc(sizeof(rules)*(cnt+1)); // idhar bhi error tha same upar wala
	// printf("cnt print %d\n",cnt);

	for(i=0;i<=cnt;i++){
		// printf("%d \n",i);
		// printf("%d")
		rule_adt[i].lhs=find_sno(rule_split[i][0]);
		// rule_adt[i].rhs=(rnode)malloc(sizeof(rnode)); 
		// printf("%s \n",rule_split[i][2]);
		
		rule_adt[i].rhs.symb_no=find_sno(rule_split[i][2]);


		rule_adt[i].rhs.next=NULL;

		rnode *temp,*temp2 ;
		temp = &(rule_adt[i].rhs);
		
		// printf("size of %d rule %d \n",i,sz_rule[i]);
		int j;
		for(j=3;j<sz_rule[i];j++){
			temp2 =(rnode*)malloc(sizeof(rnode));
			temp2->symb_no=find_sno(rule_split[i][j]);
			// printf("%s %d\n",rule_split[i][j],temp->symb_no);
			temp2->next=NULL;
			temp->next=temp2;
			temp=temp2;
		}

		// printf("***done***");
	}
	// cnt gives number of rules 
	// printf("%s \n",symb_name[(rule_adt[0].rhs.next)->symb_no]);

/*	// checking for rule population 
	for(i=0;i<=cnt;i++){
		printf("Rule no : %d\n",i);
		traverse(rule_adt[i].rhs);
		printf("****************\n");
	}
	// traverse(rule_adt[73].rhs);
*/	
}

void populate_first_sets(){

	/*-----------------------compute first sets-------------------------------*/

	/* compute first set first iteration and store in first_set[1] alternate with modulo */
	for(i=0;i<=cnt;i++){
		traverse_and_compute_first(rule_adt[i],1);
	}
	int first_check = check_if_equal();
	int toggle =1;

	int iter=0;
	while(first_check==0){
		//copy current into idx=0
		copy_layer();

		// printf("iteration %d \n",iter);
		iter++;

		for(i=0;i<=cnt;i++){
			traverse_and_compute_first(rule_adt[i],1);	
		}
		
		// toggle = ~toggle;
		first_check=check_if_equal();
	}

	for(i=0;i<39;i++){
		first_sets[i][1]=set(first_sets[i][1],i);
	}

	// printf("done\n");
	/*
	checking first set computations 

	int debug_iter =0;
	printf("%s \n",symb_name[23]);
	
	printf("%lld\n",first_sets[find_sno("<matrix>")][1]);

	for(debug_iter=39;debug_iter<84;debug_iter++){
		printf("%s ",symb_name[debug_iter]);
		long long int temp_first =first_sets[debug_iter][1];
		long long int i ;
		for(i=0;i<39;i++){
			if(temp_first&(1LL<<i))
				printf("%s ",symb_name[i]);
		}		
		printf("\n");
	}
	*/

	/*	
		debug stmts
		long long int k = 0;
		k=set(k,36);
		printf("%lld\n",k);
	*/	

}
void populate_follow_sets(){

	/*-----------------------compute follow sets-------------------------------*/

	/* should be done in one pass*/
	for(i=0;i<=cnt;i++){
		traverse_and_compute_follow(rule_adt[i]);	
	}
	int a=0,b=0;

	for(;a<100;a++){
		for(;b<100;b++){
			if(follow_matrix[a][b]==1 && follow_matrix[b][a]==1){
				follow_sets[a] |= follow_sets[b];
				follow_sets[b] |= follow_sets[a];
			}
		}
	}

	for(a=0;a<100;a++){
		for(b=0;b<100;b++){
			if(follow_matrix[a][b]==1 && follow_matrix[b][a]==1){
				follow_sets[a] |= follow_sets[b];
				follow_sets[b] |= follow_sets[a];
				continue;
			}
			else if(follow_matrix[a][b]==1){
				follow_sets[b]|=follow_sets[a];
			}
		}
	}
	
	// checking follow set computations 
	
	// int debug_iter =0;

	// for(debug_iter=39;debug_iter<84;debug_iter++){
	// 	printf("%s ",symb_name[debug_iter]);
	// 	long long int temp_follow =follow_sets[debug_iter];
	// 	long long int i ;
	// 	for(i=0;i<39;i++){
	// 		if(temp_follow&(1LL<<i))
	// 			printf("%s ",symb_name[i]);
	// 	}		
	// 	printf("\n");
	// }
	// printf("*******************************************\n");

}
void populate_parse_table(){
	
	// populate parse table now
	for(i=0;i<=cnt;i++){
		traverse_and_populate_table(rule_adt[i],i);	
	}
	 // Debuggers for parse_table
/*	
	for(i=80;i<81;i++){
		int j;
		printf("%s \n",symb_name[i]);
		for(j=0;j<39;j++){
			printf("%d ",parse_table[i][j]);
		}	
		printf("\n");
	}	
*/	
}
void init_grammar(){
	init_pt();
	populate_rules();
	populate_first_sets();
	populate_follow_sets();
	populate_parse_table();
}
// int main(){
// 	// check for some global declarations that need to be done when this is 
// 	// converted to a function

// }
