///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "astDefs.h"

int redundant[11];
char * red_names[9]={"MAIN","END","SQO","SQC","OP","CL","COMMA","SEMICOLON","eps"};

int rem_redundant(int num){  //this is the symb_no
    int i;
    for(i=0;i<9;i++){
        if(strcmp(symb_name[num],red_names[i])==0){
           return 1; 
        }
    }    
}

int get_symb_no(char * x){
    int i;
    for(i=0;i<84;i++){
        if(strcmp(x,symb_name[i])==0){
            return i;
        }
    }
    return -1;
}

tnode * tree_pruner(tnode* x,int par){

    tnode * y =x->child;
    int count =0;
    // printf("check1\n");
    if(x->child!=NULL){    
        if(rem_redundant(x->child->symb_no)==1){
            tnode* z; 
            z=x->child->sibling;
            // printf("%s \n",symb_name[z->symb_no]);
            
            while(z!=NULL){
                if(rem_redundant(z->symb_no)==1){
                    // printf("%s \n",symb_name[z->symb_no]);
                    z=z->sibling;
                }
                else{
                    break;
                }
            }
            x->child = z;
            // free memory
            tnode * yy = y;
            while(yy->sibling!=z){
                yy=yy->sibling;
            }
            yy->sibling=NULL;
            free(y);
        }
    }
    // printf("check2\n");

    if(x->child!=NULL){
        tnode * n = x->child;
        tnode * m = n;
        // printf("%s \n",symb_name[x->child->symb_no]);
        // printf("check1\n");

        while(n->sibling!=NULL ){
            // printf("check2\n");
            if(rem_redundant(n->sibling->symb_no)==1){
                tnode * zz = n->sibling;
                // printf("%s \n",symb_name[n->sibling->symb_no]);
                tnode *nm = n->sibling->sibling;
                // printf("%s 2 \n",symb_name[zz->symb_no]);
                // printf("%s 2 \n",symb_name[zz->symb_no]);

                // printf("in here\n");
                if(nm==NULL){
                    n->sibling=NULL;
                    n=nm;
                    break;
                }
                else{
                    n->sibling=n->sibling->sibling; 
                    // n=n->sibling;
                }
            }
            else{
                n=n->sibling;
            }        
        }
    }
    // x->child=m;

    // printf("%s \n",symb_name[x->symb_no]);
    // if(x->child->sibling!=NULL)
        // printf("bacha sib %s \n",symb_name[x->child->sibling->symb_no]);
    
    tnode *xx =x->child;
    if(xx!=NULL){
        count=1;
        while(xx->sibling!=NULL){
            // printf("boom\n");
            // printf("%d",xx->symb_no);
            // if(xx->sibling->symb_no!=-42)
                // printf("%s --> ",symb_name[xx->sibling->symb_no]); 
            xx=xx->sibling;
            count++;
        }
    }
    // printf("child check fin \n");
    x->ch_num=count;

    // printf("%s \n",symb_name[x->symb_no]);
    // printf("before sibs\n");
    if(x->sibling!=NULL){
        // printf("sibling %s \n",symb_name[x->sibling->symb_no]);
        x->sibling=tree_pruner(x->sibling,2);

    }
    // printf("before kids\n");
    if(x->child!=NULL)
        x->child = tree_pruner(x->child,1);

    
    /* ****** declaration statement pruning should work for left multi list as well ***** 
    TYPE -- > VARLIST 
                |
                ID , ID ,ID     
    ******************************************* */
    int ast_id  =get_symb_no("ID");

    if(x->symb_no==ast_id && x->sibling!=NULL && x->sibling->symb_no==get_symb_no("<more_ids>")){
        x->sibling=x->sibling->child;         
        return x;
    }
    
    /* ****** conditional statement pruning ***** 
    LT /GT etc
        |    
    <constrainedVars Child > -> <constrainedVars Child >
    ******************************************* */
    int bool_id  =get_symb_no("<booleanExpression>");
    // exclude the not case important 
    if(x->symb_no==bool_id ){
        int cvars =  get_symb_no("<constrainedVars>");
        int sym_not = get_symb_no("NOT");
        
        if(x->child->symb_no==cvars){
            tnode * one   = x->child->sibling->child;            
            tnode * two = x->child->child;
            tnode * three = x->child->sibling->sibling->child;
            
            three->sibling = NULL;
            
            x->child= one ;
            // x->child->sibling= NULL;

            x->child->child = two;
            x->child->child->sibling = three; 

            printf("%s %s %s \n",symb_name[x->child->symb_no],symb_name[x->child->child->symb_no],\
            symb_name[x->child->child->sibling->symb_no]);

            x->child->sibling = x->sibling;
            printf("done\n");
            //  change to x->child to return x->child
        }
        else if(x->child->symb_no!=sym_not){
            tnode * one   = x->child->sibling->child;
            tnode * two = x->child;
            tnode * three = x->child->sibling->sibling;
          
            
            x->child=one;

            x->child->child = two;
            x->child->child->sibling = three; 
            // printf("%s %s %s \n",symb_name[x->child->symb_no],symb_name[x->child->child->symb_no],\
            symb_name[x->child->child->sibling->symb_no]);
            // printf("done3\n");
            x->child->sibling = x->sibling;
        }
        
        return x->child;
    }

    /************** parameter_list pruning ********* 
    <parameter_list> ===> <type> ID <remainingList>
      
    ************************************************/
    
    // there was ! here in x->sibling->symb_no != paralist_id which was wrong check other statements written while tests

    int paralist_id  =get_symb_no("<remainingList>");
    if(x->symb_no == ast_id && x->sibling!=NULL && x->sibling->symb_no== paralist_id){
        x->sibling = x->sibling->child;         
        return x;
    }


    /************** listVar  pruning  for inputParameterList in function********* 
    /* <var> <listVar> */

    int var_id  =get_symb_no("<var>");
    int listvar_id = get_symb_no("<listVar>");
    if(x->symb_no == var_id && x->sibling!=NULL && x->sibling->symb_no== listvar_id){
        x->sibling = x->sibling->child->child;         
        return x;
    }
      
    /* 
    /************** rows  pruning  ********* 

    <rows> ===> <row> <RowLit>
    <RowLit> ===> SEMICOLON <rows>      
    */ 
    // occurence of an aritmetic expression term will now signify a row
    int row_id    = get_symb_no("<row>");
    int rowLit_id = get_symb_no("<RowLit>");
    if(x->symb_no == row_id && x->sibling!=NULL && x->sibling->symb_no== rowLit_id){
        x->sibling = x->sibling->child->child;         
        return x;
    }

    /* 
    /**************  accumulating column elements as a list  ********* 
    /*  fiddles with <row> ===> NUM <remainingColElements> 
    */
    int num_id    = get_symb_no("NUM");
    int remcol_id = get_symb_no("<remainingColElements>");
    
    if(x->symb_no == num_id && x->sibling!=NULL && x->sibling->symb_no== remcol_id){
        x->sibling = x->sibling->child;         
        return x;
    }

    /* 
    /**************  working on the arithmetic expressions now and pruning them  ********* 
    /*   
    */
    
    /* 
    
    <arithmeticExpression> ===> <arithmeticTerm> <ArithExpFactored>
    <ArithExpFactored> ===> <operator_lowPrecedence> <arithmeticExpression>
    <ArithExpFactored> ===> eps
    <arithmeticTerm> ===> <factor> <ArithTermFactored>
    <ArithTermFactored> ===> <operator_highPrecedence> <arithmeticTerm>
    <ArithTermFactored> ===> eps
    <factor> ===> OP <arithmeticExpression> CL
    <factor> ===> <var>
    
    */    

    int arith_term =  get_symb_no("<arithmeticTerm>");    
    int arith_id  = get_symb_no("<arithmeticExpression>");
    int arith_exp_fact = get_symb_no("<ArithExpFactored>");    
    int op_low = get_symb_no("<operator_lowPrecedence>");
    int op_hi = get_symb_no("<operator_highPrecedence>");
    int fact  = get_symb_no("<factor>");
    int arith_term_fact=get_symb_no("<ArithTermFactored>");       
    int mul =get_symb_no("MUL");
    int div =get_symb_no("DIV");
    int add =get_symb_no("PLUS");
    int sub =get_symb_no("MINUS");

    // printf("checker %s\n",symb_name[arith_term_fact]);
    if(x->symb_no==op_hi || x->symb_no == op_low){
        x->child->sibling=x->sibling;
        return x->child;
    }
    /// base case 1 2 3
    if(x->symb_no==arith_term && x->sibling!=NULL && x->sibling->symb_no==arith_exp_fact){
        x->sibling = x->sibling->child;
        // printf("%s ** \n",x->child->lexeme);
    
        if(x->symb_no==arith_term && x->child!=NULL && x->child->sibling!=NULL && x->child->sibling->sibling!=NULL){
        // printf("wooter\n");
            // printf("%s \n",x->child->lexeme);
            tnode * one , *two,*three;
            one = x->child->sibling;
            two = x->child;
            three = x->child->sibling->sibling;        

            x->child=one;
            x->child->child = two;
            x->child->child->sibling = three; 
            x->child->sibling = x->sibling;
            return x->child;
        }
    
        if(x->symb_no==arith_term && x->child!=NULL && x->child->sibling==NULL){
            x->child->sibling = x->sibling;
            return x->child;
        }        
        return x;
    }
    if((x->symb_no== add ||x->symb_no== sub)  && x->sibling!=NULL && x->sibling->symb_no==arith_exp_fact){
        x->sibling = x->sibling->child;
        return x;
    }
    if((x->symb_no== div ||x->symb_no== mul) && x->sibling!=NULL && x->sibling->symb_no==arith_exp_fact){
        x->sibling = x->sibling->child;
        return x;
    }
    // base cases for the factor ones
    if(x->symb_no==fact && x->sibling!=NULL && x->sibling->symb_no==arith_term_fact){

        x->sibling = x->sibling->child;
            // printf("woot\n");
        // matlit case hmm
        int y = x->child->symb_no; 
        if(y==get_symb_no("ID")||y==get_symb_no("NUM")||y==get_symb_no("RNUM")||y==get_symb_no("STR") \
            || y==get_symb_no("<row>") ||y==get_symb_no("FUNID") || y ==  add ||y == sub \
            || y== mul ||y== div){

            if(y==get_symb_no("ID") && x->child->sibling!=NULL){
                // The curious case of matlit
                x->child->sibling->sibling=x->sibling; 
                return x->child;
            }    
            x->child->sibling=x->sibling;
            return x->child;
        }
        return x;
    }
    if((x->symb_no== add ||x->symb_no== sub) && x->sibling!=NULL && x->sibling->symb_no==arith_term_fact){
        x->sibling = x->sibling->child;
        return x;
    }
    if((x->symb_no== mul ||x->symb_no== div) && x->sibling!=NULL && x->sibling->symb_no==arith_term_fact){
        x->sibling = x->sibling->child;
        return x;
    }    

    // now let's resolve the arithemticTerm and Arithmetic Expression constructs needs some fixing with respect to factor wali computations
    if(x->symb_no==arith_term && x->child!=NULL && x->child->sibling!=NULL && x->child->sibling->sibling!=NULL){
        // printf("wooter\n");
        // printf("%s \n",x->child->lexeme);
        tnode * one , *two,*three;
        one = x->child->sibling;
        two = x->child;
        three = x->child->sibling->sibling;        

        x->child=one;
        x->child->child = two;
        x->child->child->sibling = three; 
        x->child->sibling = x->sibling;
        return x->child;
    }
     
    if(x->symb_no==arith_id && x->child!=NULL && x->child->sibling!=NULL && x->child->sibling->sibling!=NULL){
        tnode * one , *two,*three;
        one = x->child->sibling;
        two = x->child;
        three = x->child->sibling->sibling;        

        x->child=one;
        x->child->child = two;
        x->child->child->sibling = three; 
        
        x->child->sibling = x->sibling;
        return x->child;        
    }
        
    if(x->symb_no==arith_id && x->child!=NULL && x->child->sibling==NULL){
        x->child->sibling = x->sibling;
        return x->child;
    }
    if(x->symb_no==arith_term && x->child!=NULL && x->child->sibling==NULL){
        // printf("woots\n");
        x->child->sibling = x->sibling;
        // printf("%s\n",x->child->lexeme);
        return x->child;
    }

    if(count==0 && x->sibling==NULL){
        if(x->symb_no<39){
            if(x->symb_no==0){
                return NULL;
            }
            return x;  // pay careful attention here 
        }
        else{
            return NULL; //  eps followed by eps looks impossible but think about it 
        }
    }
    else if(/* count==1 && */ par ==1 && x->sibling==NULL){
        if(x->symb_no==row_id){
            return x; // trying to preserve some structure helpful for the things ahead
        }
        if(x->symb_no==get_symb_no("<rows>")){
            return x;
        }
        if(x->symb_no==get_symb_no("<declarationStmt>")){
            return x;
        }
        return x->child;
    }
    return x;
