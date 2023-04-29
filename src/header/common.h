#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define game_string "rn1qkbnr/pppppppp/8/b7/3PP3/8/PPP2PPP/RNBQKBNR"

typedef uint64_t field; 
extern field files[8];
extern field rows[8];
extern field diag_r[15];
extern field diag_l[15];
// Files
#define a_file 0x8080808080808080ull
#define b_file 0x4040404040404040ull
#define c_file 0x2020202020202020ull
#define d_file 0x1010101010101010ull
#define e_file 0x0808080808080808ull
#define f_file 0x0404040404040404ull
#define g_file 0x0202020202020202ull
#define h_file 0x0101010101010101ull
// rows
#define row_1 0x00000000000000ffull
#define row_2 0x000000000000ff00ull
#define row_3 0x0000000000ff0000ull
#define row_4 0x00000000ff000000ull
#define row_5 0x000000ff00000000ull
#define row_6 0x0000ff0000000000ull
#define row_7 0x00ff000000000000ull
#define row_8 0xff00000000000000ull 
// diagonals left corner
#define diag_l_1 0x0000000000000080ull
#define diag_l_2 0x0000000000008040ull
#define diag_l_3 0x0000000000804020ull
#define diag_l_4 0x0000000080402010ull
#define diag_l_5 0x0000008040201008ull
#define diag_l_6 0x0000804020100804ull
#define diag_l_7 0x0080402010080402ull
#define diag_l_8 0x8040201008040201ull
#define diag_l_9 0x4020100804020100ull
#define diag_l_10 0x2010080402010000ull
#define diag_l_11 0x1008040201000000ull
#define diag_l_12 0x0804020100000000ull
#define diag_l_13 0x0402010000000000ull
#define diag_l_14 0x0201000000000000ull
#define diag_l_15 0x0100000000000000ull
// diagonals right corner
#define diag_r_1 0x0000000000000001ull
#define diag_r_2 0x0000000000000102ull
#define diag_r_3 0x0000000000010204ull
#define diag_r_4 0x0000000001020408ull
#define diag_r_5 0x0000000102040810ull
#define diag_r_6 0x0000010204081020ull
#define diag_r_7 0x0001020408102040ull
#define diag_r_8 0x0102040810204080ull
#define diag_r_9 0x0204081020408000ull
#define diag_r_10 0x0408102040800000ull
#define diag_r_11 0x0810204080000000ull
#define diag_r_12 0x1020408000000000ull
#define diag_r_13 0x2040800000000000ull
#define diag_r_14 0x4080000000000000ull
#define diag_r_15 0x8000000000000000ull

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