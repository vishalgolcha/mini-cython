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

ast_node * makenode();
ast_node * create_tree();
