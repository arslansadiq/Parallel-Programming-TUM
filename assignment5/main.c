#include "ds.h"
#include "familytree.h"
#include "vis.h"
#include "time.h"
// Random names to make possible identification of the family members
// 5 generations

int main(int argc, char** argv){
  long int num_threads = 4;
  struct timespec begin, end;

  // argument handling
	if (argc != 2) {
					fprintf(stderr, "usage: %s [#threads]\n", argv[0]);
    return 1;
  }
  if ((num_threads = strtol(argv[1], NULL, 0)) == 0 || num_threads < 0) {
  	fprintf(stderr, "#threads not valid!\n");
    return 1;
	}

	// insert nodes (members) in the family tree
  tree *topNode = malloc(sizeof(tree));
  initialize(topNode);


  // visit all of them to compute an IQ in a misterious way
  clock_gettime(CLOCK_MONOTONIC, &begin);
  traverse(topNode, num_threads);
  clock_gettime(CLOCK_MONOTONIC, &end);
  
  // Showing results
  visualize();

  tearDown(topNode);
  
  // print timing information
  printf("\n\nTime: %.5f seconds\n", ((double)end.tv_sec + 1.0e-9*end.tv_nsec) -
                     ((double)begin.tv_sec + 1.0e-9*begin.tv_nsec));
	return 0;
}