/*     xx =x->child;
    
    if(xx!=NULL){
        // count=1
        xx=x->child->sibling;
        while(xx!=NULL){
            // printf("boom\n");
            // printf("%s prints \n",symb_name[x->sibling->symb_no]); 
            xx=tree_pruner(xx,2);
            xx=xx->sibling;
            // count++;
        }
    }
    // count again
    xx=x->child;
    if(xx!=NULL){
        count=1;
        while(xx->sibling!=NULL){
            // printf("boom\n");
            // printf("%s prints \n",symb_name[x->sibling->symb_no]); 
            xx=xx->sibling;
            count++;
        }
    }        
    // if(count==1)
 */    
    
}


void calculate_children(tnode* trv_node){
	if(trv_node==NULL){
		return;
	}
	if(trv_node->child!=NULL){		
		calculate_children(trv_node->child);	
	}
    trv_node->ch_num=0;
	if(trv_node->symb_no == -42){
        trv_node->ch_num=1;
    }
	else{
        tnode *temp=trv_node->child;
        while(temp!=NULL){
            trv_node->ch_num+=1;
            if(trv_node->ch_num==0){
                temp = trv_node->child->sibling;                
            }
            else{
                temp = temp->sibling;
            }
        }
	}
    if(trv_node->symb_no!=-42)
        // printf("%s %d\n",symb_name[trv_node->symb_no],trv_node->ch_num);
        ;
    else{
        // printf("%s %d\n","$",trv_node->ch_num);
    }
    if(trv_node->child!=NULL){
		tnode * x = trv_node->child->sibling;
		while(x!=NULL){
			calculate_children(x);
			x = x->sibling;
		}
	}
}

