#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>

#include "mandelbrot_set.h"

static int global_start;
#define CHUNK_SIZE 8
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; 
typedef struct {

    int chunk_size;
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
    void *image;
	
} compute_args;

void * kernell( void * arguments){

	compute_args *args = arguments;
	int chunk_size = args->chunk_size;
	int x_resolution1 = args->x_resolution;
	int y_resolution=args->y_resolution;
    int max_iter=args->max_iter;
	double view_x0=args->view_x0;
    double view_x1=args->view_x1;
	double view_y0=args->view_y0;
    double view_y1=args->view_y1;
	double x_stepsize=args->x_stepsize;
	double y_stepsize=args->y_stepsize;
	int palette_shift=args->palette_shift;
    unsigned char (*image)[x_resolution1][3]= (unsigned char (*)[x_resolution1][3])args->image;
	int start;
	
	for (;;) {
		pthread_mutex_lock(&mtx);
	    // if every row is processed unlock and come out
		if ( y_resolution - global_start < 1 ) {
	        pthread_mutex_unlock(&mtx);
			break;
		}
	    start = global_start; 

		// increase the shared loop start    
	    global_start += chunk_size;
		pthread_mutex_unlock(&mtx);

		// for the thread that works on the last chunk when the last chunk
		// is smaller than chunk_size
	    if ( y_resolution - start < chunk_size )
	        chunk_size = y_resolution - start;

			for (int i = start; i < start + chunk_size; i++)
			{
				for (int j = 0; j < x_resolution1; j++)
				{
					double y = view_y1 - i * y_stepsize;
					double x = view_x0 + j * x_stepsize;

					complex double Z = 0 + 0 * I;
					complex double C = x + y * I;

					int k = 0;

					do
					{
						Z = Z * Z + C;
						k++;
					} while (cabs(Z) < 2 && k < max_iter);

					if (k == max_iter)
					{
						memcpy(image[i][j], "\0\0\0", 3);
					}
					else
					{
						int index = (k + palette_shift)
						            % (sizeof(colors) / sizeof(colors[0]));
						memcpy(image[i][j], colors[index], 3);
					}
				}
			}

}

}

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
	                double view_x0, double view_x1, double view_y0, double view_y1,
	                double x_stepsize, double y_stepsize,
	                int palette_shift, unsigned char (*image)[x_resolution][3],
						 int num_threads) {


	global_start = 0; // for starting
	int i, t;

	pthread_t *thread = (pthread_t*)malloc(num_threads * sizeof(*thread));
	compute_args *args = (compute_args*)malloc(num_threads * sizeof(*args));
	
	// start the threads
	for (t=0; t < num_threads; t++) {
		
		args[t].chunk_size = CHUNK_SIZE;
		args[t].x_resolution = x_resolution;
		args[t].y_resolution=y_resolution;
	    args[t].max_iter=max_iter;
		args[t].view_x0=view_x0;
	    args[t].view_x1= view_x1;
		args[t].view_y0=view_y0;
        args[t].view_y1=view_y1;		 
		args[t].x_stepsize=x_stepsize;
		args[t].y_stepsize=y_stepsize;
		args[t].palette_shift=palette_shift;
	    args[t].image= (void*) image;
		pthread_create(&thread[t], NULL, kernell, &args[t]);
	}

	// join threads
	for (t = 0; t < num_threads; t++) {
		pthread_join(thread[t], NULL);
	}

	free(thread);
	free(args);

	}
