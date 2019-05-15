#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>

#include "mandelbrot_set.h"

struct pthread_args
{
	int x_resolution;
	int y_resolution;
	int max_iter;
	double view_x0;
	double view_x1;
	double view_y0;
	double view_y1;
	double x_stepsize;
	double y_stepsize;
	int palette_shift;
	int num_thread;
	int num_threads;
	void *image;
};

void* draw(void *args){
	
	struct pthread_args *arg = (struct pthread_args*) args;
	int rows_per_thread = arg->y_resolution/arg->num_threads;
    int row_start = rows_per_thread * (arg->num_thread - 1);
    int row_end = row_start + rows_per_thread;
    if(arg->num_threads == arg->num_thread){
        row_end = arg->y_resolution;
    }
	unsigned char (*img)[arg->x_resolution][3] = (unsigned char (*)[arg->x_resolution][3]) arg->image;
	double y;
	double x;

	complex double Z;
	complex double C;

	int k;
	
	for (int i = row_start; i < row_end; i++)
	{
		for (int j = 0; j < arg->x_resolution; j++)
		{
			y = arg->view_y1 - i * arg->y_stepsize;
			x = arg->view_x0 + j * arg->x_stepsize;

			Z = 0 + 0 * I;
			C = x + y * I;

			k = 0;

			do
			{
				Z = Z * Z + C;
				k++;
			} while (cabs(Z) < 2 && k < arg->max_iter);

			if (k == arg->max_iter)
			{
				memcpy(img[i][j], "\0\0\0", 3);
			}
			else
			{
				int index = (k + arg->palette_shift)
				            % (sizeof(colors) / sizeof(colors[0]));
				memcpy(img[i][j], colors[index], 3);
			}
		}
	}
}
void obj_assign(struct pthread_args *args, int x_resolution, int y_resolution, int max_iter, double view_x0, double view_x1, double view_y0, double view_y1, double x_stepsize, double y_stepsize, int palette_shift, unsigned char (*image)[x_resolution][3], int num_threads, int thread_num){
		args->x_resolution = x_resolution;
		args->y_resolution = y_resolution;
		args->max_iter = max_iter;
		args->view_x0 = view_x0;
		args->view_x1 = view_x1;
		args->view_y0 = view_y0;
		args->view_y1 = view_y1;
		args->x_stepsize = x_stepsize;
		args->y_stepsize = y_stepsize;
		args->palette_shift = palette_shift;
		args->image = (void*)image;
		args->num_thread = thread_num;
		args->num_threads = num_threads;
}
						 
						 

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
	                double view_x0, double view_x1, double view_y0, double view_y1,
	                double x_stepsize, double y_stepsize,
	                int palette_shift, unsigned char (*image)[x_resolution][3],
						 int num_threads) {
	// TODO:
	// implement your solution in this file.
	
	/**Creation of threads**/
	pthread_t *threads = (pthread_t*) malloc (num_threads *sizeof(pthread_t));
	
	/**Assignment of arguments for each kernel respectively**/
	struct pthread_args* args = (struct pthread_args*) malloc (num_threads*sizeof (struct pthread_args));
	
	/**Division of work for assignment to each thread**/
	for (int i=0; i<num_threads; ++i){
		
		obj_assign(args+i, x_resolution, y_resolution, max_iter, view_x0, view_x1,
                       view_y0, view_y1, x_stepsize, y_stepsize, palette_shift,
                      (void*) image, num_threads, (i+1));
		
		//thread creation
		pthread_create(&threads[i], NULL, draw, args+i);
	}
	
	/**Joining Threads**/
	for (int i=0; i<num_threads; ++i){
		pthread_join(threads[i], NULL);
	}
	
	free(threads); free(args);
}

