#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <chrono>
#include <iostream>

#include"x_gradient.h"

using namespace boost::gil;

int main(int argc, char **argv) {

    int num_threads=1;
    char file_name[256] = "tum.jpg";
    int no_output = 0;

	int c;
	while ((c = getopt(argc, argv, "t:n:f:")) != -1)
	{
		switch (c)
		{
		case 't':
			if (sscanf(optarg, "%d", &num_threads) != 1)
				goto error;
			break;
		case 'n':
			if (sscanf(optarg, "%d", &no_output) != 1)
				goto error;
			break;
		case 'f':
			strncpy(file_name, optarg, sizeof(file_name));
			break;
		case '?':
        error: printf(
			    "Usage:\n"
			    "-t \t number of threads used in computation(default: 1)\n"
			    "-f \t output file name(default: tum.jpg)\n"
				"-n \t no output(default: 0)\n"
			    "\n"
			    "Example:\n"
			    "%s -t 4 -f tum.jpg\n",
			    argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}

    rgb8_image_t img;
    jpeg_read_image(file_name,img);
    
    gray8s_image_t img_out(img.dimensions());
    fill_pixels(view(img_out),int8_t(0));

    auto start = std::chrono::high_resolution_clock::now();
    x_luminosity_gradient(const_view(img), view(img_out), num_threads);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;

    if (!no_output)
        jpeg_write_view("output.jpg",color_converted_view<gray8_pixel_t>(const_view(img_out)));

    std::cout << "Time: " << diff.count() << " seconds\n";

    return 0;
}