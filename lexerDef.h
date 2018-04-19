///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////
typedef struct node{
	int val;
	struct node * next ;
}node;

typedef struct str_node{
	char * str_val;
	int arrid;
	struct str_node * next ;
}str_node;

typedef struct llst__int{
	node *head;
	node *tail;
	int cnt;
}llsti;

typedef struct llst__str{
	str_node *head;
	str_node *tail;
	int cnt;
	int key_map;
}llst_str;

typedef struct _htable{
	llst_str * llarray ;
	int cnt;
}htable;

typedef struct token{
    // int tk_name;
    int line_no;
    char *tok_name; // this is the token name only as of now
    char *lexeme;
} token ;  

extern int eof_flag;
extern char * key_words[100];


