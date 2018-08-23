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
  int n, energy_intensity, niters, iter_energy, px, py, output;

  // argument checking
  if(argc < 8) {
      if(!rank) printf("usage: program <n> <energy_intensity> <niters> <iter_energy> <px> <py> <output_flag>\n");
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
      output=atoi(argv[7]); // output flag

      if(px * py != size) MPI_Abort(comm, 1);// abort if px or py are wrong
      if(n % py != 0) MPI_Abort(comm, 2); // abort; px needs to divide n
      if(n % px != 0) MPI_Abort(comm, 3); // abort; py needs to divide n

      // broadcast arguments
      int args[7] = {n, energy_intensity, niters, iter_energy, px, py, output};
      MPI_Bcast(args, 7, MPI_INT, 0, comm);
  }
  else {
      int args[7];
      MPI_Bcast(args, 7, MPI_INT, 0, comm);
      n=args[0]; energy_intensity=args[1]; niters=args[2]; iter_energy=args[3]; px=args[4]; py=args[5], output=args[6];
  }

  const int nsources = 4;
  int sources [nsources][2];
  init_sources(nsources, sources, n);

  double *h_old = NULL;
  double *h_new = NULL;

  double t = -MPI_Wtime();
  jacobi(h_new, h_old, niters, energy_intensity, n, iter_energy, nsources, sources, rank, size, px, py, comm, output);
  t += MPI_Wtime();

  if (rank==0) printf("\nTime: %.3lf seconds\n", t); 

  MPI_Finalize();

  return 0;
}
