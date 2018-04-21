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

hnode *hnode_create_node(tnode *t,char *typ,llhnode *a,llhnode *b){
    	
	hnode* hvart = (hnode *) malloc(sizeof(hnode));
	// printf("debugger2\n");
	hvart->next=NULL;

	hvart->lexeme = (char *)malloc(sizeof(char)*30);
	hvart->var_type = (char *)malloc(sizeof(char)*30);

	hvart->symb_no = t->symb_no;

    strcpy(hvart->lexeme,t->lexeme);
    strcpy(hvart->var_type,typ);
    hvart->line_num = t->line_num;
    hvart->row_num = -1;
	hvart->col_num = -1;
    hvart->ele = t;
	if(strcmp(symb_name[hvart->symb_no],"FUNID")==0){
        hvart->is_func=1;
        hvart->inpt=a;
        hvart->outt=b;
        hvart->inp_num=a->cnt;
        hvart->out_num=b->cnt;
    }
    else{
        hvart->is_func=0;
        hvart->inpt=NULL;
        hvart->outt=NULL;
        hvart->inp_num=0;
        hvart->out_num=0;        
    }
    
    // use strcpy later for this
    return hvart;
}

// could do without returns here hmm
llhnode* hnode_attach(llhnode* x,tnode *t,char *typ,llhnode *a,llhnode *b){
	// printf("debugger\n");
	hnode* y =hnode_create_node(t,typ,a,b);

	if(x->cnt==0){
		x->head=y;		
		x->tail=y;
	}
	else{
		x->tail->next = y;
		x->tail = x->tail->next;
		x->tail->next=NULL;
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
	// printf("terminate\n");
}

/******************************************************/

htable_sym * insert_sym_htable(htable_sym  *z ,tnode* y/*this gets the funid node */ \
            ,char *typ,llhnode * inpt ,llhnode * outt/* link lists of strings to store type info
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