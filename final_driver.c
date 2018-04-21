///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////

// #include "lexer.h"
// #include "parser.h"
#include "symboltable.h"
// #include "hash-helper.h"
// #include "astDefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void traverse_symbol_table(sym_node * temp){
    if(temp->child!=NULL){
        traverse_symbol_table(temp->child);
    }    
    if(temp->parent!=NULL){     
        printf("%s --- %s \n",temp->parent->scope_name,temp->scope_name);
    }
    else{
        printf("%s --- %s \n","No Parent",temp->scope_name);
    }
    if(temp->child!=NULL){
        sym_node *temp2;
        temp2 = temp->child->sibling;
        while(temp2!=NULL){
            traverse_symbol_table(temp2);
            temp2=temp2->sibling;
        }
    }
}
void print_init(){
    printf("0. exit from loop \n");
	printf("1. display only tokens from the file\n");
	printf("2. display parsing errors and lexical errors if any and print parse tree\n");
	printf("3. display ast nodes traversed in inorder traversal child root siblings\n");
	printf("4. display ast compression statistics \n");	
    printf("5. display symbol table\n");
    printf("6. display semantic errors \n");    
}

int main(int args, char*argvs[]){
	printf("LEVEL #2: I have implemented type checks and semantic checks in one pass for improved\
    efficieny of error checking  but not completely able to implemet both , symbol table offset\
    code generation could not be implemented \n");
	
    print_init();
	int opt ;
	fflush(stdin);
	
	scanf("%d",&opt);

	printf("%d\n",opt);
	while(opt!=0){
		if(opt==1){
			get_only_token(argvs[1]);
			printf("***************************\n");
		}
		else if(opt==2){
			get_syntactic_errors(argvs[1]);
			traverse_parse_tree_final(argvs[1]);
			printf("***************************\n");            
		}	
		else if(opt==3){
            ast_constructer(argvs[1]);
			printf("***************************\n");
		}
		else if(opt==4){
            printf("Please ensure you run option 2 and 3 before this every time to accumulate statistics \n");

            printf("Parse tree node count %d \n",parse_tree_count);
            printf("AST node count %d \n",ast_count);
            if( ast_count==0 || parse_tree_count==0 ){
                printf("either option 2 or option 3 or both not used before using this");
            }
            else{
                // printf("%d \n",diff);
                int diff = parse_tree_count- ast_count;
                double ratio = (double)(diff)/(double)(parse_tree_count);
                printf("%f\n",ratio);
            }

            printf("resetting respective counts \n");
            ast_count=0;
            parse_tree_count=0;
            printf("Re run 2 and 3 to get the counts again \n");
        }
        else if(opt==5){
            get_syntactic_errors(argvs[1]);
            ast_constructer_wo_print(argvs[1]);

            printf("semantic checking and symbol table along with type checking done parallely hence semantic errors will be seen here as well\n");
            var_init();
            make_sym_table(ptree);
            traverse_symbol_table(sroot);
        }
        else if(opt==6){
            get_syntactic_errors(argvs[1]);
            ast_constructer_wo_print(argvs[1]);

            if(suc_parse==1){
                var_init();
                make_sym_table(ptree);
                traverse_symbol_table(sroot);                
            }
        }
		else if(opt==0){
			printf("terminated\n");
			printf("***************************\n");
			break;
		}
        printf("\n \n");
        print_init();
		scanf("%d",&opt);
	}
}
