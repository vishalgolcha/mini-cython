///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////

#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_init(){
	printf("1. display uncommented file \n");
	printf("2. display only tokens from the file \n");
	printf("3. display parsing errors and lexical errors if any and parse \n");
	printf("4. get parse tree of file x in file y \n");	
	printf("5. done with the compiler for the day \n");

}

int main(int args, char*argvs[]){
	printf("(a) FIRST and FOLLOW set automated \n(c) Both lexical and syntax analysis modules implemented\n(-) All test cases working\n");
	print_init();
	int opt ;
	fflush(stdin);
	
	scanf("%d",&opt);

	printf("%d\n",opt);
	while(opt!=5){
		if(opt==1){
			uncommentedfile(argvs[1]);
			printf("***************************\n");
		}
		else if(opt==2){
			get_only_token(argvs[1]);
			printf("***************************\n");
		}	
		else if(opt==3){
			get_syntactic_errors(argvs[1]);
			printf("***************************\n");
		}
		else if(opt==4){
			traverse_parse_tree(argvs[1],argvs[2]);
			printf("***************************\n");
		}
		else if(opt==5){
			printf("terminated\n");
			printf("***************************\n");
			break;
		}
		printf("1. display uncommented file \n");
		printf("2. display only tokens from the file \n");
		printf("3. display parsing errors and lexical errors if any and parse \n");
		printf("4. get parse tree of file x in file y \n");	
		printf("5. done with the compiler for the day \n");

		scanf("%d",&opt);
	}
}
