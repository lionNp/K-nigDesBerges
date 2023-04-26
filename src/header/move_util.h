#ifndef MOVE_UTIL_H
#define MOVE_UTIL_H

#include "common.h"

field check_for_chess(field own_pieces, field enemy_pieces, field position, bool color, field all_fields[]);
field find_legal_pawn_moves(field own_pieces, field enemy_pieces, field position, bool color);
field find_legal_rook_moves(field own_pieces, field enemy_pieces, field position);
field find_legal_diag_moves(field own_pieces, field enemy_pieces, field position);

#endif