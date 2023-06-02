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

float alphabeta(int depth, float alpha, float beta, bool max_player){
    //printf("%d\n", depth);
    if(depth == 0){
        num_moves_iterated++;
        field hashtable_value = hash_boards();
        //printf("%u\n", hashtable_value);
        if(hash_table[hashtable_value] == 0.0f){
            num_moves_trans++;
            hash_table[hashtable_value] = evaluation(max_player);
        }
        return hash_table[hashtable_value];
    }

    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    float rating[max_move_count];

    int move_count = generate_moves(moves_from, moves_to, piece_idx);

    if(move_count == 0){
        num_moves_iterated++;
        if(is_player_white == max_player)
            return losing_move;
        else
            return winning_move;
    }

    float value;
    if(is_player_white == max_player){
        //iterate over every moveset for a piece
        value = losing_move;
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];
            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = 1 - is_player_white;
            value = fmax(value, alphabeta(depth - 1, alpha, beta, max_player));
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));

            if(value > beta)
                break;
            alpha = fmax(alpha, value);
        }
        //printf("Max_Alpha: %f Max_Beta: %f\n", alpha, beta);
        return value;
    }else{
        //iterate over every moveset for a piece
        value = winning_move;
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];
            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = 1 - is_player_white;
            value = fmin(value, alphabeta(depth - 1, alpha, beta, max_player));
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));

            if(value < alpha)
                break;
            beta = fmin(beta, value);
        }
        //printf("Min_Alpha: %f Min_Beta: %f\n", alpha, beta);
        return value;
    }
}
    