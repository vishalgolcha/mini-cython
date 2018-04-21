///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// hash token name with the hash function you have and store in it the original
// in the array

// might need a change for this global var
int res=0;
htable * token_lookup;
tnode *ptree;
tnode *cur;
llsti ptstack;
// dnt forget to initialize this stack

typedef struct _stack_pair{
	llsti one,two;
}s_pair;

long long int error_heuristic = 0; 

void init_reverse_token_lookup_hash(){
    token_lookup = create_hashtable(123);
    int i;
    for(i=0;i<84;i++){ // remove main from here , na let it stay
        token_lookup = insert(token_lookup,symb_name[i],i);        
    }
}

// you receive node * elements when the stack pops
tnode *init_tnode(tnode *x){
	x=(tnode *)malloc(sizeof(struct _tnode));
	x->child=NULL;
	x->parent=NULL;
	x->sibling=NULL;
	x->lexeme = (char*)malloc(sizeof(char)*100);
	return x;
}

tnode * assign_values(tnode *x,char* lex,int id,int lnumber){
	if(id<39)
		strcpy(x->lexeme,lex);
	else{
		strcpy(x->lexeme,symb_name[id]);
	}
	x->symb_no=id;
	x->line_num=lnumber;
	return x;
}

void init_ptree_pstack(){		
	ptree= init_tnode(ptree);
	ptree =assign_values(ptree,"$",-42,-1);
	ptree->child= init_tnode(ptree->child);
	ptree->child= assign_values(ptree->child,"<mainFunction>",hfind(token_lookup,"<mainFunction>")-1,-1);
	// printf("%s \n",ptree->child->lexeme);
	cur=ptree->child;
	cur->parent = ptree;
	ptstack.cnt=0;
	ptstack=rev_attach(ptstack,-42); //inserting a dollar id
	ptstack=rev_attach(ptstack,hfind(token_lookup,"<mainFunction>")-1);
}


// this function uses global variables cur and the global stack
llsti accumulate_stack(llsti x,int rule_no){
	rnode * to_insert = &rule_adt[rule_no].rhs;
	while(to_insert!=NULL){
		// if(to_insert->symb_no!=0)
		x=rev_attach(x,to_insert->symb_no);
		// printf("%d ",to_insert->symb_no);
		to_insert = to_insert->next;
	}
	return x;
}

s_pair stack_reverse(llsti x,llsti y){  // transfers from x to y
	node * z = llst_top(x);
	// printf("done\n");
	while(z!=NULL){
		// printf("%d \n",z->val);
		// printf("%d \n",x.cnt);
		y=rev_attach(y,z->val);
		x=llst_pop(x);
		z=llst_top(x);
	}

	s_pair ret ;
	ret.one = x; 
	ret.two = y;
	return ret;
}

