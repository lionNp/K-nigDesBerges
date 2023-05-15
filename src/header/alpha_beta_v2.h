#ifndef A_B_V2
#define A_B_V2

#include "common.h"
#include <limits.h>

typedef struct s_node {
    int piece_type;
    field piece_from;
    field piece_to;
    float rating;
} ab_node;

void alpha_beta_search(ab_node* node, int depth, float alpha, float beta, bool is_maxing);

#endif