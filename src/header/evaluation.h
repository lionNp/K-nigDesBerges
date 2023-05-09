#ifndef EVALUATION_H
#define EVALUATION_H
#include "common.h"

// evaluate a move and return a float rating based of conditions
// takes a field (move_from) with a bit set from where the move originates 
// and a field (move_to) to where the piece is moved. The piece is specified by its enumerated int value
float evaluation(field move_to, field move_from, int piece);

#endif