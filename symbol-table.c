#include "symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


sym_node * sroot;
sym_node * scur;

int sub1,div1,add1,mul1,mat1,row1,col1;

void var_init(){
    mul1 = get_symb_no("MUL");
    div1 = get_symb_no("DIV");
    add1 = get_symb_no("PLUS");
    sub1 = get_symb_no("MINUS");    
    mat1 = get_symb_no("MATRIX");
    row1=0;
    col1=0;
}

sym_node* create_sym_node(){
    sym_node* x= (sym_node*)malloc(sizeof(sym_node));
    x->htlink = create_hashtable_symtable(60);
    x->parent = NULL;
    x->child=NULL;
    x->sibling=NULL;
    x->scope_name= (char *)malloc(sizeof(char)*50);
    return x;    
}

char * repeated_find(tnode *y){
    sym_node * x = scur;
    while(x!=NULL){
        char * res = find_sym_htable(x->htlink,y);
        if(res!=NULL){
            return res;
        }
        else{
            x=x->parent;
        }
    }
    return NULL;
}

// used to check it it exists in the hash table  
// if it does then return matrix's row and column 
pair * repeated_find_retrieve_rc_(tnode *y){
    sym_node * x = scur;
    while(x!=NULL){
        pair * res = retrieve_hash_table_rc(scur->htlink,y);
        if(res!=NULL){
            return res;
        }
        else{
            x=x->parent;
        }
    }
    return NULL;
}

int repeated_find_modify_rc_(tnode *y,pair *k){
    sym_node * x = scur;
    while(x!=NULL){
        int res= modify_hash_table_rc(scur->htlink,y,k);
        if(res!=0){
            return res;
        }
        else{
            x=x->parent;
        }
    }
    return 0;
}


pair* matrix_size_check(tnode *y){
    tnode * x = y;
    tnode *xc = y->child;
    int c = 0;
    int r = 0;

    // printf("issue here 1\n");
    pair * np = (pair *)malloc(sizeof(pair));
    while(xc!=NULL){
        c++;
        xc=xc->sibling;
    }
    // printf("issue here 2\n");
    while(x!=NULL){
        r++;
        tnode *t = x->child;    
        int temp=0;
        while(t!=NULL){
            temp++;
            t=t->sibling;
        }
        if(temp!=c){
            np->c=-1;
            np->r=-1;
            return np;
        }
        x=x->sibling;
    }
    // printf("issue here 3\n");

    np->r=r;
    np->c=c;
    return np;
}


// in case you find a proper matching rhs you have to assign / modify the value of
//  matrix's number of columns in the hashtable

int mat_rhs_traversal_helper(char* typ,tnode *x,char * ac1,char* ac2){
    // ensure the passed pointer is not ArithExp
    // printf("in here %s \n",symb_name[x->symb_no]);
    int a=1,b=1,c=1;
    
    if(x->child!=NULL){
        a = mat_rhs_traversal_helper(typ,x->child,ac1,ac2);
    }
    int y =x->symb_no;
    char *z = symb_name[x->symb_no];

    if(y==mul1 || y==add1 || y==sub1 || y==div1){
        if(y==mul1 || y==div1){
            printf("Error on line no %d",x->line_num);
            printf("matrix division or multiplication is not allowed \n");
        }
        b=1;
    }
    else if(strcmp(z,ac1)==0){
        // look for the id in the symbol table
        char * j=repeated_find(x);
        pair * k=repeated_find_retrieve_rc_(x);

        if(j==NULL){
            printf("Error on line %d undeclared variable %s \n",x->line_num,x->lexeme);
            // printf("here\n");
        }
        else{
            // k cant be NULL since we found the id in the hash 
            
            if(strcmp(j,typ)==0){
                b=1;
                if(k->r==-1){
                    printf("Error on line %d in %s",x->line_num,x->lexeme);
                    printf("unknown matrix size , invalidates computaion ,\
                     please assign the matrix variable  a value .\n");
                    b=0;
                }
                else if(row1==0 && col1==0){
                    // assign row value and col to this global variable
                    row1=k->r;
                    col1=k->c;
                }
                else if(row1!=k->r || col1!=k->c){
                    printf("Error on line %d in %s",x->line_num,x->lexeme);
                    printf("dissimilar matrix sizes for computation \n");            
                    b=0;
                }
            }
            else{
                b=0;
                
            }
        }
    } 
    else if(strcmp(z,ac2)==0){
        // the key matches to that of arithmetic expression hence the child contains a matrix
        // get the size of the matrix below
        b=1;
        pair *k =matrix_size_check(x->child); // this row->row->row with their respective col kids below
        // printf("m size %d %d \n",k->c,k->r);
        // printf("segment2\n");

        if(k->r==-1 || k->c==-1){
            printf("Error on line %d ",x->child->line_num);
            printf("non uniform matrix encountered in computation evry row should have same number of cols\n");
            b=0;
        }
        else if(row1==0 && col1==0){
            row1 = k->r;
            col1 = k->c;
        }
        else if(row1!=k->r || col1!=k->c){
            printf("Error on line %d ",x->child->line_num);
            printf("dissimilar matrix types in computation\n");
            b=0;
        }
    }
    else{

        if(strcmp(z,"<row>")==0 || strcmp(z,"NUM")==0){
            b=1;
        }
        else{
            b=0;
            printf("we hit this\n");
        }
    }
    
    if(x->child!=NULL){
        tnode *k = x->child->sibling;
        while(k!=NULL){
            c=c*mat_rhs_traversal_helper(typ,k,ac1,ac2);
            k=k->sibling;
        }
    }
    return a*b*c;
}


