///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "lexer.h"
#include <ctype.h>

#define buffsize 1024

// char* key_words[100];
char *buffer1 ;
char *buffer2 ;

// implement retract and get_next_char type of functions 
enum token_names{ASSIGNOP,COMMENT,FUNID,ID,NUM,RNUM,STR,END,INT,STRING,MATRIX,MAIN,SQO,SQC,OP,CL,SEMICOLON
,COMMA,IF,ELSE,ENDIF,READ,PRINT,FUNCTION,PLUS,MINUS,MUL,DIV,SIZE,AND,OR,NOT,LT,EQ,GT,LE,NE};

size_t size = 0;
int ptr; // global ptr on the buffer array you;ve loaded
int lno; // global line number descriptor

char cur_char ;
FILE *fp;
int buf1_flag;
int buf2_flag;
int set_unset_flag;
int l_ptr_in_buf;

int tok_cofirm;
int l_ptr,r_ptr;
int set_unset_flag;
int retract_buffer_flag;
int eof_flag;

htable * keyword_hash ;

void init_keyword_hash(){
    keyword_hash = create_hashtable(123);
    int i;
    for(i=0;i<15;i++){ // remove main from here , na let it stay
        keyword_hash = insert(keyword_hash,key_words[i],0);        
    }
}

void set_unset(){
    buf1_flag = buf2_flag;
    buf2_flag = buf1_flag;
    set_unset_flag=1;
}

token * gen_token(token *x ,char * tok_name,char* lex ){
    x = (token *)malloc(sizeof(struct token));
    x->line_no = lno;
    x->tok_name=(char*)malloc(sizeof(char)*70);
    x->lexeme = (char*)malloc(sizeof(char)*70);
    if(lex!=NULL)
        strcpy(x->lexeme,lex);
    strcpy(x->tok_name,tok_name);
    return x;
}

// strings must be null terminates for this to work 
char* keyword_name_gen(char *y){
    int i=0;
    if(y[0]=='.' || y[0]=='_'){
        int j=i;
        while(y[j]!='\0'){
            y[j]=y[j+1];
            j++;
        }   
    }

    while(y[i]!='\0'){
        if(isalpha(y[i])){
            y[i]=toupper(y[i]);
        }
        i++;
    }

    if(y[i-1]=='.'){
        y[i-1]='\0';
    }
    return y;
}

void chose_and_read(){
    if(buf1_flag){
        cur_char = buffer1[ptr];
    }
    else{
        cur_char = buffer2[ptr];  
    }
}

void init_file_lexer(char *x){
    fp = fopen(x,"r");
}
void close_file_lexer(){
    fclose(fp);
}
void init_lexer(char *z){
    init_keyword_hash();
    ptr=0;   lno=0;
    eof_flag=0;
    init_file_lexer(z);
    // printf("opened file \n");
    buffer1 = (char *)malloc((buffsize+1)*sizeof(char));
    buffer2 = (char *)malloc((buffsize+1)*sizeof(char));
    // // dollar terminating things here since the end of file will come as a '\0'
    buffer1[buffsize]='$';
    buffer2[buffsize]='$';
    // buffer1[buffsize]='\0';
    // buffer2[buffsize]='\0';

    /* Read the file into the buffer */
    fread(buffer1,buffsize*sizeof(char), 1, fp); /* Read 1 chunk of size bytes from fp into buffer */
    // printf("read bk\n");
    buf1_flag=1;
    buf2_flag=0;
    cur_char=buffer1[ptr];
    // printf("readbk\n");
}


// if(tok_cofirm==0){
    // we were reading from the previous buffer as well .
    // hence our left pointer is still in the previos buffer 
// }        

void get_next(){
    ptr++;
    
    //select the buffer
    chose_and_read();

    if(cur_char==0){
        // printf("lexing phase complete\n");
        eof_flag=1;
    }
    // rectify :  rectified now check

    // major flaw in this when your lptr is in another buffer and bychance you retract back into this buffer 
    // this function would read from the buffer again which is totally wrong

    if(cur_char=='$' && ptr==1024){  /// you need to reload the other buffer and read again 
        if(buf1_flag){
            
            if(retract_buffer_flag==0)
                fread(buffer2,buffsize*sizeof(char), 1, fp);
            else{
                retract_buffer_flag=0;
            }

            set_unset();
            ptr=0;
            chose_and_read();
        }        
        else{
            
            if(retract_buffer_flag==0)
                fread(buffer1,buffsize*sizeof(char), 1, fp);
            else{
                retract_buffer_flag=0;
            }

            set_unset();
            ptr=0;
            chose_and_read();
        }
    } 

    if(cur_char=='\n'){
        // printf("\n");
        lno++;
    }

}


