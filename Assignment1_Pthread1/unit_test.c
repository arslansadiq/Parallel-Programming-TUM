#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <omp.h>
#include <time.h>

#include "mandelbrot_set.h"

int main(int argc, char **argv)
{
   int max_iter = 1000; // sizeof(colors);
   int x_resolution = 480;
   int y_resolution = 380;
   int palette_shift = 0;
   double view_x0 = -2.00;
   double view_x1 = +0.50;
   double view_y0 = -1.25;
   double view_y1 = +1.25;
	
   double x_stepsize = (view_x1 - view_x0) / x_resolution;
   double y_stepsize = (view_y1 - view_y0) / y_resolution;

   printf("Following settings are used for computation:\n"
          "Max. iterations: %d\n"
	       "Resolution: %dx%d\n"
	       "View frame: [%lf,%lf]x[%lf,%lf]\n"
	       "Stepsize x = %lf y = %lf\n"
	       , max_iter, x_resolution,
	       y_resolution, view_x0, view_x1, view_y0, view_y1, x_stepsize,
	       y_stepsize);

   // sequential handling
   unsigned char (*image)[x_resolution][3];
   image = malloc(x_resolution * y_resolution * sizeof(char[3]));

   if (image == NULL )
   {
      perror("malloc");
      exit(EXIT_FAILURE);
   }

   // compute mandelbrot sequentially
   mandelbrot_draw_ref( x_resolution, y_resolution,
                        max_iter,
                        view_x0, view_x1, view_y0, view_y1,
                        x_stepsize, y_stepsize,
                        palette_shift,
                        image);


	// compute mandelbrot in parallel and check
 	for (int t=2; t<8; t++) {
		unsigned char (*image_par)[x_resolution][3];
		image_par = malloc(x_resolution * y_resolution * sizeof(char[3]));
		mandelbrot_draw( x_resolution, y_resolution,
							  max_iter,
                       view_x0, view_x1, view_y0, view_y1,
                       x_stepsize, y_stepsize,
                       palette_shift,
                       image_par, t);
      for (int i=0; i < y_resolution; i++) 
         for (int j=0; j < x_resolution; j++)
            if (*image[i][j] != *image_par[i][j]) {
               fprintf(stderr, "Computation with %d threads failed\n", t);
               fprintf(stderr, "Image differs from the one produced by the sequential version\n");
               return 1;
            }
      free(image_par);
   }

   free (image);
   return 0;
}
