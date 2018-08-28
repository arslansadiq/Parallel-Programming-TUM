/*
 * timer.h
 *
 * Small library for time measurement.
 *
 * @date   18/09/09
 * @author Wolfgang Eckhardt
 * @mailto eckhardw@in.tum.de
 *
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>


#ifndef TIMER_H
#define TIMER_H

/**
 * This struct encapsulates all the different data of various timing functions.
 * Although one precise function should be enough, the overhead of using several
 * functions should not be too large, so it should be alright for an evaluation of
 * these functions.
 */
typedef struct {
    /** used by gettimeofday() */
    struct timeval tv;
    /** used by clock()  */
    clock_t ticks;
} time_marker_t;

/**
 * returns a struct time with all data initialized to the current time.
 */
time_marker_t get_time();

/**
 * returns the time difference in seconds between the time which is represented by the parameter
 * and the current time, using gettimeofday().
 * precision: up to micro seconds, hardware dependent.
 */
double get_ToD_diff_time(time_marker_t time);

/**
 * returns the time difference in seconds between the time which is represented by the parameter
 * and the current time, using clock().
 * precision: milli seconds.
 */
double get_ticks_diff_time(time_marker_t time);

#endif /* TIMER_H */
