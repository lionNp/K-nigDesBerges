#ifndef ALPHABETA_H
#define ALPHABETA_H
#include "common.h"
#include "stopwatch_utils.h"

float alphabeta(int depth, float alpha, float beta, bool max_player);
float alphabeta_without_tt(int depth, float alpha, float beta, bool max_player);
float pvs(int depth, float alpha, float beta, bool max_player);
float pvs_no_tt(int depth, float alpha, float beta, bool max_player);
#endif