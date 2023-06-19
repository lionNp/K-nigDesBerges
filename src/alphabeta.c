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

float alphabeta_without_tt(int depth, float alpha, float beta, bool max_player){
    if(depth == 0){
        num_moves_iterated++;
        return evaluation();
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
                score = alphabeta(depth - 1, beta, beta - 1, max_player);
                if(score > alpha && score < beta)
                    score = alphabeta(depth - 1, alpha, score, max_player);
            }
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
            if(i == 0)
                score = alphabeta(depth - 1, alpha, beta, max_player);
            else{
                score = alphabeta(depth - 1, alpha + 1, alpha, max_player);
                if(score > alpha && score < beta)
                    score = alphabeta(depth - 1, score, beta, max_player);
            }
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

float pvs_no_tt(int depth, float alpha, float beta, bool max_player){
    //printf("%d\n", depth);
    if(depth == 0){
        num_moves_iterated++;
        return evaluation();
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
    for(int i = 0; i < move_count; i++){
        field captured[8] = {0UL};
        bool castle_flags_left[2];
        bool castle_flags_right[2];
        memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
        memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

        make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        is_player_white = 1 - is_player_white;
        if(i == 0)
            score = alphabeta_without_tt(depth - 1, -beta, -alpha, max_player);
        else{
            score = alphabeta_without_tt(depth - 1, -alpha - 10, -alpha, max_player);
            if(score > alpha && score < beta)
                score = alphabeta_without_tt(depth - 1, -beta, score, max_player);
        }
        is_player_white = 1 - is_player_white;
        unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

        memcpy(castle_left,castle_flags_left,sizeof(castle_left));
        memcpy(castle_right,castle_flags_right,sizeof(castle_right));
        
        alpha = fmax(alpha, score);
        if(alpha >= beta)
            break;
    }
    return alpha;
}

float abqs(int depth, float alpha, float beta, bool max_player){
    //printf("%d\n", depth);
    if(depth == 0){
        num_moves_iterated++;
        field hashtable_value = hash_boards();
        if(hash_table[hashtable_value] == 0.0f){
            num_moves_trans++;
            hash_table[hashtable_value] = qs(0, alpha, beta, max_player);
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
            score = (1 + (depth % 2) * tempo_bonus) * alphabeta(depth - 1, alpha, beta, max_player);
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
            score = (1 + (depth % 2) * tempo_bonus) * alphabeta(depth - 1, alpha, beta, max_player);
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

float qs(int depth, float alpha, float beta, bool max_player){    
    field enemy = bitfields[white];
    field promotion = rank_1;
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    float rating[max_move_count];
    //recursion anchor
    float stand_pat = evaluation();

    if(stand_pat >= beta)
        return beta;
    if(stand_pat > alpha)
        alpha = stand_pat;

    if(is_player_white){
        enemy = bitfields[black];
        promotion = rank_8;
    }
    if(depth < 5){
        //filter non capture moves
        //TODO: include checks
        int move_count = generate_moves(moves_from, moves_to, piece_idx);
        int removed = 0;
        for(int i = 0; i < move_count; i++){
            /*pawn promotion 
            if(get_piece_id(moves_from[i] == 7)
            && (moves_to[i] & promotion > 0))
            {}*/
            //non capture -> filtered
            if((enemy & moves_to[i]) == 0){
                removed++;
                piece_idx[i] = -100;
            }
            //losing capture -> filtered
            else if(get_piece_id(moves_to[i]) > get_piece_id(moves_from[i])){
                piece_idx[i] = -100;
                removed++;
            }
        }

        //sorting
        bsMVV_LVA(moves_from, moves_to, piece_idx, move_count);

        float value;
        float score = 0.0f;
        //iterate over filtered moves
        for(int i = 0; i < move_count - removed; i++){
            if(piece_idx[i] != -100)
                continue;
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = 1 - is_player_white;
            
            score = qs(depth+1, alpha, beta, max_player);
            is_player_white = 1 - is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));

            if(is_player_white){
                if(alpha > beta)
                    break;
                alpha = fmax(alpha, score);
            }
            else if(beta < alpha){
                    break;
                beta = fmin(beta, score);
            }
        }
    }
    if(is_player_white)
        return alpha;
    return beta;
}


void bsMVV_LVA(field moves_from[], field moves_to[], int piece_idx[], int movecount){
    field temp;
    int tempIdx = 0;
    int change = 0;

    while(change > 0){
        for(int i = 0; i < movecount-1; i++){
            change = 0;
            int to = get_piece_id(moves_to[i]);
            int toNext = get_piece_id(moves_to[i+1]);
            //check most valuable target
            if(to > toNext                                          //better target smaller id
            || (piece_idx[i] == -100 && (piece_idx[i+1] != -100))){ //both filtered
                temp = moves_to[i+1];
                moves_to[i+1] = moves_to[i];
                moves_to[i] = temp;
                temp = moves_from[i+1];
                moves_from[i+1] = moves_from[i];
                moves_from[i] = temp;
                tempIdx = piece_idx[i];
                piece_idx[i+1] = piece_idx[i];
                piece_idx[i] = tempIdx;
                change++;
            }
            //check equal target - least valuable attacker
            else if(to == toNext 
                && get_piece_id(moves_from[i]) > get_piece_id(moves_from[i+1])){
                    temp = moves_from[i+1];
                    moves_from[i+1] = moves_from[i];
                    moves_from[i] = temp;
                    change++;
            }
        }
    }
}
