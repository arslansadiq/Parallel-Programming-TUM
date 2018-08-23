#include "familytree.h"

void traverse(tree *node, int numThreads){

	if(node != NULL){
		node->IQ = compute_IQ(node->data);
		genius[node->id] = node->IQ;
		
		traverse(node->right, numThreads);
		traverse(node->left, numThreads);
	}
}

