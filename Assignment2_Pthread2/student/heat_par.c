#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include "heat.h"
#include "helper.h"

double jacobi(double *h_new, double *h_old, int niters, int energy_intensity, int n, int iter_energy, const int nsources, int sources[nsources][2], int rank, int size, int px, int py, int num_threads, MPI_Comm comm, int output)
{
    // determine my coordinates (x,y) -- r=x*a+y in the 2d processor array
    int rx = rank % px;
    int ry = rank / px;
    // determine my four neighbors
    int north = (ry - 1) * px + rx;
    if (ry - 1 < 0)
        north = MPI_PROC_NULL;
    int south = (ry + 1) * px + rx;
    if (ry + 1 >= py)
        south = MPI_PROC_NULL;
    int west = ry * px + rx - 1;
    if (rx - 1 < 0)
        west = MPI_PROC_NULL;
    int east = ry * px + rx + 1;
    if (rx + 1 >= px)
        east = MPI_PROC_NULL;
    // decompose the domain
    int bx = n / px;    // block size in x
    int by = n / py;    // block size in y
    int offx = rx * bx; // offset in x
    int offy = ry * by; // offset in y

    h_old = (double *)calloc(1, (bx + 2) * (by + 2) * sizeof(double)); // extended with halos of width 1
    h_new = (double *)calloc(1, (bx + 2) * (by + 2) * sizeof(double)); // extended with halos of width 1
    double *tmp;

    int locnsources = 0;         // number of sources in my area
    int locsources[nsources][2]; // sources local to my rank
    for (int i = 0; i < nsources; ++i)
    { // determine which sources are in my patch
        int locx = sources[i][0] - offx;
        int locy = sources[i][1] - offy;
        if (locx >= 0 && locx < bx && locy >= 0 && locy < by)
        {
            locsources[locnsources][0] = locx + 1; // offset by halo zone
            locsources[locnsources][1] = locy + 1; // offset by halo zone
            locnsources++;
        }
    }

    // allocate communication buffers
    double *sbufnorth = (double *)calloc(1, bx * sizeof(double)); // send buffers
    double *sbufsouth = (double *)calloc(1, bx * sizeof(double));
    double *sbufeast = (double *)calloc(1, by * sizeof(double));
    double *sbufwest = (double *)calloc(1, by * sizeof(double));
    double *rbufnorth = (double *)calloc(1, bx * sizeof(double)); // receive buffers
    double *rbufsouth = (double *)calloc(1, bx * sizeof(double));
    double *rbufeast = (double *)calloc(1, by * sizeof(double));
    double *rbufwest = (double *)calloc(1, by * sizeof(double));

    for (int iter = 0; iter < niters; ++iter)
    {
        MPI_Request reqs[8];
        for (int i = 0; i < bx; ++i)
            sbufnorth[i] = h_old[map(i + 1, 1, bx + 2)]; // pack loop - last valid region
        for (int i = 0; i < bx; ++i)
            sbufsouth[i] = h_old[map(i + 1, by, bx + 2)]; // pack loop
        for (int i = 0; i < by; ++i)
            sbufeast[i] = h_old[map(bx, i + 1, bx + 2)]; // pack loop
        for (int i = 0; i < by; ++i)
            sbufwest[i] = h_old[map(1, i + 1, bx + 2)]; // pack loop
        MPI_Isend(sbufnorth, bx, MPI_DOUBLE, north, 9, comm, &reqs[0]);
        MPI_Isend(sbufsouth, bx, MPI_DOUBLE, south, 9, comm, &reqs[1]);
        MPI_Isend(sbufeast, by, MPI_DOUBLE, east, 9, comm, &reqs[2]);
        MPI_Isend(sbufwest, by, MPI_DOUBLE, west, 9, comm, &reqs[3]);
        MPI_Irecv(rbufnorth, bx, MPI_DOUBLE, north, 9, comm, &reqs[4]);
        MPI_Irecv(rbufsouth, bx, MPI_DOUBLE, south, 9, comm, &reqs[5]);
        MPI_Irecv(rbufeast, by, MPI_DOUBLE, east, 9, comm, &reqs[6]);
        MPI_Irecv(rbufwest, by, MPI_DOUBLE, west, 9, comm, &reqs[7]);

// update inner grid points
        #pragma omp parallel for num_threads(num_threads)
        for (int j = 2; j < by; ++j)
            for (int i = 2; i < bx; ++i)
                h_new[map(i, j, bx + 2)] = h_old[map(i, j, bx + 2)] / 2.0 + (h_old[map(i - 1, j, bx + 2)] + h_old[map(i + 1, j, bx + 2)] + h_old[map(i, j - 1, bx + 2)] + h_old[map(i, j + 1, bx + 2)]) / 4.0 / 2.0;

        // communication has to be finished at this point
        MPI_Waitall(8, reqs, MPI_STATUSES_IGNORE);
        for (int i = 0; i < bx; ++i)
            h_old[map(i + 1, 0, bx + 2)] = rbufnorth[i]; // unpack loop - into ghost cells
        for (int i = 0; i < bx; ++i)
            h_old[map(i + 1, by + 1, bx + 2)] = rbufsouth[i]; // unpack loop
        for (int i = 0; i < by; ++i)
            h_old[map(bx + 1, i + 1, bx + 2)] = rbufeast[i]; // unpack loop
        for (int i = 0; i < by; ++i)
            h_old[map(0, i + 1, bx + 2)] = rbufwest[i]; // unpack loop

        // north, south
        for (int j = 1; j < by + 1; j += by - 1)
            for (int i = 2; i < bx; ++i)
                h_new[map(i, j, bx + 2)] = h_old[map(i, j, bx + 2)] / 2.0 + (h_old[map(i - 1, j, bx + 2)] + h_old[map(i + 1, j, bx + 2)] + h_old[map(i, j - 1, bx + 2)] + h_old[map(i, j + 1, bx + 2)]) / 4.0 / 2.0;

        // east, west
        for (int j = 1; j < by + 1; ++j)
            for (int i = 1; i < bx + 1; i += bx - 1)
                h_new[map(i, j, bx + 2)] = h_old[map(i, j, bx + 2)] / 2.0 + (h_old[map(i - 1, j, bx + 2)] + h_old[map(i + 1, j, bx + 2)] + h_old[map(i, j - 1, bx + 2)] + h_old[map(i, j + 1, bx + 2)]) / 4.0 / 2.0;

        if (iter < iter_energy)
            for (int i = 0; i < locnsources; ++i)
                h_new[map(locsources[i][0], locsources[i][1], bx + 2)] += energy_intensity; // heat rate

        tmp = h_new;
        h_new = h_old;
        h_old = tmp;
    }
    if (output)
        printarr(h_new, bx, rank);

    MPI_Barrier(comm);
    return calculate_total_heat_omp(h_new, bx, num_threads);
}
