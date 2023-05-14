#include "move_util.h"
#include "bit_boards_util.h"
#include "common.h"
#include "evaluation.h"
#include "move_executer.h"
#include "move_generator.h"
#include <math.h>
#include "alphabeta.h"
#include "stopwatch_utils.h"

float alphabeta(int depth, stopwatch time){
    //printf("%d\n", depth);
    if(depth == 0)
        return 0;
    field t = 0UL;
    field moves_from[max_move_count] = {0UL};
    field moves_to[max_move_count] = {0UL};
    int piece_idx[max_move_count] = {0};
    float rating[max_move_count] = {0.0f};
    int move_count = generate_moves(moves_from, moves_to, piece_idx);
    for(int d = depth; d > 0 || t < 10000; d--){
        field captured[8];
        //iterate over every moveset for a piece
        for(int i = 0; i < move_count; i++){
            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            rating[i] = evaluation(moves_from[i], moves_to[i], piece_idx[i]);
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        }
        for(int i = 0; i < d; i++){
            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = 1 - is_player_white;
            rating[i] -= alphabeta(d - 1, time);
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        }
        t = stop_stopwatch(time);
    }
    int max_idx = 0;
    for(int i = 0; i < move_count; i++){
        if(rating[max_idx] < rating[i])
            max_idx = i;
    }
    return rating[max_idx];
}
    