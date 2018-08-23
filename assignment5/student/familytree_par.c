#include "familytree.h"
#include <omp.h>

void parallel_traverse(tree *node) {
	if (node != NULL) {

 	  #pragma omp task
	  parallel_traverse(node->right);
          parallel_traverse(node->left);

          node->IQ = compute_IQ(node->data);
 	  genius[node->id] = node->IQ;
  }
}

void traverse(tree *node, int numThreads){
	#pragma omp parallel num_threads( numThreads )
	{
		#pragma omp single
		parallel_traverse(node);
	}
}

