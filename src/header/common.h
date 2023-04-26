#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint64_t field; 

#define down_diag 0x8040201008040201;
#define up_diag 0x102040810204080;
#define horizontal 0xFF;
#define vertical 0x8080808080808080;

#define white true
#define black false
#define figure_count 8

enum figure {
    bl = 0,
    w = 1,
    k = 2,
    q = 3,
    r = 4,
    b = 5,
    n = 6,
    p = 7
};

#endif