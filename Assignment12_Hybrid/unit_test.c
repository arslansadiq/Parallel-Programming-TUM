#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "heat.h"
#include "helper.h"

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
    int rank,size;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    int n, energy_intensity, niters, iter_energy, px, py, num_threads, output;

    // argument checking
    if(argc < 8) {
        if(!rank) printf("usage: program <n> <energy_intensity> <niters> <iter_energy> <px> <py> <num_threads> <output_flag>\n");
        MPI_Finalize();
        exit(1);
    }

    if (rank==0) {
        n = atoi(argv[1]);                    // grid size nxn
        energy_intensity = atoi(argv[2]);     // energy intensity
        niters = atoi(argv[3]);               // number of iterations
        iter_energy = atoi(argv[4]);          // number of iterations with energy exposure
        px=atoi(argv[5]); // 1st dim process topolgy
        py=atoi(argv[6]); // 2nd dim process topolgy
        num_threads=atoi(argv[7]); // 2nd dim process topolgy
        output=atoi(argv[8]); // output flag

        if(px * py != size) MPI_Abort(comm, 1);// abort if px or py are wrong
        if(n % py != 0) MPI_Abort(comm, 2); // abort; px needs to divide n
        if(n % px != 0) MPI_Abort(comm, 3); // abort; py needs to divide n

        // broadcast arguments
        int args[8] = {n, energy_intensity, niters, iter_energy, px, py, num_threads, output};
        MPI_Bcast(args, 8, MPI_INT, 0, comm);
    }
    else {
        int args[8];
        MPI_Bcast(args, 8, MPI_INT, 0, comm);
        n=args[0]; energy_intensity=args[1]; niters=args[2]; iter_energy=args[3]; px=args[4]; py=args[5]; num_threads=args[6]; output=args[7];
    }

    const int nsources = 4;
    int sources [nsources][2];
    init_sources(nsources, sources, n);

    double heat_ref=0;
    if (rank == 0){
        double *h_old_seq = NULL;
        double *h_new_seq = NULL;
        heat_ref = jacobi_ref(h_new_seq, h_old_seq, niters, energy_intensity, n, iter_energy, nsources, sources, rank, size, px, py, num_threads, comm, output);
    }

    MPI_Barrier(comm);
    double *h_old_par = NULL;
    double *h_new_par = NULL;
    double heat_partial = jacobi(h_new_par, h_old_par, niters, energy_intensity, n, iter_energy, nsources, sources, rank, size, px, py, num_threads, comm, output);

    double heat;
    MPI_Reduce(&heat_partial,&heat,1,MPI_DOUBLE,MPI_SUM,0,comm);

    if(rank==0)
        if ( (heat-heat_ref) >0.00001){
            MPI_Abort(comm,1);
        }

    MPI_Finalize();
    return 0;
}
