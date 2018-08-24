#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "heat.h"

void printarr(double *a, int n, int rank);
double calculate_total_heat(double *h_new, int n);
double calculate_total_heat_omp(double *h_new, int n, int num_threads);
#endif
