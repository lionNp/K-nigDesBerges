#ifndef EVALUATION_H
#define EVALUATION_H
#include "common.h"

extern float material_modify;
extern float position_modify;
extern float contol_modify;
extern float pawns_modify;
extern float king_safety_modify;

// evaluate a move and return a float rating based of conditions
// takes a field (move_from) with a bit set from where the move originates 
// and a field (move_to) to where the piece is moved. The piece is specified by its enumerated int value
float evaluation();
int random_max_rating(float rating[], int move_count);
int max_rating(float rating[], int move_count);
int min_rating(float rating[], int move_count);
float evaluate_position();
float evaluate_control();
float evaluate_material();
float evaluate_pawns();
float evaluate_king_safety();
float piece_value(field board);
#endif