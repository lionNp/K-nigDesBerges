#ifndef MCTS_H
#define MCTS_H
#include "common.h"

typedef struct mcts_node {
    int pv;
    field from;
    field to;
    int idx;
    float rating;
    struct mcts_node * parent; 
    struct mcts_node * next;
} node;

float rollout();
float mcts(node *parent, int depth);

#endif
