///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////

#include "hash-helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _sym_node sym_node;

struct _sym_node{   
    htable_sym * htlink;
    sym_node *parent;
    sym_node *child;
    sym_node *sibling;
    char * scope_name ;
};

sym_node* create_sym_node();
extern sym_node * sroot;
void make_sym_table(tnode *root);
void var_init();
extern sym_node *scur;