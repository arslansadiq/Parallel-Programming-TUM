#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include "helper.h"
#include "reverse.h"


int isEqual(char* a_seq, char* a_par, int length) {
	for (int i = 0; i < length; i++) {
		if ( a_seq[i] != a_par[i])
			return 0;
	}
	return 1;
}


int main(int argc, char** argv) {

	int np, rank;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char *str_ref = NULL, *str = NULL;
	int str_len = strlen(argv[1]);

	if (rank == 0)
	{
		if (argc == 2 && str_len >= np)
		{
			str = strdup(argv[1]);
		}
		else
		{
			fprintf(stderr, "Usage: %s \"This is a simple string that should be printed in reverse order\"\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	int exit_status;

	if (rank == 0) printf("Rank: %d Before: %s\n", rank, str);

	reverse(str, str_len);

	if (rank == 0) printf("Rank: %d After: %s\n", rank, str);

	str_ref = strdup(argv[1]);

	reverse_str(str_ref, str_len);

	if (rank == 0) {
		if (isEqual(str_ref, str, str_len))
		{
			exit_status = EXIT_SUCCESS;
			printf("Unit test completed successfully.\n");
		}
		else {
			fprintf(stderr, "ERROR: Sequential implementation and parallel implementation produce different results.\n");
			exit_status = EXIT_FAILURE;
		}
	}
	else
	{
		MPI_Finalize();
		return EXIT_SUCCESS;

	}

	MPI_Finalize();
	printf("%d\n", exit_status);
	return exit_status;
}
