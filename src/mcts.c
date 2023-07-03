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
    struct mcts_node parent;// count visited parent
    int  n = 0;             // count visited node
    bool terminal = false;  // node game over
};


float mcts(int depth, float alpha, float beta, bool max_player){
    if(depth == 0){
        num_moves_iterated++;
        field hashtable_value = hash_boards();
        float eval = hash_table[hashtable_value];
        if(eval == 0.0f){
            num_moves_trans++;
            eval = evaluation();
        }
        hash_table[hashtable_value] = eval;
        return eval;
    }
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    float rating[max_move_count];

    int move_count = generate_moves(moves_from, moves_to, piece_idx);
    
    if(move_count == 0){
        num_moves_iterated++;
        if(is_player_white)
            return losing_move;
        else
            return winning_move;
    }

    float score = 0.0f;
    if(is_player_white){
        //iterate over every moveset for a piece
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = 1 - is_player_white;
            score = alphabeta(depth - 1, alpha, beta, max_player);
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));
            alpha = fmax(alpha, score);
            if(alpha > beta)
                break;
            
        }
        return alpha;
    }else{
        //iterate over every moveset for a piece
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = 1 - is_player_white;
            score = alphabeta(depth - 1, alpha, beta, max_player);
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));
            beta = fmin(beta, score);
            if(beta < alpha)
                break;
            
        }
        return beta;
    }
}