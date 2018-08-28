#include"dgemm.h"

void dgemm_ref(float *a, float *b, float *c, int n)
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                c[i * n + j] += a[i * n  + k] * b[j * n  + k];
            }
        }
    }
}