ast_node* makenode(tnode * x){
    // copy contents from tnode to x 
    // get the pointers from the kid 
    ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
    
    temp->sibling=NULL;
    temp->child=NULL;

    temp->symb_no=x->symb_no;
    temp->lexeme = x->lexeme;// could be prone to errors of allocation
    temp->line_num = x->line_num;
    return temp;
}

ast_node* make_ast(tnode *x){
    if(x==NULL){
        return NULL;
    }
    if(x->symb_no==0){  // epsilon issues
        return NULL;
    }
    if(x->ch_num==0 && x->sibling==NULL){
        return makenode(x);
    }
    else if(x->ch_num==1 && x->sibling==NULL){
        return make_ast(x->child);
    }   
    else{
        ast_node *temp =(ast_node *)malloc(sizeof(ast_node));
        
        temp->symb_no=x->symb_no;
        temp->lexeme = x->lexeme ; // check for errors here 
        temp->line_num = x->line_num;
        if(x->child!=NULL)
            temp->child =  (make_ast(x->child));
        else{
            temp->child=NULL;
        }
        if(x->sibling!=NULL){
            temp->sibling = (make_ast(x->sibling));
        }
        else{
            temp->sibling =NULL;
        }
        // if(x->symb_no!=-42)
        // printf("%s\n",symb_name[x->symb_no]);

        return temp;
    }
}

