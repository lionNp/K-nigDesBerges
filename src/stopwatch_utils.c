#include <stdlib.h>

#include "stopwatch_utils.h"

stopwatch* start_stopwatch()
{
    stopwatch* new_stopwatch = malloc(sizeof(stopwatch));
    gettimeofday(&new_stopwatch->start_time, NULL);
    return new_stopwatch;
}

unsigned long stop_stopwatch(stopwatch* running_stopwatch)
{
    gettimeofday(&running_stopwatch->stop_time, NULL);

    unsigned long time = (running_stopwatch->stop_time.tv_sec - running_stopwatch->start_time.tv_sec) * 1000000 
                        + running_stopwatch->stop_time.tv_usec - running_stopwatch->start_time.tv_usec;

    free(running_stopwatch);
    return time;
}