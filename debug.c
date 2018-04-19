#include <stdio.h>
#include <stdlib.h>
typedef struct _node{
    int a;
    struct _node * next;
} node ;
void change(node *x){
    node *y=x;
    y->next->a=3;
}
int main(){
    node *x =  (node*)malloc(sizeof(node));
    x->a=1;
    x->next = (node*)malloc(sizeof(node));
    x->next->a =2;
    change(x);
    printf("%d",x->next->a);
}