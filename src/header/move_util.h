#ifndef MOVE_UTIL_H
#define MOVE_UTIL_H

#include "common.h"

field in_check(field own_pieces, field enemy_pieces, field position, bool color, field all_fields[]);
field find_legal_pawn_moves(field own_pieces, field enemy_pieces, field position, bool color);
field find_legal_rook_moves(field own_pieces, field enemy_pieces, field position);
field find_legal_diag_moves(field own_pieces, field enemy_pieces, field position);

void init_knight_moves(field knight_moves[]);
void init_king_moves(field king_moves[]);
void init_queen_moves(field queen_moves[]);
void init_bishop_moves(field bishop_moves[]);
void init_rook_moves(field rook_moves[]);

#endif