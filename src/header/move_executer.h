#ifndef MOVE_EXECUTER_H
#define MOVE_EXECUTER_H
#include "common.h"

// make_move executes a move on global bitboards. Takes a field (move_from) with a bit set from where the move originates 
// and a field (move_to) to where the piece is moved. The piece is specified by its enumerated int value. 
// A captured field array saves which piece is taken.
void make_move(int piece, field moves_from, field moves_to, field captured[]);

// unmake_move reverts a move on global bitboards. Takes a field (move_from) with a bit set from where the move originates 
// and a field (move_to) to where the piece is moved. The piece is specified by its enumerated int value. 
// A captured field array in which a taken piece is saved.
void unmake_move(int piece, field moves_from, field moves_to, field captured[]);

void castle_flags(int piece, field moves_from);

#endif