#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "helper.h"

void reverse(char *str, int strlen)
{
    // get the rank and number of MPI processes
    int np, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int len, idx, map, len_r, idx_r, map_r;

    int n_extra = strlen%np;
    len = strlen/np + (rank<n_extra);
    idx = rank*len + n_extra*(rank>=n_extra);
    //Alocate memory
    char* temp = (char*) malloc(len*sizeof(char));

    // Sending from rank 0
    if (rank==0){
        // send the data to other ranks
        for (int r = 1; r < np; r++){
            len_r = strlen/np + (r<n_extra);
            idx_r = r*len_r + n_extra*(r>=n_extra);
            MPI_Send(&str[idx_r], len_r, MPI_CHAR, r, 0, MPI_COMM_WORLD);
        }

        // copy data to a local temporary buffer
        memcpy(temp, str, len*sizeof(char));

    }
    
    else MPI_Recv(temp, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    // Reverse local strings in temp
    reverse_str(temp, len);

    // Send back reverted strings to rank 0
    if (rank!=0){
        MPI_Send(temp, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else{
        //receive the data back in rank 0
        for (int r = 1; r<np; r++) {
            len_r = strlen/np + (r<n_extra);
            idx_r = r*len_r + n_extra*(r>=n_extra);
            map_r = strlen - (idx_r + len_r - 1) - 1;
            MPI_Recv(&str[map_r], len_r, MPI_CHAR, r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        
        //copy the reverted string chunk back to the appropriate location in rank 0
        map = strlen - (idx + len - 1) - 1;
        memcpy(str+map, temp, len*sizeof(char));
    }
}
