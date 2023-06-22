#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "stopwatch_utils.h"

#define game_string_5 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq"
#define game_string "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq"
#define game_string_4 "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq"
#define game_string_3 "r2qk2r/p1ppn1pp/bpnb1p2/4p3/4P3/2NPBN2/PPP1BPPP/R2Q1RK1 w Qkq" // Stellung 1
#define game_string_1 "8/1k6/1r3rp1/8/4R2P/2K5/3R4/8 w - -" // Stellung 2
#define game_string_2 "k7/8/3p4/4q3/3P4/8/4Q3/K7 w - -"

typedef uint64_t field; 

#define hash_prime 120000007
#define oob 10000.0f

#define tempo_bonus 0.1

extern field total_remaining_time;
extern field default_time_per_move;
extern field default_expected_move_num;
extern field total_moves;

extern int num_hash_collisions;
extern int hash_coeff[8];
extern float hash_table[hash_prime];

// each file set to 1 separately
extern field files[8];

// each rank set to 1 separately
extern field ranks[8];

// each right diagonal set to 1 separately
extern field diag_r[15];

// each left diagonal set to 1 separately
extern field diag_l[15];

// define value array to assign value to squares for white pawns pieces
extern int white_pawn_values[64];

// define value array to assign value to squares for black pawns pieces
extern int black_pawn_values[64];

// define value array to assign value to squares for knight pieces
extern int knight_values[64];

// define value array to assign value to squares for bishop pieces
extern int bishop_values[64];

// define value array to assign value to squares for rook pieces
extern int rook_values[64];

// define value array to assign value to squares for queen pieces
extern int queen_values[64];

// define value array to assign value to squares for white king pieces
extern int white_king_values[64];

// define value array to assign value to squares for black king pieces
extern int black_king_values[64];

// define move_masks for knight as their moves are independant of blocking pieces
extern field knight_moves[64];

// define move_masks for king as their moves are independant of blocking pieces
extern field king_moves[64];

// IMPORTANT: sets current player
extern bool is_player_white;

// IMPORTANT: sets gamestate
extern bool gameover;

// defines all bitfields: (0, white pieces), (1, black pieces), (2, king pieces), (3, queen pieces), (4, rook pieces),
// (5, bishop pieces), (6, knight pieces), (7, pawn pieces)
extern field bitfields[8];

// boolean array to determine if black and white can castle left
extern bool castle_left[2];

// boolean array to determine if black and white can castle right
extern bool castle_right[2];

// castling

// field set to easily check if pieces are on correct squares
extern field castle_black_left;
extern field castle_black_right;
extern field castle_black_right_check;
extern field castle_black_left_check;
extern field castle_white_left;
extern field castle_white_right;
extern field castle_white_right_check;
extern field castle_white_left_check;

extern int num_moves_iterated;
extern int num_moves_trans;

// winning move rating
#define winning_move 9999.0f
#define losing_move -9999.0f
#define max_move_count 100

// Center Square (King of the Hill square)
#define koth 0x0000001818000000ull

// Files
#define a_file 0x8080808080808080ull
#define b_file 0x4040404040404040ull
#define c_file 0x2020202020202020ull
#define d_file 0x1010101010101010ull
#define e_file 0x0808080808080808ull
#define f_file 0x0404040404040404ull
#define g_file 0x0202020202020202ull
#define h_file 0x0101010101010101ull

// ranks
#define rank_1 0x00000000000000ffull
#define rank_2 0x000000000000ff00ull
#define rank_3 0x0000000000ff0000ull
#define rank_4 0x00000000ff000000ull
#define rank_5 0x000000ff00000000ull
#define rank_6 0x0000ff0000000000ull
#define rank_7 0x00ff000000000000ull
#define rank_8 0xff00000000000000ull 

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

#define figure_count 8

// enumerate pieces for easy access and better readability
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