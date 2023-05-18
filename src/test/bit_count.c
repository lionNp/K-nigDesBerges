#include <sys/time.h>
#include <math.h>

#include "common.h"
#include "stopwatch_utils.h"

void main()
{
    printf("Doing 100 iterations every bit position\n\n");

    printf("Using Bitshit:\n");
    
    unsigned long sum_time = 0;
    for (size_t i = 0; i < 64; i++)
    {
        stopwatch stopwatch1 = start_stopwatch();
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
        unsigned long time = stop_stopwatch(stopwatch1);
        sum_time += time;
    }

    printf("Avg calculation time: %luus\n", sum_time / 64);
    printf("Complete Operation took %luus\n", sum_time);

    printf("\n-------------\n\n");

    printf("Using log2()\n");
    sum_time = 0;
    for (size_t i = 0; i < 64; i++)
    {
        stopwatch stopwatch2 = start_stopwatch();
        for (size_t j = 0; j < 100; j++)
        {
            field pos = (field) 1 << i;
            int bit_num = log2(pos);

            if(bit_num != i)
                printf("Wrong Result !\n");
        }
        unsigned long time = stop_stopwatch(stopwatch2);
        sum_time += time;
    }

    printf("Avg calculation time: %luus\n", sum_time / 64);
    printf("Complete Operation took %luus\n", sum_time);
}