void retract(){
    ptr--;
    // cur_char = buffer1[ptr];
    if(ptr<0){
        //shift to the previous buffer 
        if(set_unset_flag==0){
            // you can't retract
            printf("retract not possible here");
        }
        else{
            // needs a change here wrt to the todo in the above func 
            set_unset();
            ptr=buffsize-1;
            chose_and_read();
            retract_buffer_flag=1; // implies that the next buffer is already loaded so dont mess up
        }
        
    }
    else{
        chose_and_read();
    }

}

int variable_break(){
     // variable id ends onyl when you encounter ; _ \t \n , use this for a better code
    // the case of end followed by nothing by '\0' which is eof
    if(cur_char==' ' || cur_char==';' || cur_char=='\n' || cur_char=='\t' || 
        cur_char==','||cur_char==']'||cur_char==')'||cur_char==0 || cur_char=='='||cur_char=='(' || cur_char=='\0'||cur_char=='\r'
        || cur_char=='@'|| cur_char==']' || cur_char=='+'|| cur_char=='-' || cur_char=='/'||cur_char=='*'|| 
        cur_char=='.' || cur_char=='<' ||cur_char=='>'||cur_char=='\r'
    ){
        return 1;
    }
    else{
        return 0;
    }
}

int function_break(){
     // variable id ends onyl when you encounter ; _ \t \n , use this for a better code 
    if(cur_char==' ' || cur_char==';' || cur_char=='\n' || cur_char=='\t' || cur_char==',' || cur_char=='[' || cur_char=='('||
        cur_char=='\r' || cur_char==0 || cur_char=='@'|| cur_char==']' || cur_char=='+'|| cur_char=='-' || cur_char=='/'||cur_char=='*'|| 
        cur_char=='.' || cur_char=='<' ||cur_char=='>'||cur_char=='\r')
    {
        return 1;
    }
    else{
        return 0;
    }
}

int str_inst_break(){
    if(cur_char==' ' || cur_char==';' || cur_char=='\n' || cur_char=='\t' || cur_char==','){
        return 1;
    }
    else{
        return 0;
    }   
}

// set this at the start of the while loop // you have to change lptr evrytime you generate a token 

void set_lptr_buffer_val(){
    if(buf1_flag){
        l_ptr_in_buf = 1;
    }    
    else{
        l_ptr_in_buf = 2;
    }
}

// utility function to retrieve subtrings from the two buffers

char * get_substring(char *x,int l,int r) {
    // r will always be cur_char which is a global value
    int i=0;
    int j;

    if((l_ptr_in_buf==1 && buf1_flag)){

        for(i=l;i<=r;i++){
            x[i-l] = buffer1[i];
        }
        x[i-l]='\0';
    }
    else if((l_ptr_in_buf==2 && buf2_flag)){
        
        for(i=l;i<=r;i++){
            x[i-l] = buffer2[i];
        }   
        x[i-l]='\0';
    }
    else if(l_ptr_in_buf==1 && buf2_flag){

        for(i=l;i<buffsize;i++){
            x[i-l] = buffer1[i];
        }

        j=i-l;
        
        for(i=0;i<=r;i++,j++){
            x[j]=buffer2[i];
        }
        x[j]='\0';        
    }
    else if(l_ptr_in_buf==2 && buf1_flag){
        
        for(i=l;i<buffsize;i++){
            x[i-l] = buffer2[i];
        }
        
        j=i-l;
        
        for(i=0;i<=r;i++,j++){
            x[j]=buffer2[i];
        }
        x[j]='\0';
    }

    return x;
}

void  uncommentedfile(char *x){
    init_file_lexer(x); //dont forget to close this via close_file_lexer
    char c;
    while ((c = getc(fp)) && c != EOF){
        if(c!='#')
            putchar(c); 
        else{
            while ((c = getc(fp)) && c != EOF && c!='\n'){
                continue;
            }
        }
    }
    close_file_lexer();
}

