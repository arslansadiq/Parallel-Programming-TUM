#include <iostream>
#include <cstdlib>
#include <omp.h>

#include "laplace.h"

#define N 10
#define ITER 1000 

int main(int argc, char **argv)
{
    //sequential
    double A[N + 2][N + 2];
    double B[N + 2][N + 2];

    initialize_ref<N>(A,B);

    // boundary conditions
    set_boundaries<N>(A,B);

    for (int n = 0; n < ITER; n++)
    {
        time_step_ref<N>(A,B,n);
    }

    //parallel
    omp_set_num_threads(32);
    double C[N + 2][N + 2];
    double D[N + 2][N + 2];

    //initialize
    initialize<N>(C,D);

    // boundary conditions
    set_boundaries<N>(C,D);

    // Iteration
    for (int n = 0; n < ITER; n++)
    {
        time_step<N>(C,D,n);
    }
    
    for (int i = 0; i < N+2; i++) {
       for (int j= 0; j < N+2; j++) {
		if (A[i][j] !=  C[i][j]){
                    std::cerr <<  "Computation with 32 threads failed\n";
                    std::cerr <<  "Result differs from the one produced by the sequential version\n";
                    return 1;
                }
       }
    }

    //print<N>(A);
    //print<N>(C);

    return 0;
}