int rhs_traversal_helper(char* typ,tnode *x,char * ac1,char* ac2){
    // ensure the passed pointer is not rows
    int a=1,b=1,c=1;
    
    if(x->child!=NULL){
        a = rhs_traversal_helper(typ,x->child,ac1,ac2);
    }
    int y =x->symb_no;
    char *z = symb_name[x->symb_no];

    if(y==mul1 || y==add1 || y==sub1 || y==div1){
        b=1;
    }
    else if(strcmp(z,ac1)==0){
        // look for the id in the symbol table
        char * j=repeated_find(x);
        if(j==NULL){
            printf("Error on line %d undeclared variable %s \n",x->line_num,x->lexeme);
            printf("here\n");
        }
        else{
            if(strcmp(j,typ)==0){
                b=1;
            }
            else{
                b=0;
            }
        }
    } 
    else if(strcmp(z,ac2)==0){
        b=1;
    }
    else{
        b=0;
    }
    
    if(x->child!=NULL){
        tnode *k = x->child->sibling;
        while(k!=NULL){
            c=c*rhs_traversal_helper(typ,k,ac1,ac2);
            k=k->sibling;
        }
    }
    return a*b*c;
}

int non_matrix_rhs_traverse(char *typ,tnode *rhs){

    char *ac1 = (char*)malloc(sizeof(char)*30);
    char *ac2 = (char*)malloc(sizeof(char)*30);

    if(strcmp(typ,"INT")==0){
        strcpy(ac1,"ID");
        strcpy(ac2,"NUM");
    }
    else if(strcmp(typ,"REAL")==0){
        strcpy(ac1,"ID");
        strcpy(ac2,"RNUM");        
    }
    else if(strcmp(typ,"STRING")==0){
        strcpy(ac1,"ID");
        strcpy(ac2,"STR");
    }
    // printf("in here\n");
    // printf("%s %s",ac1,ac2);
    int ch = rhs_traversal_helper(typ,rhs,ac1,ac2);
    return ch;
}



/**************************************************************************************************/
/**************************************************************************************************/

void traverse_and_construct(tnode* x){
    if(x->child!=NULL)
        traverse_and_construct(x->child);

    // printf("%s \n",x->lexeme);
    int dec_stm = get_symb_no("<declarationStmt>");
    
    /******************* declaration contruct insertions *********************************************/
    /************************************************************************************************/
    
    if(x->symb_no==dec_stm){
        // printf("in dec \n");
        // there was an error here
        char * typ =x->child->child->lexeme;
        tnode *list = x->child->sibling->child;
        while(list!=NULL){
            char* check =repeated_find(list);
            if(check!=NULL){
                // printf("here\n");
                printf("Error on line %d redeclared variable %s \n",list->line_num,symb_name[list->symb_no]);
            }
            else{
                // printf("inserted\n");
                scur->htlink=insert_sym_htable(scur->htlink,list,typ,NULL,NULL);
            }
            list=list->sibling;
        }
    }

    /*******************************LHS 1 = RHS1 semantic checks*************************************/
    /************************************************************************************************/

    int rhs1 = get_symb_no("<rightHandSide_type1>");
    int lhs1 = get_symb_no("<leftHandSide_singleVar>");
    
    // char * typ_lhs = repeated_find(lhs);
    // if(typ_lhs==NULL){
    //     printf("lhs not declared before");
    // } use this to find lhs type and divert ways for matrix and nm

    if(x->symb_no==lhs1){
        char* check = repeated_find(x->child);
        // printf("check print %s",check);
        
        if(check==NULL){
            // printf("here\n");
            printf("Error on line %d undeclared variable %s \n",x->child->line_num,\
            x->child->lexeme);
        }
        else if(strcmp(check,"MATRIX")!=0){
            int ch2=non_matrix_rhs_traverse(check,x->sibling->sibling->child);
            if(ch2==0){
                printf("non - similar data types in computation on line %d \n",x->line_num);
            }
        }
        else{ 
            // ast constructed such that row variable has a parent rows else
            //  rows would not be in the ast;
            // printf("%s \n",symb_name[x->sibling->sibling->child->symb_no]);
            int ch2 = mat_rhs_traversal_helper(check,x->sibling->sibling->child,"ID","<rows>");
            if(ch2==1){
                // modify hashtable to add sizes to the matrix;
                // printf("computed \n");
                pair * kk = (pair*)malloc(sizeof(pair));
                kk->r = row1;
                kk->c = col1;
                repeated_find_modify_rc_(x->child,kk);
                printf("matrix size assigned to %s \n",x->child->lexeme);
                row1=0;
                col1=0;
            }
            else{
                printf("Error on line %d variable could note be computed because of thr above erros %s\
                 \n",x->child->line_num,\
                x->child->lexeme);
            }
        }
    }


    tnode * z = x->child;
    if(z!=NULL){
        tnode *y = z->sibling;
        while(y!=NULL){
            traverse_and_construct(y);
            y=y->sibling;
        }
    }
}

void make_sym_table(tnode *root){
    sroot= create_sym_node();
    sroot->scope_name="MAIN";        
    scur=sroot;
    traverse_and_construct(root);    
}

int main(){
    ast_constructer("add.txt","addout.txt");
    printf("******************************\n");
    // printf("%s\n",symb_name[ptree->child->symb_no]);
    // printf("******************************\n");    
    var_init();
    make_sym_table(ptree);
}