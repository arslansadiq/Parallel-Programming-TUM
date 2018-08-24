#include "familytree.h"
#include <string.h>
#include <stdlib.h>

void fill(tree *node, int level){
	  
	node->left = NULL;
	node->right = NULL;

   node->id = g_node_id;
	sprintf(node->name, "%s%d", names[g_node_id % 31], g_node_id/31);
	node->data = (123*(g_node_id + node->name[1] * g_node_id / 31)) % 9113; 
	node->IQ = 0;
	g_node_id++;

	if (level < MAXLEVEL-1) {
		node->right = malloc(sizeof(tree));
		node->left = malloc(sizeof(tree));
		fill(node->right, level+1);
		fill(node->left, level+1);
	}
}

void initialize(tree *node) {
	g_node_id = 0;
	fill(node, 0);
}

void tearDown(tree* node) {
	if (node != NULL) {
		if (node->left != NULL) tearDown(node->left);
		if (node->right != NULL) tearDown(node->right);
		free(node);
	}
}

int is_prime(int n){

	int i, flag = 1;

  for(i=2; i<=n/2; ++i){
    if(n%i==0){
      flag=0;
      break;
    }
  }

	return flag;
}

int compute_IQ(int data){

	// nothing meaningfull, just a function that is kind of computationally expensive
	// such that parallelising makes sense :)

  int i; int sum = 0;
	for(i = 0; i < data; i++){
		if(is_prime(i))
			sum++;
	}
	
  return 70 + (sum % 100)*(sum % 100)*(sum % 100)*(sum % 100)/1000000;
}
