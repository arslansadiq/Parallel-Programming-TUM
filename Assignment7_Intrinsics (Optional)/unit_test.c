#include "timer.h"
#include "time.h"
#include "dgemm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
    int n, i, j, k, mem_size;
    float *a, *b, *c, *d;
      
    if(argc > 1){
        n = atoi(argv[1]);
        if (n%8!=0){
            printf("N must be divisible by 8!\n");
            return -1;
        }
        printf("Array size defined as: %d\n", n);
    }
    else{
        n = 256;
        printf("Array size was not defined and specified as: %d\n", n);
    }

    // mem allocations
    mem_size = n * n * sizeof(float);
    a = (float*)malloc(mem_size);
    b = (float*)malloc(mem_size);
    c = (float*)malloc(mem_size);
    d = (float*)malloc(mem_size);

    // check if allocated
    if(0 == a || 0 == b || 0 == c || 0 == d){
        printf("memory allocation failed\n");
        if (0!=a) free (a);
        if (0!=b) free (b);
        if (0!=c) free (c);
        if (0!=d) free (d);
        return 0;
    }

    /* initialisation */
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            *(a + i * n + j) = (float)((i+j) % 50);
            *(b + i * n + j) = (float)(((2*n - i) - j) % 50);
        }
    }

    // matrix-multiplication sequential
    memset(c, 0, mem_size);
    dgemm_ref(a, b, c, n);

    // matrix-multiplication vectorized
    memset(d, 0, mem_size);
    dgemm(a, b, d, n);

    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            if(d[ i*n+ j ]!=c[ i*n+ j ]){
                printf("Bug in code indexes (i,j)=(%d,%d), values: %.1f, %.1f ! \n", i, j, c[ i*n+ j ], d[ i*n+ j ]);
                free(a); free(b); free(c); free(d);
                return -1;
            }
        }
    }

    free(a); free(b); free(c);
    return(0);
}
