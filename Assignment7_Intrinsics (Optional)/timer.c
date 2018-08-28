/*
 * timer.c
 *
 * @date   18/09/09
 * @author Wolfgang Eckhardt
 * @mailto eckhardw@in.tum.de
 *
 */

#include "timer.h"


time_marker_t get_time() {
    time_marker_t retval;
    gettimeofday(&retval.tv, NULL);
    retval.ticks = clock();
    return retval;
}


double get_ToD_diff_time(time_marker_t time) {
    struct timeval tmp;
    gettimeofday(&tmp, NULL);
    tmp.tv_sec -= time.tv.tv_sec;
    tmp.tv_usec -= time.tv.tv_usec;
    return (double)tmp.tv_sec + (double)tmp.tv_usec*1e-6;
}


double get_ticks_diff_time(time_marker_t time) {
    clock_t tmp = clock();
    return ((tmp - time.ticks) / (double)CLOCKS_PER_SEC);
}
