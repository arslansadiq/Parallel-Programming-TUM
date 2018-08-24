#include "ds.h"
#include "familytree.h"
#include "familytree_ref.h"
#include "vis.h"
// Random names to make possible identification of the family members
// 5 generations

int main(){

	int ind, failed=0;
	int genius_ref[NUMNODES];

	// run the sequential version
	printf("running reference sequential code\n"); 
	tree *node_ref = malloc(sizeof(tree));
   initialize(node_ref);
	traverse_ref(node_ref);
	tearDown(node_ref);

	for (ind=0; ind<NUMNODES; ind++){
		genius_ref[ind]=genius[ind];
		genius[ind]=0;
	}
	
	printf("running the parallel code\n"); 
	// run the parallel version
	tree *node = malloc(sizeof(tree));
   initialize(node);
  	traverse(node, 4);
	tearDown(node);

	for (ind=0; ind<NUMNODES; ind++){
		if (genius_ref[ind]!=genius[ind]){
			failed=1;
			break;
		}
	}

	if (failed == 1){
		fprintf(stderr, "Wrong parallel result for %d: %s -> %d (correct: %d)\n",
                                ind, names[ind], genius[ind], genius_ref[ind]);
		printf("YOUR CODE FAILED THE UNIT TEST\n");
	}else{
		printf("YOUR CODE PASSED THE UNIT TEST\n");
	}
}

