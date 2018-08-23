#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <omp.h>

#include "mandelbrot_set.h"

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
                     double view_x0, double view_x1, double view_y0, double view_y1,
                     double x_stepsize, double y_stepsize,
                     int palette_shift, unsigned char (*img)[x_resolution][3],
							int num_threads) {
	double y;
	double x;

	complex double Z;
	complex double C;

	int k;

	for (int i = 0; i < y_resolution; i++)
	{
		for (int j = 0; j < x_resolution; j++)
		{
			y = view_y1 - i * y_stepsize;
			x = view_x0 + j * x_stepsize;

			Z = 0 + 0 * I;
			C = x + y * I;

			k = 0;

			do
			{
				Z = Z * Z + C;
				k++;
			} while (cabs(Z) < 2 && k < max_iter);

			if (k == max_iter)
			{
				memcpy(img[i][j], "\0\0\0", 3);
			}
			else
			{
				int index = (k + palette_shift)
				            % (sizeof(colors) / sizeof(colors[0]));
				memcpy(img[i][j], colors[index], 3);
			}
		}
	}
}
