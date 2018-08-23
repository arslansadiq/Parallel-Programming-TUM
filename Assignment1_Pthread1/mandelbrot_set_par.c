#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>
#include <math.h>

#include "mandelbrot_set.h"

struct ThreadObject {
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
	int thread_number;
	int num_threads;
};

void* kernel(void*);

void initializeObject(struct ThreadObject* obj, int x_resolution,
	int y_resolution, int max_iter,
    double view_x0, double view_x1, double view_y0, double view_y1,
    double x_stepsize, double y_stepsize,
    int palette_shift, unsigned char (*image)[x_resolution][3],
	int num_threads, int thread_number) {

	obj->x_resolution = x_resolution;
	obj->y_resolution = y_resolution;
	obj->max_iter = max_iter;
	obj->view_x0 = view_x0;
	obj->view_x1 = view_x1;
 	obj->view_y0 = view_y0;
	obj->view_y1 = view_y1;
	obj->x_stepsize = x_stepsize;
	obj->y_stepsize = y_stepsize;
	obj->palette_shift = palette_shift;
	obj->image = (void*) image;
	obj->num_threads = num_threads;
	obj->thread_number = thread_number;
}

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
    double view_x0, double view_x1, double view_y0, double view_y1,
    double x_stepsize, double y_stepsize,
    int palette_shift, unsigned char (*image)[x_resolution][3],
	int num_threads) {
	pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t) * num_threads);
	struct ThreadObject* args = (struct ThreadObject*) malloc(sizeof(struct ThreadObject) * num_threads);
	for (int i = 0; i < num_threads; i++) {
		initializeObject(args+i, x_resolution, y_resolution, max_iter,
			view_x0, view_x1, view_y0, view_y1,
			x_stepsize, y_stepsize, palette_shift,
			image, num_threads, i+1);
		pthread_create(&threads[i], NULL, kernel, (void*)(args+i));
	}
	for (int i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	free(threads); free(args);
}

void* kernel(void* arg) {
	struct ThreadObject* args = (struct ThreadObject *) arg;
	int rows_per_thread = (args->y_resolution / args->num_threads);
	int row_start = rows_per_thread * (args->thread_number - 1);
	int row_end = row_start + rows_per_thread;
	if (args->thread_number == args->num_threads) {
		row_end = args->y_resolution;
	}
	unsigned char (*image)[args->x_resolution][3] = (unsigned char(*)[args->x_resolution][3]) args->image;
	double y, x;
	complex double Z, C;
	int k;

	for (int i = row_start; i < row_end; i++) {
		for (int j = 0; j < args->x_resolution; j++) {
			y = args->view_y1 - i * args->y_stepsize;
			x = args->view_x0 + j * args->x_stepsize;

			Z = 0 + 0 * I;
			C = x + y * I;

			k = 0;
			do {
				Z = Z * Z + C;
				k++;
			} while (cabs(Z) < 2 && k < args->max_iter);

			if (k == args->max_iter) {
				memcpy(image[i][j], "\0\0\0", 3);
			} else {
				int index = (k + args->palette_shift)
				            % (sizeof(colors) / sizeof(colors[0]));
				memcpy(image[i][j], colors[index], 3);
			}
		}
	}
}
