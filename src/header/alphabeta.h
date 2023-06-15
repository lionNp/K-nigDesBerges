#ifndef ALPHABETA_H
#define ALPHABETA_H
#include "common.h"
#include "stopwatch_utils.h"

float alphabeta(int depth, float alpha, float beta, bool max_player);
float pvs(int depth, float alpha, float beta, bool max_player);
//check if qs or alphabeta
float qsCheck(int depth, float alpha, float beta, bool max_player);
float qs(float alpha, float beta, bool max_player);
//MostValuableVictim-LeastValuableAttacker-Sorting--Bubblesort
void bsMVV_LVA(field[] moves_from, field[] moves_to, int[] piece_idx, int movecount);
#endif
