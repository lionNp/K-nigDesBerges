#ifndef MCTS_H
#define MCTS_H
#include "common.h"

typedef struct mcts_node {
    field board_state[8];
    int pv;
    field from;
    field to;
    int idx;
    struct mcts_node * parent; 
    struct mcts_node * next;
    float rating;
} node;

float rollout();
float mcts(node *parent, int depth);

#endif
