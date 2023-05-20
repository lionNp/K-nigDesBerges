#ifndef EVALUATION_H
#define EVALUATION_H
#include "common.h"

// evaluate a move and return a float rating based of conditions
// takes a field (move_from) with a bit set from where the move originates 
// and a field (move_to) to where the piece is moved. The piece is specified by its enumerated int value
float evaluation(bool max_player);
int random_max_rating(float rating[], int move_count);
int max_rating(float rating[], int move_count);

float evaluate_position(bool max_player);
float evaluate_control(bool max_player);
float evaluate_material(bool max_player);

#endif