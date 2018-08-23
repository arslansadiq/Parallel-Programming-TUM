#ifndef _LAPLACE_
#define _LAPLACE_

#include <iostream>

#include "laplace_ref.h"

#ifndef PARALLEL
#include "laplace_seq.h"
#endif

#ifdef PARALLEL
#include "laplace_par.h"
#endif

template<int SIZE>
void set_boundaries(double a[SIZE + 2][SIZE + 2], double b[SIZE + 2][SIZE + 2])
{
    for (int i = 0; i < SIZE + 2; i++)
    {
        a[i][0] = 1.0;
        b[i][0] = 1.0;
    }
    for (int i = 0; i < SIZE + 2; i++)
    {
        a[i][SIZE + 1] = 1.0;
        b[i][SIZE + 1] = 1.0;
    }
}

template<int SIZE>
void print(double a[SIZE + 2][SIZE + 2])
{
    for (int i = 0; i < SIZE+2; i++) {
       for (int j= 0; j < SIZE+2; j++) {
	  std::cout << a[i][j] << " ";
       }
       std::cout << std::endl;
    }
}

#endif // !_LAPLACE_
