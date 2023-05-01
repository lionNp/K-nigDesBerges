#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <sys/time.h>

struct s_stopwatch
{
    struct timeval start_time;
    struct timeval stop_time;
} typedef stopwatch;

stopwatch* start_stopwatch();
unsigned long stop_stopwatch(stopwatch* running);

#endif