void update_ptree(char* inp, char *outp){
    init_lexer(inp);
	init_grammar();

	init_reverse_token_lookup_hash();
	init_ptree_pstack();

	parse();
	close_file_lexer();

    calculate_children(ptree);
}


void astraverse(ast_node *trv_node){
    if(trv_node==NULL){
		return;
	}

	if(trv_node->child!=NULL){
		// printf("1\n");
		// printf("%s \n",symb_name[trv_node->symb_no]);
		// // printf("%s\n",trv_node->child->lexeme);		
	    // printf("2\n");
        if(trv_node->child == NULL){
            printf("null hai bete\n");
        }
        astraverse(trv_node->child);	
	}

	if(trv_node->symb_no == -42){
        printf("dollar\n");
    }
	else{
        printf("%s \n",symb_name[trv_node->symb_no]);
	}
		

	if(trv_node->child!=NULL){
		// printf("in here %s ")
		ast_node * x = trv_node->child->sibling;
		while(x!=NULL){
			// printf("not null\n");
			// printf("%s par %s \n",x->lexeme,trv_node->lexeme);
			// printf("%s\n",x->lexeme);
            // printf("3\n");
            astraverse(x);
			x = x->sibling;
		}
	}
}

void ptraverse(tnode *trv_node){
    if(trv_node==NULL){
		return;
	}
	if(trv_node->child!=NULL){
		// printf("not null\n");
		// printf("%s par %s \n",trv_node->child->lexeme,trv_node->lexeme);
		// printf("%s\n",trv_node->child->lexeme);		
		ptraverse(trv_node->child);	
	}
	// printf("%d\n",trv_node->symb_no);
	
	if(trv_node->symb_no == -42){
        printf("$\n");
    }
	else{
        printf("%s \n",symb_name[trv_node->symb_no]);
	}
	if(trv_node->child!=NULL){
		// printf("in here %s ")
		tnode * x = trv_node->child->sibling;
		while(x!=NULL){
			// printf("not null\n");
			// printf("%s par %s \n",x->lexeme,trv_node->lexeme);
			// printf("%s\n",x->lexeme);
			ptraverse(x);
			x = x->sibling;
		}
	}
}

