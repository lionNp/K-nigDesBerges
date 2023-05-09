#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "common.h"

// generate_moves determines all pseudo legal moves. Takes an empty field array (legal_moves)
// and an empty field array (legal_moves_piece) in which all moves for a piece are saved.
// An empty in array save the enumerated piece integer and 
// an empty int array (counts) saves all possible pseudolegal moves and the number of pieces
void generate_moves(field legal_moves[], field legal_moves_piece[], int piece_array[], int counts[]);

#endif
