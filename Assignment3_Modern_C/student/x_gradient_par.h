#ifndef _X_GRADIENT_PAR
#define _X_GRADIENT_PAR

#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <omp.h>
#include"x_gradient.h"
#include<thread>
using namespace boost::gil;

template <typename Out> struct halfdiff_cast_channels; // forward declaration

template <typename SrcView, typename DstView>
void Arslan(const SrcView &src, const DstView &dst, int num_threads)
{
    typedef typename channel_type<DstView>::type dst_channel_t;

    for (int y = 0; y < src.height(); ++y)
    {
        typename SrcView::x_iterator src_it = src.row_begin(y);
        typename DstView::x_iterator dst_it = dst.row_begin(y);

        for (int x = 1; x < src.width() - 1; ++x)
        {
            static_transform(src_it[x - 1], src_it[x + 1], dst_it[x],
                             halfdiff_cast_channels<dst_channel_t>());
        }
    }
}
template <typename SrcView, typename DstView>
void x_gradient(const SrcView& src, const DstView& dst, int num_threads) {

	// TODO put your solution in here.
        int height = src.height()/num_threads;
	std::vector<std::thread> t(num_threads);
	for(int i=0; i<num_threads; ++i){
		t[i] = std::thread(Arslan<SrcView,DstView>, src, dst , num_threads, i*height, height*(i+1));
	}
	for(int i=0; i<num_threads; ++i){
		t[i].join();
	}
}

#endif // !_X_GRADIENT_PAR_