token *cur_tok ;
FILE * outfile;
void init_file(char * z){
	outfile=fopen(z,"w");
}
void close_file(){
	fclose(outfile);
}
int parse_process(){
	// the moment you get into this you pop the stack
	// x= get_next_token();
	int tk_num  =  hfind(token_lookup,cur_tok->tok_name)-1;// hash and obtain ; // mentioned as lexeme prevsly
	node * pnodetemp =llst_top(ptstack);
	int tos_num =  pnodetemp->val;
	ptstack=llst_pop(ptstack);

	llsti temp1 , temp2; // declare an integer stack to reverse the rules
	temp1.cnt=0;
	temp2.cnt=0;

	// printf("%s %s\n",symb_name[tos_num],symb_name[tk_num]);
	
	// some error in reversal as of now 
	if(tos_num==0){  //epsilon encoutnered pop
		// printf("epsilon state %s \n",symb_name[cur->symb_no]);
		// if(tos_num==tk_num){

/*		this was an error we dont need to assign 
		cur = assign_values(cur,cur_tok->lexeme,
			hfind(token_lookup,cur_tok->tok_name)-1);
*/		
		// printf("%s \n",symb_name[cur->symb_no]);
		// printf("current name %s \n",symb_name[cur->symb_no]);
		// printf("booom\n");
		// go back to the parent incorrect 
		tnode* sib =cur->sibling;
		// printf("%d \n",sib->symb_no);
		if(sib==NULL){
			// printf("in here mfs\n");
			tnode * par=cur->parent;
			// printf("parent found\n");
			// printf("%s \n",symb_name[par->symb_no]);
			while(par->sibling==NULL){
				par=par->parent;
				// printf("parent found\n");
				// printf("%s \n",symb_name[par->symb_no]);

				if(par->symb_no == -42){
					cur_tok=get_next_token();

					while(cur_tok==NULL && eof_flag==0){
						cur_tok =get_next_token();
					}
					// printf("eps return \n");
					return 3;					
					break; //signal end of the parsing process
				}
			}
			// printf("**********parent found\n");
			// printf("%s \n",symb_name[par->symb_no]);

			cur = par->sibling;
		}
		else{
			cur=sib;
		}
		// }
		// printf("epsilon state %s \n",symb_name[cur->symb_no]);
		return 1;
	}
	if(tos_num >=39){
		// printf("%s %s\n",symb_name[tos_num],symb_name[tk_num]);

		// could give error if all values in parse table not aligned to 0
		// printf("i am a non terminal\n");
		if(parse_table[tos_num][tk_num] != -1){ // 0 signifies error state in parse table
			// printf("in here woohoo\n");
			int rno  = parse_table[tos_num][tk_num];
			// we'll reverse the rhs using an explicit stack 
			
			temp1 = accumulate_stack(temp1,rno);
			// printf("***************\n");
			// llst_traverse(temp1);
			s_pair  temp3 = stack_reverse(temp1,temp2);
			temp1 = temp3.one;
			temp2 = temp3.two;
			// printf("***************\n");
			// llst_traverse(temp2);

			temp1 = accumulate_stack(temp1,rno); // needed for parse tree
			// printf("***************\n");
			// llst_traverse(temp1);

			node * temp2_iter = llst_top(temp2);
		

			if(temp2_iter!=NULL){
				cur->child= init_tnode(cur->child);
				cur->child->symb_no = temp2_iter->val;
				cur->child=assign_values(cur->child,symb_name[temp2_iter->val],temp2_iter->val,cur_tok->line_no);				
				temp2_iter=temp2_iter->next;
				
				// if(temp2_iter==NULL){ since we need every parent pointer else one would suffice
					cur->child->parent = cur;
				// }
			}
			// printf("symbol name %s ",symb_name[cur->child->symb_no]);
			
			tnode *sibling_iterator = cur->child;
			// tnode * sibling_iterator = cur->child->sibling;
			
			while(temp2_iter!=NULL){
				sibling_iterator->sibling = init_tnode(sibling_iterator->sibling);
				sibling_iterator->sibling->symb_no = temp2_iter->val;
				// sibling_iterator = sibling_iterator->sibling;
				sibling_iterator->sibling=assign_values(sibling_iterator->sibling,symb_name[temp2_iter->val],temp2_iter->val,cur_tok->line_no);
				sibling_iterator = sibling_iterator->sibling;	// firse check this out for errors 			
				// printf(" %s ",symb_name[sibling_iterator->symb_no]);
				// if(temp2_iter->next==NULL){
					sibling_iterator->parent = cur;
				// }
				// sibling_iterator = sibling_iterator->sibling;
				temp2_iter=temp2_iter->next;
			}
			// printf("\n");
			// printf("***sib check***\n");
			// printf("%d \n",cur->child->sibling->symb_no);

			if(temp2_iter==NULL){
				sibling_iterator =NULL;
			}
			cur=cur->child;
			// tnode * parent_ch=cur;
			// while(parent_ch!=NULL){
			// 	printf("parent %s %s\n",parent_ch->parent->lexeme,parent_ch->lexeme);
			// 	parent_ch =parent_ch->sibling;
			// }
			// now empty temp2 into ptstack
			temp3 = stack_reverse(temp1,ptstack);
			temp1 = temp3.one;
			ptstack=temp3.two;
			// weird behaviour from this print statement
			// printf("the kid %s \n",symb_name[cur->symb_no]);
			// printf("je ne sais pas\n");
			return 1;
		}
		else{
			// throw out a error 
			// printf("%s %s\n",symb_name[tos_num],symb_name[tk_num]);
			printf("Syntax Error : line no : %d No entry in parse table COMPILER IN PANIC RECOVERY MODE \n",cur_tok->line_no);
			error_heuristic=0;
			// we store first and follow sets of tos_num in error_heuristic 
			error_heuristic |= first_sets[tos_num][1];
			error_heuristic |= follow_sets[tos_num];
			// re insert the current element to the top of the stack
			ptstack = rev_attach(ptstack,tos_num);
			return -1;
		}
	}
	else {
		if(tos_num==tk_num){
			// printf("%s %s\n",symb_name[tos_num],symb_name[tk_num]);

			cur = assign_values(cur,cur_tok->lexeme,
				hfind(token_lookup,cur_tok->tok_name)-1,cur_tok->line_no);
			// printf("%s \n",symb_name[cur->symb_no]);
			// printf("current name %s \n",symb_name[cur->symb_no]);
			// printf("booom\n");
			// go back to the parent incorrect 
			tnode* sib =cur->sibling;
			// printf("%d \n",sib->symb_no);
			if(sib==NULL){
				// printf("in here mfs\n");
				tnode * par=cur->parent;
				// printf("current parent %s \n",symb_name[par->symb_no]);
				// printf("parent found\n");
				// printf("%s \n",symb_name[par->symb_no]);
				while(par->sibling==NULL){
					par=par->parent;
					// printf("parent found\n");
					// printf("%s \n",symb_name[par->symb_no]);

					if(par->symb_no == -42){
						cur_tok=get_next_token();
						while(cur_tok==NULL && eof_flag==0){
							cur_tok =get_next_token();
						}
						// printf("non eps return \n");
						return 3;
						break; //signal end of the parsing process
					}
				}
				// printf("**********parent found\n");
				// printf("%s \n",symb_name[par->symb_no]);

				cur = par->sibling;
			}
			else{
				cur=sib;
			}

			return 2;
		}
		else{
				// printf("%s %s\n",symb_name[tos_num],symb_name[tk_num]);

			printf("Syntax Error: line no %d Expecting a different type of terminal here \n",cur_tok->line_no);
			// retrieve next token accepting the fact that 
			// assume that the user gave a wrong terminal and move on which means you retrieve next token from lexer
			return -2;
		}
	}
}

