#ifndef EVALUATION_H
#define EVALUATION_H
#include "common.h"

// evaluate a move and return a float rating based of conditions
// takes a field (move_from) with a bit set from where the move originates 
// and a field (move_to) to where the piece is moved. The piece is specified by its enumerated int value
float evaluation(field move_from, field move_to, int piece);
int random_max_rating(float rating[], int move_count);
#endif