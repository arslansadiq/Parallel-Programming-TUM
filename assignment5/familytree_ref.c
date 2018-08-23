#include "familytree_ref.h"

void traverse_ref(tree *node){

	if(node != NULL){
		node->IQ = compute_IQ(node->data);
		genius[node->id] = node->IQ;
		
		//potential tasks and sessions will take place here
		traverse_ref(node->right);
		traverse_ref(node->left);
	}
}

