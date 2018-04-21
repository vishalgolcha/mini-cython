///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

char * key_words[100] = {
"end","int","real","string","matrix","if","else","endif",
"read","print","function","_main",".and.",".or.",".not."
};



htable * create_hashtable(int array_size){
	htable * x = (htable *)malloc(sizeof(htable));
	x->llarray = (llst_str*)malloc(sizeof(llst_str)*array_size);
	x->cnt = array_size;
	return x;
}

unsigned long hash(char *str){

    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

// unsigned long hash2(char *x){

// }


node * create_node(int y){
	node* x = (node *)malloc(sizeof(node));
	x->val = y;
	x->next= NULL;
	return x;
}

/* takes a character  pointer as parameter */
str_node *str_create_node(){	
	str_node* x = (str_node *) malloc(sizeof(node));
	x->str_val = (char *)malloc(sizeof(char)*50);
	x->next=NULL;
	return x;
}

llsti attach(llsti x,int val){
	node* y =create_node(val);

	if(x.cnt==0){
		x.head=y;		
		x.tail=y;
	}
	else{
		x.tail->next = y;
		x.tail = x.tail->next;
	}
	x.cnt++;
	return x ;
}

llsti rev_attach(llsti x,int val){
	node* y =create_node(val);

	if(x.cnt==0){
		x.head=y;		
		x.tail=y;
	}
	else{
		y->next = x.head;
		x.head=y;	
	}
	x.cnt++;
	return x ;
}
// check for a need of a function to initialize the stack
llsti llst_pop(llsti x){
	if(x.cnt==0){
		x.head=NULL;
		x.tail=NULL;
	}
	else{
		node *temp = x.head;
		x.head=x.head->next;
		x.cnt--;
		free(temp);
	}
	return x;
}
node* llst_top(llsti x){
	if(x.cnt==0){
		return NULL;
	}
	else{
		return x.head;
	}
}

llst_str attach_str(llst_str x,char *z,int n){
	str_node* y =str_create_node();
	strcpy(y->str_val,z);

	// printf("%s\n",y->str_val);
	y->arrid=n;
	if(x.cnt==0){
		x.head=y;		
		x.tail=y;
	}
	else{
		x.tail->next = y;
		x.tail = x.tail->next;
	}
	x.cnt++;
	return x ;
}
void llst_traverse(llsti x){
	node *y = x.head;
	while(y!=NULL){
		printf("%d\n",y->val);	
		y=y->next;
	}
}

void llst_traverse_str(llst_str x){
	str_node *y = x.head;
	while(y!=NULL){
		printf("%s\n",y->str_val);	
		y=y->next;
	}
}

htable * insert(htable *z , char * y,int id){
	unsigned long h = hash(y);
	int x = h%123;
	// printf("hash value %d\n",x );
	z->llarray[x] = attach_str(z->llarray[x],y,id);
	return z;
}


int hfind(htable* z,char *y){
	unsigned long h = hash(y);
	int x = h%123;
	// printf("hash value %d\n",x );
	str_node *t = (z->llarray[x]).head;
	while(t!=NULL){
		// printf("%d\n",y->val);
		if(strcmp(t->str_val,y)==0){
			return (t->arrid)+1;  // retrieve this and index-1 on calls
		}	
		t=t->next;
	}	
	return 0;
	// while()
}

/*
int main(){
	htable *z = create_hashtable(123);
	z = insert(z,"dopey",1);
	int res = hfind(z,"dopey");
	printf("%d\n",res );
	// node *x = create_node(1);
	// printf("%d\n",x->val);
	// llsti list1 ;
	// list1.cnt=0;
	// // usage : as follows 
	// traverse(list1);
	// list1 = attach(list1,10);
	// list1 = attach(list1,10);
	// list1 = attach(list1,20);

}	*/
