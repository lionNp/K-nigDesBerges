#ifndef MCTS_H
#define MCTS_H
#include "common.h"

typedef struct mcts_node {
    field board_state[8];
    node **children;
    node *parent;             // count visited parent
    int n;                     // count visited node
    bool terminal;          // node game over
    float rating;
} node;

float rollout();
float ucb();
float mcts(node parent);

#endif
