#ifndef MOVE_UTIL_H
#define MOVE_UTIL_H

#include "common.h"

//calculate time budget based on the number of moves possible
field calc_time_budget(int move_count);

// check if king is checked
field in_check(field position);

// find all possible pawn moves
field find_legal_pawn_moves(field own_pieces, field enemy_pieces, field position);
field find_legal_pawn_attacks(field position);

field pinned_piece_check(field position);
field find_bishop_pins(field own_pieces, field enemy_pieces, field position);
field find_rook_pins(field own_pieces, field enemy_pieces, field position);

field find_bishop_check(field own_pieces, field enemy_pieces, field position);
field find_rook_check(field own_pieces, field enemy_pieces, field position);
// find all possible vertical/horizontal and diagonal moves
field find_legal_rook_moves(field own_pieces, field enemy_pieces, field position);
field find_legal_diag_moves(field own_pieces, field enemy_pieces, field position);
field find_legal_rook_attacks(field own_pieces, field enemy_pieces, field position);
field find_legal_diag_attacks(field own_pieces, field enemy_pieces, field position);

// initialize move_masks || currently no longer used
void init_knight_moves(field knight_moves[]);
void init_king_moves(field king_moves[]);
void init_queen_moves(field queen_moves[]);
void init_bishop_moves(field bishop_moves[]);
void init_rook_moves(field rook_moves[]);

#endif