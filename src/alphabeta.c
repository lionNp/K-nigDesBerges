#include "move_util.h"
#include "bit_boards_util.h"
#include "common.h"
#include "evaluation.h"
#include "move_executer.h"
#include "move_generator.h"
#include <math.h>
#include "alphabeta.h"
#include "stopwatch_utils.h"

float alphabeta(int depth){
    //printf("%d\n", depth);
    field captured[8];
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    float rating[max_move_count];
    int move_count = generate_moves(moves_from, moves_to, piece_idx);
    if(move_count == 0)
        return winning_move;
    for(int i = 0; i < move_count; i++){
        make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        rating[i] = evaluation(moves_from[i], moves_to[i], piece_idx[i]);
        unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
    }
    for(int d = depth; d > 0; d--){
        //iterate over every moveset for a piece
        for(int i = 0; i < move_count; i++){
            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = 1 - is_player_white;
            if(d % 2 == 0)
                rating[i] += alphabeta(d - 1);
            else
                rating[i] -= alphabeta(d - 1);
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        }
    }
    
    int idx = 0;
    for(int i = 0; i < move_count; i++){
        if(rating[idx] < rating[i])
            idx = i;
    }
    return rating[idx];
}
    