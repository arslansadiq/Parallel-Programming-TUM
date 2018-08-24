#ifndef _X_GRADIENT_REF
#define _X_GRADIENT_REF

#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include"x_gradient.h"

using namespace boost::gil;

template <typename Out> struct halfdiff_cast_channels; // forward declaration

template <typename SrcView, typename DstView>
void x_gradient_ref(const SrcView& src, const DstView& dst) {
    typedef typename channel_type<DstView>::type dst_channel_t;

    for (int y=0; y<src.height(); ++y) {
        typename SrcView::x_iterator src_it = src.row_begin(y);
        typename DstView::x_iterator dst_it = dst.row_begin(y);

        for (int x=1; x<src.width()-1; ++x) {
            static_transform(src_it[x-1], src_it[x+1], dst_it[x], 
                             halfdiff_cast_channels<dst_channel_t>());
        }
    }
}

template <typename SrcView, typename DstView>
void x_luminosity_gradient_ref(const SrcView& src, const DstView& dst) {
    typedef pixel<typename channel_type<SrcView>::type, gray_layout_t> gray_pixel_t;
    x_gradient_ref(color_converted_view<gray_pixel_t>(src), dst);
}

#endif // !_X_GRADIENT_REF_