void ast_constructer(char *inp,char *outp){
    // rem_redundant();
    update_ptree(inp,outp);
    // ast_node * ast_root = make_ast(ptree);
    // printf("%s \n",symb_name[ptree->child->symb_no]);
    ptree->child=tree_pruner(ptree->child,1);
    printf("pruned\n");
    // printf("%s \n",symb_name[ptree->child->symb_no]);
    // printf("%s \n",symb_name[ptree->child->child->sibling->symb_no]);
    printf("%d \n",ptree->symb_no);
    ptraverse(ptree);
    // int symb = ptree->child->sibling->sibling->sibling->symb_no;
    // printf("%s \n",symb_name[symb]);
    // astraverse(ast_root);
}


// int main(){

//     // rem_redundant();
//     update_ptree("./revised_testcases/testcase5.txt","addres.txt");
//     // ast_node * ast_root = make_ast(ptree);
//     // printf("%s \n",symb_name[ptree->child->symb_no]);
//     ptree->child=tree_pruner(ptree->child,1);
//     printf("pruned\n");
//     // printf("%s \n",symb_name[ptree->child->symb_no]);
//     // printf("%s \n",symb_name[ptree->child->child->sibling->symb_no]);
//     printf("%d \n",ptree->symb_no);
//     ptraverse(ptree);
//     // int symb = ptree->child->sibling->sibling->sibling->symb_no;
//     // printf("%s \n",symb_name[symb]);
//     // astraverse(ast_root);
// }