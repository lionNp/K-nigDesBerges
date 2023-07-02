#include "move_util.h"
#include "bit_boards_util.h"
#include "common.h"
#include "evaluation.h"
#include "move_executer.h"
#include "move_generator.h"
#include <math.h>
#include "alphabeta.h"
#include "stopwatch_utils.h"
#include <string.h>
#include "hash_boards.h"

typedef struct mcts_node {
    field board_state[8];
    struct mcts_node children[];
    int   n_parent = 0;   // count visited parent
    int   n = 0;          // count visited node
    float v = 0;          // exploitation factor
    bool terminal = false;
};


float mcts(int depth, float alpha, float beta, bool max_player){
    
}