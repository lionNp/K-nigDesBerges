#include "hash_boards.h"
#include "common.h"
#include "math.h"

field hash_boards(){
    field val = 0;
    for(int i = 0; i < 8; i++)
        val += hash_coeff[i] * bitfields[i];
    return (val % hash_prime);
}