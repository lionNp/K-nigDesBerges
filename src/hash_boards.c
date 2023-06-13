#include "hash_boards.h"
#include "common.h"
#include "math.h"

// avg 25-50 hash collisions, median 30, min 9, max 83
field hash_boards(){
    field val = 0UL;
    for(int i = 0; i < 8; i++)
        val += hash_coeff[i] * bitfields[i];
    return (val % hash_prime);
}

// avg 90k-105k hash collisions, median 95
field hash_boards2(){
    field val = 0UL;
    
    for(int i = 0; i < 8; i++)
    {
        val = ((val >> 32) ^ val) * 564126427;
        val = ((val >> 32) ^ val) * 564126427;
        val = ((val >> 16) ^ val) * 0x45d9f3b;
        val = ((val >> 16) ^ val) * 0x45d9f3b;
        val = (val >> 16) ^ val * hash_coeff[i] * bitfields[i];
    }
    return (val % hash_prime);
}