void parse(){
	fflush(stdin);
	// if parse_process return 3 it means it returned to $ symbol in the stack
	// implying the parse tree has been formed 
	// if it returns a 2 that means a terminal was matched correctly
	cur_tok =get_next_token();
	while(cur_tok==NULL){
		cur_tok =get_next_token();
	}
	// printf("%s \n",cur_tok->tok_name);
	
	// was stupid enough to not use strcmp here in the first phase
	
	while(strcmp(cur_tok->tok_name,"eof")!=0){

		res = parse_process();
		if(res==2 || res==-2 ){
			cur_tok=get_next_token();
			while(cur_tok==NULL){
				cur_tok =get_next_token();
			}
			// printf("%s");
			// if(cur_tok->tok_name=="eof"){
				// printf("bo\n");
			// }
			// parse_process();
			// printf("%s \n",cur_tok->tok_name);
		}
		else if(res==3){
			// printf("arre re\n");
			cur_tok=get_next_token();
			while(cur_tok==NULL){
				cur_tok =get_next_token();
			}

			break;
		}
		else if(res==-1){
			
			cur_tok=get_next_token();
			while(cur_tok==NULL){
				cur_tok =get_next_token();
			}				
			int token_num = hfind(token_lookup,cur_tok->tok_name)-1;
			
			while(error_heuristic&(1LL<<token_num)==0 && cur_tok->tok_name!="eof"){
				printf("Syntax Error : line no :%d token not in synchronizing set",cur_tok->line_no);
				cur_tok=get_next_token();
				while(cur_tok==NULL){
					cur_tok=get_next_token();					
				}
				token_num=hfind(token_lookup,cur_tok->tok_name)-1;				
			}
			if(cur_tok->tok_name!="eof"){
				printf("Recovery mode parse tree generated\n");
				break ;
			}
		}
		// if(eof_flag==0)
		// 	res=parse_process();
	}




	// updates here 
	// printf("%s \n",cur_tok->tok_name);
	// printf("%s \n",)
	// llst_traverse(ptstack);
	//
	if(eof_flag==1 && res==3){
		printf("properly parsed without syntactic errors\n");
	}
	else if(res==3){
		printf("Improper parse tree generation  stack pre emptively empty before eof\n");
	}
	else{
		printf("Stack not empty yet \n");
		// llst_traverse(ptstack);
		// printf("%s\n",symb_name[78]);
		// printf("jeez\n");
	}	

}
void inorder(tnode* trv_node){

	// lexemeCurrentNode  lineno token valueIfNumber parentNodeSymbol
	// isLeafNode(yes/no) NodeSymbol
	if(trv_node==NULL){
		return;
	}
	if(trv_node->child!=NULL){
		// printf("not null\n");
		// printf("%s par %s \n",trv_node->child->lexeme,trv_node->lexeme);
		// printf("%s\n",trv_node->child->lexeme);		
		inorder(trv_node->child);	
	}

	
	// printf("%d\n",trv_node->symb_no);
	
	if(trv_node->symb_no == -42){
		// fprintf(outfile,"----\n");
		fprintf(outfile,"%25.25s %6d %25.25s %25.25s %25.25s %6.6s %25.25s\n","----",-1,"----","----","----","NO","$");
	}
	else{
		if(strcmp(trv_node->lexeme,"<mainFunction>")==0){
			// fprintf(outfile,"parent $\n");
			fprintf(outfile,"%25.25s %6d %25.25s %25.25s %25.25s %6.6s %25.25s\n","----",-1,"----","----","$","NO","<mainFunction>");
		}
		else if(trv_node->symb_no==0){
			// fprintf(outfile,"%s\n",trv_node->parent->lexeme);
			fprintf(outfile,"%25.25s %6d %25.25s %25.25s %25.25s %6.6s %25.25s\n","eps",-1,"eps","----",trv_node->parent->lexeme,"YES","----");	
		}
		else if(strcmp(symb_name[trv_node->symb_no],"RNUM")==0 ||strcmp(symb_name[trv_node->symb_no],"NUM")==0){
			fprintf(outfile,"%25.25s %6d %25.25s %25.25s %25.25s %6.6s %25.25s\n",trv_node->lexeme,trv_node->line_num,symb_name[trv_node->symb_no]
				,trv_node->lexeme,trv_node->parent->lexeme,"YES","----");	
		}
		else if(trv_node->symb_no <=38){
			fprintf(outfile,"%25.25s %6d %25.25s %25.25s %25.25s %6.6s %25.25s\n",trv_node->lexeme,trv_node->line_num,symb_name[trv_node->symb_no]
				,"----",trv_node->parent->lexeme,"YES","----");
		}
		else if(trv_node->symb_no>38){
			fprintf(outfile,"%25.25s %6d %25.25s %25.25s %25.25s %6.6s %25.25s\n","----",-1,"----","----",trv_node->parent->lexeme,"NO",trv_node->lexeme);	
		}
		// else if()
	}
	if(trv_node->child!=NULL){
		// printf("in here %s ")
		tnode * x = trv_node->child->sibling;
		while(x!=NULL){
			// printf("not null\n");
			// printf("%s par %s \n",x->lexeme,trv_node->lexeme);
			// printf("%s\n",x->lexeme);
			inorder(x);
			x = x->sibling;
		}

	}
}
void get_only_token(char *x){
	init_lexer(x);

	cur_tok =get_next_token();
	while(cur_tok==NULL){
		cur_tok =get_next_token();
	}

	while(strcmp(cur_tok->tok_name,"eof")!=0){
		printf("%s %s %d\n",cur_tok->tok_name,cur_tok->lexeme,cur_tok->line_no);
		cur_tok =get_next_token();
		while(cur_tok==NULL){
			cur_tok =get_next_token();
		}
	}
	close_file_lexer();
}

