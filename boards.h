#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bit_boards_util.c"
#include <math.h> // when using maths you need to link the maths header with -lm flag when compiling

#define figuren_anz 8

typedef uint64_t field;

enum figuren{
    schwarz,
    weiss,
    koenig,
    dame,
    turm,
    pferd,
    laeufer,
    bauer
};

void print_board(field board);
void test_bitfield_print();
field add_to_board_coords(field board, int x, int y);
void test_add_to_board_coords();