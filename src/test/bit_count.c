#include <sys/time.h>
#include <math.h>

#include "common.h"

void main()
{
    printf("Doing 100 iterations every bit position\n\n");

    printf("Using Bitshit:\n");
    struct timeval stop, start;

    unsigned long sum_time = 0;
    for (size_t i = 0; i < 64; i++)
    {
        gettimeofday(&start, NULL);
        for (size_t j = 0; j < 100; j++)
        {
            field pos = (field) 1 << i;
            int bit_num = 1;
            for(; bit_num < 64; bit_num++)
            {
                if(pos >> bit_num == 0) 
                    break;
                //if((pos > (1 << bit_num + 7))) 
                //    bit_num += 7;
            }
            bit_num--;

            if(bit_num != i)
                printf("Wrong Result !\n");
        }
        gettimeofday(&stop, NULL);
        unsigned long time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
        sum_time += time;
    }

    printf("Avg calculation time: %luus\n", sum_time / 64);
    printf("Complete Operation took %luus\n", sum_time);

    printf("\n-------------\n\n");

    printf("Using log2()\n");
    sum_time = 0;
    for (size_t i = 0; i < 64; i++)
    {
        gettimeofday(&start, NULL);
        for (size_t j = 0; j < 100; j++)
        {
            field pos = (field) 1 << i;
            int bit_num = log2(pos);

            if(bit_num != i)
                printf("Wrong Result !\n");
        }
        gettimeofday(&stop, NULL);
        unsigned long time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
        sum_time += time;
    }

    printf("Avg calculation time: %luus\n", sum_time / 64);
    printf("Complete Operation took %luus\n", sum_time);
}