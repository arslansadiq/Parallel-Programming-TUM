#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <iostream>

#include"x_gradient.h"

using namespace boost::gil;

int main() {
    rgb8_image_t img;
    jpeg_read_image("tum.jpg",img);

    gray8s_image_t img_out_ref(img.dimensions());
    fill_pixels(view(img_out_ref),int8_t(0));

    gray8s_image_t img_out(img.dimensions());
    fill_pixels(view(img_out),int8_t(0));

    for (int t = 2; t < 8; t++)
    {
        x_luminosity_gradient_ref(const_view(img), view(img_out_ref));
        x_luminosity_gradient(const_view(img), view(img_out),t);
        
        auto v_ref = color_converted_view<gray8_pixel_t>(const_view(img_out_ref));
        auto v = color_converted_view<gray8_pixel_t>(const_view(img_out));

        for (int y=0; y<v_ref.height(); ++y){
            auto v_it = v.row_begin(y);
            auto v_ref_it = v_ref.row_begin(y);
            for (int x=0; x<v_ref.width(); ++x) {
                if (*v_it !=  *v_ref_it){
                    fprintf(stderr, "Computation with %d threads failed\n", t);
                    fprintf(stderr, "Image differs from the one produced by the sequential version\n");
                    return 1;
                }
                v_it++;
                v_ref_it++;
            }
        }
    }
    
    return 0;
}