#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define game_string "r3k2r/8/p6p/P6P/8/8/8/R1P1K2R w"

typedef uint64_t field; 
extern field files[8];
extern field rows[8];
extern field diag_r[15];
extern field diag_l[15];
extern int white_pawn_values[64];
extern int black_pawn_values[64];
extern int knight_values[64];
extern int bishop_values[64];
extern int rook_values[64];
extern int queen_values[64];
extern int white_king_values[64];
extern int black_king_values[64];
extern field knight_moves[64];
extern field king_moves[64];
// Player
extern bool is_player_white;
extern field bitfields[8];
extern bool castle_left[2];
extern bool castle_right[2];
//castle
extern field castle_black_left;
extern field castle_black_right;
extern field castle_black_right_check;
extern field castle_black_left_check;
extern field castle_white_left;
extern field castle_white_right;
extern field castle_white_right_check;
extern field castle_white_left_check;
// illegal move
#define illegal_move -9999.0f
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
#define figure_count 8

enum figure {
    black = 0,
    white = 1,
    king = 2,
    queen = 3,
    rook = 4,
    bishop = 5,
    knight = 6,
    pawn = 7
};

#endif