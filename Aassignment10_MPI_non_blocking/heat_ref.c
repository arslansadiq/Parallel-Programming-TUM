#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "heat.h"
#include "helper.h"

double jacobi_ref(double *h_new, double *h_old, int niters, int energy_intensity, int n, int iter_energy,  const int nsources, int sources[nsources][2], int rank, int size, int px, int py, MPI_Comm comm, int output)
{
    h_old = (double *)calloc(1, (n + 2) * (n + 2) * sizeof(double)); // extended with halos of width 1
    h_new = (double *)calloc(1, (n + 2) * (n + 2) * sizeof(double)); // extended with halos of width 1
    double *tmp;

    for (int iter = 0; iter < niters; ++iter)
    {
      for (int j = 1; j < n + 1; ++j)
      {
        for (int i = 1; i < n + 1; ++i)
        {
          h_new[map(i, j ,n+2)] = h_old[map(i, j, n+2)] / 2.0 + (h_old[map(i - 1, j, n+2)] + h_old[map(i + 1, j, n+2)] + h_old[map(i, j - 1, n+2)] + h_old[map(i, j + 1, n+2)]) / 4.0 / 2.0;
        }
      }
      if (iter < iter_energy)
      {
        for (int i = 0; i < nsources; ++i)
        {
          h_new[map(sources[i][0], sources[i][1], n+2)] += energy_intensity; // heat rate
        }
      }
      tmp = h_new; // swap arrays
      h_new = h_old;
      h_old = tmp;
    }
    if (output) printarr(h_new, n, rank);

    return calculate_total_heat(h_new, n);
}
