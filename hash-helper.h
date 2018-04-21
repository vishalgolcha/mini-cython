///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astDefs.h"

typedef struct _hnode hnode;
typedef struct _llhnode llhnode;

struct _hnode{
    char *var_type;
    int is_func;
    llhnode *inpt,*outt;
    int row_num ,col_num;
	int symb_no;
	char *lexeme;
	int line_num;
    int inp_num;
    int out_num;
    hnode *next;
    tnode *ele ; // just using this for fun paras
    //  input output 
};

struct _llhnode{
    hnode * head;
    hnode * tail;
    int cnt; 
};


typedef struct _htable_sym{
    llhnode * hlist;
    int lcnt;
}htable_sym;

typedef struct _pair{
    int r,c;
}pair;



htable_sym * create_hashtable_symtable(int array_size);

/******************************************************/

hnode *hnode_create_node(tnode *t,char *typ,llhnode *a,llhnode *b);

// could do without returns here hmm
llhnode* hnode_attach(llhnode* x,tnode *t,char *typ, llhnode *a,llhnode *b);

node* hnode_llst_top(llsti x);

void hnode_llst_traverse(llhnode * x);

/******************************************************/

htable_sym * insert_sym_htable(htable_sym  *z ,tnode* y/*this gets the funid node */ \
            ,char *typ,llhnode * inpt ,llhnode * outt/* link lists of strings to store type info
            of func defined */ );

char* find_sym_htable(htable_sym* z,tnode *y);
pair* retrieve_hash_table_rc(htable_sym* z,tnode* y);
int modify_hash_table_rc(htable_sym* z,tnode* y,pair *k);



