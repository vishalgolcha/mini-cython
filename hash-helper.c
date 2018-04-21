#include "hash-helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

htable_sym * create_hashtable_symtable(int array_size){
	htable_sym * x = (htable_sym *)malloc(sizeof(htable_sym));
	x->hlist = (llhnode*)malloc(sizeof(llhnode)*array_size);
	x->lcnt = array_size;
	return x;
}

/******************************************************/

hnode *hnode_create_node(tnode *t,char *typ,llst_str *a,llst_str *b){
    	
	hnode* x = (hnode *) malloc(sizeof(hnode));
	x->var_type = (char *)malloc(sizeof(char)*50);
	x->next=NULL;
	x->lexeme = (char *)malloc(sizeof(char)*50);
	x->symb_no = t->symb_no;
    strcpy(x->lexeme,t->lexeme);
    strcpy(x->var_type,typ);
    x->line_num = t->line_num;
    x->row_num = -1;
	x->col_num = -1;
    if(strcmp(symb_name[x->symb_no],"FUNID")==0){
        x->is_func=1;
        x->inpt=a;
        x->outt=b;
        x->inp_num=a->cnt;
        x->out_num=b->cnt;
    }
    else{
        x->is_func=0;
        x->inpt=NULL;
        x->outt=NULL;
        x->inp_num=0;
        x->out_num=0;        
    }
    
    // use strcpy later for this
    return x;
}

// could do without returns here hmm
llhnode* hnode_attach(llhnode* x,tnode *t,char *typ,llst_str *a,llst_str *b){
	hnode* y =hnode_create_node(t,typ,a,b);
	if(x->cnt==0){
		x->head=y;		
		x->tail=y;
	}
	else{
		x->tail->next = y;
		x->tail = x->tail->next;
	}
	x->cnt++;
	return x ;
}

node* hnode_llst_top(llsti x){
	if(x.cnt==0){
		return NULL;
	}
	else{
		return x.head;
	}
}

void hnode_llst_traverse(llhnode * x){
	hnode *y = x->head;
	while(y!=NULL){
		printf("%s\n",y->lexeme);	
		y=y->next;
	}
}

/******************************************************/

htable_sym * insert_sym_htable(htable_sym  *z ,tnode* y/*this gets the funid node */ \
            ,char *typ,llst_str * inpt ,llst_str * outt/* link lists of strings to store type info
            of func defined */ ){
	
    unsigned long h = hash(symb_name[y->symb_no]);
	int x = h%(z->lcnt);
	// printf("hash value %d\n",x );

    z->hlist[x] =  *hnode_attach(&(z->hlist[x]),y,typ,inpt,outt);
	return z;
}

char* find_sym_htable(htable_sym* z,tnode *y){
	unsigned long h = hash(symb_name[y->symb_no]);
	int x = h%(z->lcnt);
	// printf("hash value %d\n",x );
	hnode *t = (z->hlist[x]).head;
    while(t!=NULL){
		// printf("%d\n",y->val);
		if(strcmp(t->lexeme,y->lexeme)==0 \
                  && t->symb_no ==y->symb_no){ 
			return t->var_type;  // retrieve this and index-1 on calls
		}	
		t=t->next;
	}	
	return NULL;
}


// sets the value of rows and columns for a variable in hashtable
int modify_hash_table_rc(htable_sym* z,tnode* y,pair *k){
	unsigned long h = hash(symb_name[y->symb_no]);
	int x = h%(z->lcnt);
	// printf("hash value %d\n",x );
	hnode *t = (z->hlist[x]).head;
    while(t!=NULL){
		// printf("%d\n",y->val);
		if(strcmp(t->lexeme,y->lexeme)==0 \
                  && t->symb_no ==y->symb_no){ 
			t->row_num=k->r;
			t->col_num=k->c;
			return 1;
		}	
		t=t->next;
	}	
	return 0;
	
}

pair* retrieve_hash_table_rc(htable_sym* z,tnode* y){
	unsigned long h = hash(symb_name[y->symb_no]);
	int x = h%(z->lcnt);
	// printf("hash value %d\n",x );
	hnode *t = (z->hlist[x]).head;
    while(t!=NULL){
		// printf("%d\n",y->val);
		if(strcmp(t->lexeme,y->lexeme)==0 \
                  && t->symb_no ==y->symb_no){ 
			pair *k  = (pair *)malloc(sizeof(pair));
			
			k->c = t->col_num;
			k->r = t->row_num;
			return k;
		}	
		t=t->next;
	}	
	return NULL;
	
}