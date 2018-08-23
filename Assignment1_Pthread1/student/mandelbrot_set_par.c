#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>
#include <omp.h>
#include "mandelbrot_set.h"

struct thread_obj{
int x_resolution, y_resolution, max_iter, paletter_shift, num_threads, thread_num;
double view_x0, view_x1, view_y0, view_y1, x_stepsize, y_stepsize;
void *image;
};

//function to assign every thread's object it's respective values
void obj_assign(struct thread_obj *args, int x_resolution, int y_resolution, int max_iter,
	                double view_x0, double view_x1, double view_y0, double view_y1,
	                double x_stepsize, double y_stepsize,
	                int palette_shift;, unsigned char (*image)[x_resolution][3],
						 int num_threads, int thread_num){
     args->x_resolution = x_resolution;
     args->y_resolution = y_resolution;
     args->max_iter = max_iter;
     args->paletter_shift = paletter_shift;
     args->num_threads = num_threads;
     args->view_x0 = view_x0;
     args->view_x1 = view_x1;
     args->view_y0 = view_y0;
     args->view_y1 = view_y1;
     args->x_stepsize = x_stepsize;
     args->y_stepsize = y_stepsize;
     args->image = (void*) image;
     args->thread_num = thread_num;
}
void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
	                double view_x0, double view_x1, double view_y0, double view_y1,
	                double x_stepsize, double y_stepsize,
	                int palette_shift;, unsigned char (*image)[x_resolution][3],
						 int num_threads) {
	// TODO:
	// implement your solution in this file.
	pthread_t *threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
        struct thread_obj *args = (struct thread_obj*) malloc(num_threads * sizeof(struct thread_obj));
        for(int i=0; i<num_threads; ++i){
            obj_assign(args+i, x_resolution, y_resolution, max_iter, view_x0, view_x1,
                       view_y0, view_y1, x_stepsize, y_stepsize, palette_shift,
                      (void*) image, num_threads, (i+1));
            pthread_create(&threads[i], NULL, kernel, (void*)args+i);
        }
        for(int i=0; i<num_threads; ++i){
           pthread_join(threads[i], NULL);
        }
        free(args);
        free(threads);        
}

void* kernel(void* arg){
        struct pthread_obj *args = (struct pthread_obj*) arg;
        int rows_per_thread = args->y_resolution/args->num_threads;
        int row_start = rows_per_thread * (args->thread_num - 1);
        int row_end = row_start + rows_per_thread;
        if(args->num_threads == args->thread_num){
           row_end = args->y_resolution;
        }
        unsigned char (image*)[args->x_resolution][3] = (unsigned char (*)[args->x_resolution][3]) args->image
        double y;
	double x;

	complex double Z;
	complex double C;

	int k;

	for (int i = row_start; i < row_end; i++)
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
