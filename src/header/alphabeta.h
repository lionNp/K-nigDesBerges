#ifndef ALPHABETA_H
#define ALPHABETA_H
#include "common.h"
#include "stopwatch_utils.h"

float alphabeta(int depth, float alpha, float beta, bool max_player);
float alphabeta_without_tt(int depth, float alpha, float beta, bool max_player);
float pvs(int depth, float alpha, float beta, bool max_player);
float pvs_no_tt(int depth, float alpha, float beta, bool max_player);
float abqs(int depth, float alpha, float beta, bool max_player);
float qs(int depth, float alpha, float beta, bool max_player);
//MostValuableVictim-LeastValuableAttacker-Sorting--Bubblesort
void bsMVV_LVA(field moves_from[], field moves_to[], int piece_idx[], int movecount);
#endif
