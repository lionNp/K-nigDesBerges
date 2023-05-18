#include <pthread.h>
#include <sys/time.h>

#include "common.h"

void* func() { }

void main()
{
    int count = 16;
    unsigned long results[count];

    struct timeval stop, start;
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < count; i++)
    {

        pthread_t id;
        pthread_create(&id, NULL, func, NULL);
        pthread_join(id, NULL);

        results[i] = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    }
    gettimeofday(&stop, NULL);

    unsigned long sum = 0;
    for (size_t i = 0; i < count; i++)
        sum += results[i];

    //printf("%d Thread create/join took %lu us on avarage\n", count, sum / count); 
    printf("%d Thread create/join took %lu us\n", count, (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
}