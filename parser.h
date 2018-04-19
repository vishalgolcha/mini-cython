///////////////////////////////////////
// Author: Vishal Golcha 
// Id: 2014B5A70717P
///////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "parserDef.h"

extern char * symb_name[100] ;
extern char *rule[100];
extern char *rule_split[100][100];
extern long long int first_sets[100][2];
extern int follow_checker[100];
extern long long int follow_sets[100];
extern int follow_matrix[100][100];
extern int parse_table[100][100];
extern rules *rule_adt;


void traverse(rnode x );
int find_sno(char *x);

long long int set(long long int x,long long int b);
long long int unset(long long int x,long long int b);
void traverse_and_compute_first(rules x,int idx);
void copy_layer();
int check_if_equal();
void traverse_and_compute_follow(rules x);
void traverse_and_populate_table(rules x, int rule_no);
void populate_rules();
void populate_first_sets();
void populate_follow_sets();
void populate_parse_table();
void init_grammar();

extern tnode* ptree;
