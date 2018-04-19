///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>

typedef struct _rnode{
	struct _rnode * next;
	int symb_no;
} rnode ;

typedef struct _rules{
	int lhs;
	rnode rhs; 
}rules;

typedef struct _tnode{
	struct _tnode *child;
	struct _tnode *sibling;
	struct _tnode *parent;
	int symb_no;
	char * lexeme;
	int start,end;
	int line_num;
	int ch_num;
} tnode;
