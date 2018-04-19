#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int a;
}node ;

int main(){
    node * x= (node*)malloc(sizeof(node));
    x->a =10;
    node * y = x;
    free(y);
    free(x);
    printf("%p",y);
}