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
        if(hash_table[hashtable_value] == 0.0f){
            num_moves_trans++;
            hash_table[hashtable_value] = evaluation();
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

            if(alpha > beta)
                break;
            alpha = fmax(alpha, score);
        }
        //printf("Max_Alpha: %f Max_Beta: %f\n", alpha, beta);
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

            if(beta < alpha)
                break;
            beta = fmin(beta, score);
        }
        //printf("Min_Alpha: %f Min_Beta: %f\n", alpha, beta);
        return beta;
    }
}
    

float pvs(int depth, float alpha, float beta, bool max_player){
    //printf("%d\n", depth);
    if(depth == 0){
        num_moves_iterated++;
        field hashtable_value = hash_boards();
        if(hash_table[hashtable_value] == 0.0f){
            num_moves_trans++;
            hash_table[hashtable_value] = evaluation();
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

    float score = 0.0f;
    //iterate over every moveset for a piece
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
            if(i == 0)
                score = alphabeta(depth - 1, alpha, beta, max_player);
            else{
                score = alphabeta(depth - 1, -alpha - 1, beta, max_player);
                if(score > alpha && score < beta)
                    score = alphabeta(depth - 1, alpha, -score, max_player);
            }
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));

            alpha = fmax(alpha, score);
            if(alpha >= beta)
                break;
        }
        //printf("Max_Alpha: %f Max_Beta: %f\n", alpha, beta);
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
            if(i == 0)
                score = alphabeta(depth - 1, alpha, beta, max_player);
            else{
                score = alphabeta(depth - 1, -beta, -beta - 1, max_player);
                if(score > alpha && score < beta)
                    score = alphabeta(depth - 1, -score, beta, max_player);
            }
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));

            beta = fmin(beta, score);
            if(beta <= alpha)
                break;
        }
        //printf("Min_Alpha: %f Min_Beta: %f\n", alpha, beta);
        return beta;
    }
}