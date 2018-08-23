#ifndef _X_GRADIENT_
#define _X_GRADIENT_

#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>

#include "x_gradient_ref.h"

#ifndef PARALLEL
#include "x_gradient_seq.h"
#endif

#ifdef PARALLEL
#include "x_gradient_par.h"
#endif

using namespace boost::gil;

template <typename Out>
struct halfdiff_cast_channels {
    template <typename T> Out operator()(const T& in1, const T& in2) const {
        return Out((in2-in1)/2);
    }
};

template <typename SrcView, typename DstView>
void x_luminosity_gradient(const SrcView& src, const DstView& dst, int num_threads) {
    typedef pixel<typename channel_type<SrcView>::type, gray_layout_t> gray_pixel_t;
    x_gradient(color_converted_view<gray_pixel_t>(src), dst, num_threads);
}

#endif // !_X_GRADIENT_