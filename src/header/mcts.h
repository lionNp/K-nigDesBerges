#ifndef MCTS_H
#define MCTS_H
#include "common.h"

typedef struct mcts_node {
    field board_state[8];
    struct mcts_node * children;
    int pv;
    struct mcts_node * parent;             // count visited parent
    int n;                     // count visited node
    bool terminal;          // node game over
    float rating;
} node;

float rollout();
float mcts(node *parent, int depth);

#endif
