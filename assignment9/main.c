#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <mpi.h>
#include "helper.h"
#include "reverse.h"

int main(int argc, char** argv)
{
    int np, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char *str = NULL; 
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

    struct timespec start, stop;


    if (rank == 0) {
        printf("Before: %s\n", str);
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    reverse(str, str_len);
    clock_gettime(CLOCK_MONOTONIC, &stop);

    if (rank == 0) {
        printf("After : %s\n", str);
        printf("Time: %lf seconds\n", time_diff(&start, &stop, NULL));
    }

    MPI_Finalize();

    return 0;
}
