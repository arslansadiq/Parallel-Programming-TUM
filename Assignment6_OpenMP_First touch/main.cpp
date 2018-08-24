#include <iostream>
#include <cstdlib>
#include <omp.h>

#include "laplace.h"

#define N 256
#define ITER 100

int main(int argc, char **argv)
{
    double A[N + 2][N + 2];
    double B[N + 2][N + 2];

    double starttime, endtime;
    int threads;

    //initialize
    initialize<N>(A,B);

    // boundary conditions
    set_boundaries<N>(A,B);

    starttime = omp_get_wtime();
    // Iteration
    for (int n = 0; n < ITER; n++)
    {
        time_step<N>(A,B,n);
    }
    endtime = omp_get_wtime();

    //print<N>(A);

    std::cout << "Time: " << endtime - starttime << " seconds\n";
    return 0;
}