token * get_next_token() {
    // printf("%s \n",buffer1);
    l_ptr =ptr; // pay careful attention to this one more time could lead to trouble
    r_ptr =ptr;
    set_lptr_buffer_val();
    // printf("ptr %d \n",ptr);
    // set a flag for lexical error
    // skip spaces dont generate anything for them keep this in mind
    // store keyword names in your linked list only that'll help
    // printf("%d\n",buffer1[ptr]);
    // printf("printed\n");
    if(eof_flag==1){
        token *z;
        z=gen_token(z,"eof","eof");
        return z;
    }
    else if(cur_char=='\n' || cur_char==' ' || cur_char=='\t' || cur_char=='\r'){
        get_next();
        return NULL;
    }
    else if(cur_char=='#'){
        do{
            get_next();
        }
        while(cur_char!='\n');
        token * z ;
        z=gen_token(z,"COMMENT","some_comment");
        // after generating token you need to place l_ptr
        // l_ptr = ptr;
        // r_ptr = ;
        //  send this token over 
        // printf("Comment token generated \n");
        return NULL;
    }
    
    else if(cur_char==';'){
        // gen_token();
        //  send this token over 
        token * z ;
        z=gen_token(z,"SEMICOLON",";");
        // printf("SEMICOLON token generated \n"); 
        get_next();
        return z;
    }
    // variable id ends onyl when you encounter ; _ \t \n , use this for a better code 
    else if(cur_char=='_'){
        // use for function id and _main
        token * z ;

        get_next();
        // printf("%c\n",cur_char);
        if(isalpha(cur_char)){
            while(isalnum(cur_char)){
                get_next();
            }
            
            if(function_break()==1){
                char* temp = (char *)malloc(sizeof(char)*(500));
                
                // latest update :  neeeded to do this since ptr-1 can cause a hell lot of problems with two buffers
                retract();
                temp= get_substring(temp,l_ptr,ptr);
                get_next();

                if(hfind(keyword_hash,temp)==1){
                    z = gen_token(z,"MAIN","_main");
                }
                else{
                    z = gen_token(z,"FUNID",temp);
                }
            }                
            else{
                // token * z ;
                printf("line no: %d Lexical error : unexpected termination function declatation\n",lno);
                //generate error token
                z=gen_token(z,"ERROR",NULL);
                return NULL;
            }
        }
        else{
            printf("line no: %d Lexical error :expected a alphabet character for function declaration \n",lno);
            z=gen_token(z,"ERROR",NULL);
            //generate error token
            return NULL;
        }
        return z;
    }
    // variable identifieer starts here 

    else if(isalpha(cur_char)){
        token * z ;
        int var_length = 1;
        //now l-ptr stays at the previos token ending or the start of this presumed token 
        get_next();
        while(isalpha(cur_char)){
            var_length++;
            get_next();
        }
        // printf("alp terminates %d \n",cur_char);
        // now this condition can break only on a couple things 
        // 1) this could end in a number in  which case move further ahead to check the second cndtn
        // 2) variable breakers  in which you should retract
        if(var_length>20){
            printf("line no: %d Lexical error : string name length exceeds 20 \n",lno);
            z=gen_token(z,"ERROR",NULL);
            //generate error token and return
            return NULL;
        }        

        
        if(isdigit(cur_char) && var_length<=19){
            get_next();
            // modified after update 
            if(variable_break()==1 || isdigit(cur_char)) {

                char * temp = (char *)malloc(sizeof(char)*(var_length+2));
                retract();
                temp = get_substring(temp,l_ptr,ptr);
                get_next();
                z=gen_token(z,"ID",temp);
                // generate token and return id
                
            }
            else{
                printf("line no: %d Lexical error :error in string name improper var identifier termination \n",lno);
                // generate token and return
                z=gen_token(z,"ERROR",NULL);
                return NULL;
            }
        }
        else{
            if(variable_break()==1){
                
                char * temp = (char *)malloc(sizeof(char)*(var_length+1));
                // temp[var_length]='\0';
                // int k=0;
                retract();
                temp = get_substring(temp,l_ptr,ptr);
                get_next();

                if(hfind(keyword_hash,temp)==1){
                    // generate keyword token
                    z=gen_token(z,keyword_name_gen(temp),temp);    
                }
                else{
                    z=gen_token(z,"ID",temp);           
                }
                // generate token from l_ptr
                // retract();   
            }
            else{
                printf("line no: %d Lexical error :error in string name improper var identifier termination \n",lno);
                // generate token and return
                z=gen_token(z,"ERROR",NULL);
                return NULL;
                // return z;    
            }
        }
        return z;
    }
    else if(cur_char=='"'){
        token*z;
        int var_length = 0;
        // i missed a get_next() here geez man
        get_next();
        while(islower(cur_char) || cur_char==' '){

            get_next();
            var_length++;                
        }
        // printf("cur_charrent string instance elem %c\n",cur_char);
        if(cur_char!='"'){
            if(isupper(cur_char)){
                printf("line no: %d Lexical error : string instance does not allow uppercase chars\n",lno);                    
                z=gen_token(z,"ERROR",NULL);
                return NULL;    
            }
            else{
                printf(" line no: %d Lexical error string instance does not terminate with  double quotes \n",lno);
                z=gen_token(z,"ERROR",NULL);
                return NULL;
            }
            // generate error token 
            // retract();
            return z;
        }
        if(var_length>20){
            printf("line no: %d Lexical error : in str instance length on line no\n",lno);
            //generate error token and return
            z=gen_token(z,"ERROR",NULL);
            return NULL;
        }

        if(cur_char=='"'){
            // generate token for the string and now you have to do a get_next() to increment the pointer

            char * temp = (char *)malloc(sizeof(char)*(500));

            temp = get_substring(temp,l_ptr,ptr);
            // printf("strign instance %s \n",temp);
            z=gen_token(z,"STR",temp);

            get_next();
            return z;    
        }
    }
    else if(isdigit(cur_char)){

        token*z;
        
        get_next();
        while(isdigit(cur_char)){
            get_next();
        }
        // the above cndtn should terminate in two conditions 
        // either it could be a decimal 
        if(cur_char=='.'){
            get_next(); 
            
            if(isdigit(cur_char)){ // get first digit 
                
                int dec_len=1;
                get_next(); 
                
                while(isdigit(cur_char)){ // check second digit
                    dec_len++;
                    if(dec_len==3){
                        char * temp = (char *)malloc(sizeof(char)*(500));
                        retract();
                        temp = get_substring(temp,l_ptr,ptr);
                        get_next();
                        z=gen_token(z,"RNUM",temp);
                        return z;
                        break;
                    }
                    get_next();
                }
                
                // if(dec_len>2){
                //     printf("improper real number declaration too many digits after decimal on line no %d \n",lno);
                //     // generate token and return since the loop broke because it encountered something other
                //     // than a digit we dont need to do a get_next() , requoire de mai
                //     z=gen_token(z,"ERROR",NULL); 
                //     return z;
                // }
                if(variable_break()==1){
                    
                    char * temp = (char *)malloc(sizeof(char)*(500));
                    retract();
                    temp = get_substring(temp,l_ptr,ptr);
                    get_next();
                    // generate token for RNUM and return 
                    //extract lexeme by retract get_substring and get_next idiom
                    z=gen_token(z,"RNUM",temp);
                    return z;
                }
                
            }
            else{
                printf("line no: %d Lexical error : expected decimal values\n",lno);
                z=gen_token(z,"ERROR",NULL); 
                return NULL;
            }
            
        }
        else if(variable_break()==1){
            // generate token for NUM and return
            char * temp = (char *)malloc(sizeof(char)*(500));
            retract();
            temp = get_substring(temp,l_ptr,ptr);
            get_next();            
            z=gen_token(z,"NUM",temp);
            return z;                
        }
        else{
                
            printf("line no: %d Lexical error :improper termination for num on line no\n",lno);
            z=gen_token(z,"ERROR",NULL); 
            return NULL;
            // retract();
            // generate error token
        }
    }
    else if(cur_char=='['){
        token *z;
        get_next();
        //generate SQO token
        z=gen_token(z,"SQO","["); 
        return z;
    }
    else if(cur_char==']'){
        token *z;
        get_next();
        //generate SQC token
        z=gen_token(z,"SQC","]"); 
        return z;
    }
    else if(cur_char=='('){
        token *z;
        get_next();
        //generate OP token
        z=gen_token(z,"OP","("); 
        return z;
    }
    else if(cur_char==')'){
        token *z;
        get_next();
        //generate CL token
        z=gen_token(z,"CL",")"); 
        return z;
    }
    else if(cur_char==','){
        token *z;
        get_next();
        //generate comma token
        z=gen_token(z,"COMMA",","); 
        return z;
    }
    else if(cur_char=='+'){
        token *z;
        get_next();
        //generate PLUS token
        z=gen_token(z,"PLUS","+"); 
        return z;

    }
    else if(cur_char=='-'){
        token *z;
        get_next();
        // generate MINUS token
        z=gen_token(z,"MINUS","-"); 
        return z;
    }
    else if(cur_char=='*'){
        token *z;
        get_next();
        // generate toekn MUL
        z=gen_token(z,"MUL","*"); 
        return z;
    }
    else if(cur_char=='/'){
        token *z;
        get_next();
        // generate token DIV
        z=gen_token(z,"DIV","/"); 
        return z;

    }
    else if(cur_char=='@'){
        token *z;
        get_next();
        // geenrate size token
        z=gen_token(z,"SIZE","@"); 
        return z;

    }
    else if(cur_char=='.'){
        token *z;
        int var_length=1;
        get_next();

        while(islower(cur_char)){
            get_next();
            var_length++;
        }

        if(cur_char=='.'){

            char * temp = (char *)malloc(sizeof(char)*(var_length+2));
            temp=get_substring(temp,l_ptr,ptr);

            if(hfind(keyword_hash,temp)==1){
                // printf("logical keyword found\n");
                get_next();
                //generate_token and return
                z=gen_token(z,keyword_name_gen(temp),temp); 
                return z;
            }
            else{
                printf("line no: %d Lexical error :no such logical keyword exists\n",lno);
                get_next();
                z=gen_token(z,"ERROR",NULL); 
                return NULL;
                // generate error token and return 
            }
        
        }
        else{
            printf("line no: %d Lexical error : expected a logical operator after dot, no dot termination however none found\n",lno);
            //generate error token
            // retract(); you're already  onto the next pointer remember this while returning the lexeme value
            z=gen_token(z,"ERROR",NULL); 
            return NULL;

        }
    }
    else if(cur_char=='<'){
        token *z;
        get_next();
        if(cur_char=='='){
            get_next();
            // generate LE  token and return 
            z=gen_token(z,"LE","<="); 
            return z;
        }
        else{
            // retract(); dont need it here careful with the lexeme value return
            //generate LT token
            z=gen_token(z,"LT","<"); 
            return z;
        }
    }
    else if(cur_char=='>'){
        token *z;
        get_next();
        if(cur_char=='='){
            get_next();
            // generate GE  token and return 
            z=gen_token(z,"GE",">="); 
            return z;            
        }
        else{
            // retract(); // you dont need a retract here careful with the lexeme value return 
            //generate GT token
            z=gen_token(z,"GT",">"); 
            return z;            

        }   
    }
    else if(cur_char=='='){
        token *z;
        get_next();
        if(cur_char=='=' || cur_char=='/'){
            if(cur_char=='='){
                // generate not equal to token NE relation op
                get_next();
                z=gen_token(z,"EQ","=="); 
                return z;            
            }
            else{
                // generate EQ token
                // retract() no needto retract careful with the lexeme value return;
                get_next();
                if(cur_char=='='){
                    get_next();
                    z=gen_token(z,"NE","=/="); 
                    return z;            
                }
                else{
                    retract();
                    retract();
                    z=gen_token(z,"ASSIGNOP","="); 
                    get_next();
                    return z;    
                }
            }
        }
        else{
            z=gen_token(z,"ASSIGNOP","="); 
            return z;                             
        }
    }
    else {
        printf("line no: %d Lexical error unknown symbol ascii code %d \n",lno,cur_char);
        get_next();
        return NULL;
    }
}

