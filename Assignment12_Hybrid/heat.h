#ifndef HEAT_H
#define HEAT_H

#include <stdio.h>
#include <math.h>
#include <mpi.h>

inline
int map(const int i,const int j,const int n){return j*n+i;}

void init_sources(const int nsources , int sources [nsources][2], int n);
double jacobi(double *h_new, double *h_old, int niters, int energy_intensity, int n, int iter_energy,  const int nsources, int sources[nsources][2], int rank, int size, int px, int py, int num_threads, MPI_Comm comm, int output);
double jacobi_ref(double *h_new, double *h_old, int niters, int energy_intensity, int n, int iter_energy,  const int nsources, int sources[nsources][2], int rank, int size, int px, int py, int num_threads, MPI_Comm comm, int output);

#endif
