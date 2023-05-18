#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "common.h"

// generate_moves determines all pseudo legal moves. Takes an empty field array (legal_moves)
// and an empty field array (legal_moves_piece) in which all moves for a piece are saved.
// An empty in array save the enumerated piece integer and 
// an empty int array (counts) saves all possible pseudolegal moves and the number of pieces
int generate_moves(field moves_from[], field moves_to[], int piece_idx[]);
int generate_attacked_squares(field attacked_squares[], bool player);
field castle_move();
field filter_pin_moves(field pinned, field piece, field moves, field position);

#endif