/*int main(){

    init_lexer();
    token* x ;
    int i=0;

    x =get_next_token();
    printf(" %s ",x->tok_name);    

    while(eof_flag==0){
        x = get_next_token();        
        if(x==NULL){
            continue;
            printf("boom");
        }
        else{
             printf(" %s ",x->tok_name);
        }
    }
    printf("lexing phase complete\n");
}*/
/*    // checking hash 
    int res = hfind(keyword_hash,"main");
    printf("%d\n",res);
*/

    /* Open your_file in read-only mode */
    // fp = fopen("try.txt","r");

    /* Get the buffer size */
    // fseek(fp, 0, SEEK_END); /* Go to end of file */
    // size = ftell(fp); /* How many bytes did we pass ? */
    /* Set position of stream to the beginning */
    // rewind(fp);

    // initialize at the beginning of the parser function
     // Allocate the buffer (no need to initialize it with calloc) 
    

    //  NULL-terminate the buffer 
    // /* buffer1[size] = '\0'; */

    // /* cur_charrent character read   */
    

    // use this at the beginning of the function instead of the while loop : done
    // get_next();

    // declared globally
    // int l_ptr; // this indicates the position of the left pointer  change the position asa tok gen
    // int r_ptr ;= cur_char;  

    // you'll need to convert this to a func
    // while(cur_char!=0){
        
    // get_next(); instead of this trying setting cur_char initially to the first value of the stream maybe

    // extremely inmportant specify a function that initializes the value of ptr and cur_char as well 
    // and also sets flags accordingly

    
    /* Print it ! */
    // printf("%s\n", buffer);

// }*/
