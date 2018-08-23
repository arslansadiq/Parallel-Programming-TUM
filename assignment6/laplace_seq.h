#ifndef _LAPLACE_SEQ_
#define _LAPLACE_SEQ_

template<int SIZE>
inline void initialize(double a[SIZE + 2][SIZE + 2], double b[SIZE + 2][SIZE + 2])
{
    for (int i = 0; i < SIZE + 2; i++)
        for (int j = 0; j < SIZE + 2; j++)
        {
            a[i][j] = 0.0;
            b[i][j] = 0.0;
        }
}

template<int SIZE>
inline void time_step(double a[SIZE + 2][SIZE + 2], double b[SIZE + 2][SIZE + 2], int n)
{
    if (n % 2 == 0)
    {
        for (int i = 1; i < SIZE + 1; i++)
            for (int j = 1; j < SIZE + 1; j++)
                b[i][j] = (a[i + 1][j] + a[i - 1][j] + a[i][j - 1] + a[i][j + 1]) / 4.0;
    }
    else
    {
        for (int i = 1; i < SIZE + 1; i++)
            for (int j = 1; j < SIZE + 1; j++)
                a[i][j] = (b[i + 1][j] + b[i - 1][j] + b[i][j - 1] + b[i][j + 1]) / 4.0;
    }
}

#endif // !_LAPLACE_SEQ_
