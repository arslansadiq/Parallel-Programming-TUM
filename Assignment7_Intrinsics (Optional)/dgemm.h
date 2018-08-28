#ifndef DGEMM_H
#define DGEMM_H

void dgemm(float *a, float *b, float *c, int n);
void dgemm_ref(float *a, float *b, float *c, int n);

#endif // DGEMM_H