void get_syntactic_errors(char* inp){
	init_lexer(inp);
	// printf("lexer init done\n");

	init_grammar();
	// printf("no isues\n");

	init_reverse_token_lookup_hash();

	init_ptree_pstack();
	parse();
	close_file_lexer();

}
void traverse_parse_tree(char *inp,char *outp){
	init_lexer(inp);
	init_grammar();

	init_reverse_token_lookup_hash();

	init_ptree_pstack();

	parse();
	close_file_lexer();

	// printf("%s \n",ptree->child->child->sibling->sibling->sibling->lexeme);
	// char * fname = ;
	// FILE * fap ;
	// fap=fopen("results1.txt",'w');
	init_file(outp);
	fprintf(outfile,"%25.25s %6s %25.25s %25.25s %25.25s %6.6s %25.25s\n","lexeme curnode" ,"lineno","token ","valifnum " ,"parent symbol ","isleaf","symbol name");
	inorder(ptree);
	close_file();
}
	// reverse look up debugger
// int main(){
	// traverse_parse_tree("./revised_testcases/testcase5.txt","rev_test5_res.txt");
	// // -1 indicates dollar
	// get_syntactic_errors("test4.txt");
	// // uncommentedfile("test4.txt");
	// // get_only_token("test4.txt");

	// // fclose(fap);
	// debuggers
	// llst_traverse(ptstack);
	// ptstack  = llst_pop(ptstack);
	// llst_traverse(ptstack);
	// llsti temp1,temp2;
	
	// temp1.cnt=0;
	// temp2.cnt=0;

	// // printf("bhangra\n");
	// // reversal debug
	// temp1 = accumulate_stack(temp1,12);
	// temp2 = stack_reverse(temp1,temp2);
	// // llst_traverse(temp2);
// }	
