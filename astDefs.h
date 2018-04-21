///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
typedef struct _ast_node ast_node;

struct _ast_node{
    ast_node * child;
    ast_node * sibling;
    // link to sym table ??
    int symb_no;
	char * lexeme;
	int line_num;
};

ast_node* makenode(tnode * x);
ast_node* make_ast(tnode *x);
int rem_redundant(int num);
int get_symb_no(char * x);
tnode * tree_pruner(tnode* x,int par);
void calculate_children(tnode* trv_node);
void ast_constructer(char *inp);
void inorder_final_ast(tnode* trv_node);
void ast_constructer_wo_print(char *inp);

extern int ast_count;