///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
htable * create_hashtable(int array_size);
unsigned long hash(char *str);

node * create_node(int y);
str_node *str_create_node();
llsti attach(llsti x,int val);

llsti rev_attach(llsti x,int val);
llst_str attach_str(llst_str x,char *z,int n);

void llst_traverse(llsti x);
llsti llst_pop(llsti x);
node* llst_top(llsti x);

void llst_traverse_str(llst_str x);

htable * insert(htable *z , char * y,int id);

int hfind(htable* z,char *y);
token* get_next_token();
// extern htable * keyword_hash ;

