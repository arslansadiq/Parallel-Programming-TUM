#include"dgemm.h"
#include <immintrin.h>

void dgemm(float *a, float *b, float *c, int n)
{
    __m256 va, vb, tmp;
    float tmp_arr[8];

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k+=8){ //iterates in 8*float chunks
                //c[i * n + j] += a[i * n  + k] * b[j * n  + k];
                va = _mm256_loadu_ps(&a[i*n+k]);
                vb = _mm256_loadu_ps(&b[j*n+k]);
                tmp = _mm256_mul_ps(va,vb);
                tmp = _mm256_hadd_ps(tmp,_mm256_permute2f128_ps(tmp,tmp,1));
                tmp = _mm256_hadd_ps(tmp,tmp);
                tmp = _mm256_hadd_ps(tmp,tmp);
                c[i*n+j] += tmp[0];
            }
        }
    }
}