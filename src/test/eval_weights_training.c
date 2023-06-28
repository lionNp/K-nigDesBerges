#include "main.h"
#include <math.h>
#include "common.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include "evaluation.h"
#include "stopwatch_utils.h"
#include "move_generator.h"
#include "move_executer.h"
#include <stdlib.h>
#include "hashset.h"
#include "alphabeta.h"
#include <time.h>

void print_all_learing_weights();
void use_learing_weights();
void clear_bitfields();
void step_back_learing_weights();
void set_learning_weight(float* weight_to_adjust);
void get_fitting();
void set_all_learing_weights();
void select_leaning_weights();
void select_last_best_weights();

//globals for training
float eval;
float stock_eval;
float fitting = 0.0f;
float tmp_fitting;
float learning_rate = 0.0003f;

long total_fittings_tested = 0;

//minimum fitting that all sets need to be achieve similtaniously before termination
float all_fitting_precision = 0.98f;

float learning_material_modify = 20;
float learning_position_modify = 1;
float learning_contol_modify = 2;
float learning_pawns_modify = 1;
float learning_king_safety_modify = 1;

float bef_learning_material_modify = 20;
float bef_learning_position_modify = 1;
float bef_learning_contol_modify = 2;
float bef_learning_pawns_modify = 1;
float bef_learning_king_safety_modify = 1;

//std weight list for reference
/*
float material_modify = 20;
float position_modify = 1;
float contol_modify = 2;
float pawns_modify = 1;
float king_safety_modify = 1;
*/

#define training_runs 20
#define iteration_depth 1
#define learing_player 0

void main()
{
    //config for random number laster, rand() returns random integer
    srand(time(NULL));

    int learn_ct = 0;
    //while 
    for(int z=0; z<training_runs; z++)
    {
        //set new learing weights
        set_all_learing_weights();

        //play game
        import_gamestring(bitfields, game_string);
        memset(hash_table, 0, hash_prime * sizeof(float_t));

        int count_total_moves = 0;
        int total_pieces = 32;

        gameover = false;
        while(!gameover)
        {
            //use respective weights
            if(is_player_white == learing_player)
            {
                select_leaning_weights();
            }
            else
            {
                select_last_best_weights();
            }

            //printf(".");
            field t = 0UL;

            field moves_from[max_move_count];
            field moves_to[max_move_count];
            int piece_idx[max_move_count];

            float alpha = losing_move;
            float beta = winning_move;
            bool max_player = is_player_white;

            int move_count = generate_moves(moves_from, moves_to, piece_idx);
            
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            float rating[move_count];
            float final_rating[move_count];
            
            if(move_count == 0){
                //nothing special for now
                printf("no moves \n");
                break;
            }

            int break_after = default_time_per_move;
            
            int depth = 0;
            num_moves_iterated = 0;
            num_moves_trans = 0;

            float pv_score = 0.0f;
            float score;

            for(; depth <= iteration_depth; depth++){ 
                for(int i = 0; i < move_count; i++){

                    field captured[8] = {0UL};

                    memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
                    memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

                    make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
                    is_player_white = !is_player_white;

                    if(depth < 3)
                        rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta(depth, alpha, beta, max_player);
                    else
                        rating[i] = (1 + (depth % 2) * tempo_bonus) * pvs(depth, alpha, beta, max_player);

                    is_player_white = !is_player_white;
                    unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

                    memcpy(castle_left,castle_flags_left,sizeof(castle_left));
                    memcpy(castle_right,castle_flags_right,sizeof(castle_right));
                }

                for(int i = 0; i < move_count; i++)
                    final_rating[i] = rating[i];

                //sort the moves depending on rating
                sort_moves(final_rating, moves_from, moves_to, piece_idx, move_count);
            }

            int idx = 0;
            if(is_player_white)
                idx = max_rating(final_rating, move_count);
            else
                idx = min_rating(final_rating, move_count);
            
            //make move
            field captured[8] = {0UL};
            castle_flags(piece_idx[idx], moves_from[idx]);
            make_move(piece_idx[idx], moves_from[idx], moves_to[idx], captured);

            for (int i =0; i<8; i++)
            {
                if(captured[i] != 0UL)
                {
                    memset(hash_table, 0, hash_prime * sizeof(float_t));
                    break;
                }
            }
            // ######################

            hashset_add(bitfields[is_player_white] | bitfields[!is_player_white]);
            // save move in struct
            // struct includes current hashtable

            count_total_moves++;
            gameover = game_finished(move_count);
            if(gameover){
                printf("game %d finished\n", z);
                // learn from match if learnee won
                if(is_player_white == learing_player)
                {   
                    printf("i learned ");
                    use_learing_weights();
                    printf("new weights:\n");
                    print_all_learing_weights();

                    learn_ct++;
                }
            }

            is_player_white = 1 - is_player_white;
        }
    }   

    printf("run completed.\nfinal weights after %d times learned:\n", learn_ct);
    print_all_learing_weights();

    return;  
}

void select_leaning_weights()
{
    material_modify = learning_material_modify;
    position_modify = learning_position_modify;
    contol_modify = learning_contol_modify;
    pawns_modify = learning_pawns_modify;
    king_safety_modify = learning_king_safety_modify;
}

void select_last_best_weights()
{
    material_modify = bef_learning_material_modify;
    position_modify = bef_learning_position_modify;
    contol_modify = bef_learning_contol_modify;
    pawns_modify = bef_learning_pawns_modify;
    king_safety_modify = bef_learning_king_safety_modify;
}

void set_all_learing_weights()
{
    set_learning_weight(&learning_material_modify);
    set_learning_weight(&learning_position_modify);
    set_learning_weight(&learning_contol_modify);
    set_learning_weight(&learning_pawns_modify);
    set_learning_weight(&learning_king_safety_modify);
}

void print_all_learing_weights()
{
    printf("material_modify: %f\n",  learning_material_modify);
    printf("position_modify: %f\n", learning_position_modify);
    printf("contol_modify: %f\n", learning_contol_modify);
    printf("pawns_modify: %f\n", learning_pawns_modify);
    printf("king_safety_modify: %f\n\n", learning_king_safety_modify);
}

void use_learing_weights()
{
    bef_learning_material_modify = learning_material_modify;
    bef_learning_position_modify = learning_position_modify;
    bef_learning_contol_modify = learning_contol_modify;
    bef_learning_pawns_modify = learning_pawns_modify;
    bef_learning_king_safety_modify = learning_king_safety_modify;
}

void step_back_learing_weights()
{
    material_modify = bef_learning_material_modify;
    position_modify = bef_learning_position_modify;
    contol_modify = bef_learning_contol_modify;
    pawns_modify = bef_learning_pawns_modify;
    king_safety_modify = bef_learning_king_safety_modify;
}

void set_learning_weight(float* weight_to_adjust)
{
    int range = 100;

    float random_factor = (float) (rand() % range + 1) * learning_rate;

    if(rand()%2 == 0)
        *weight_to_adjust = *weight_to_adjust + random_factor;
    else
        *weight_to_adjust = *weight_to_adjust - random_factor;
}

void clear_bitfields()
{
    for(int i=0; i<8; i++)
    {
        bitfields[i] = 0UL;
    }
}

void get_fitting()
{
    //fitting as percentage to run regression on
    if((stock_eval < 0.0f && eval > 0.0f) || (stock_eval > 0.0f && eval < 0.0f))
    {
        tmp_fitting = 0.0f;
        return;
    }

    //positive
    if(eval > 0.0f && stock_eval > 0.0f)
    {
        tmp_fitting = stock_eval / eval;
        if(eval<stock_eval)
            tmp_fitting = eval/stock_eval;
    }
    //negative
    else if(eval < 0.0f && stock_eval < 0.0f)
    {
        tmp_fitting = stock_eval / eval;
        if(eval > stock_eval)
            tmp_fitting = eval/stock_eval;
    }
    //if one fitting is 0 and cant be used for division
    else{
        tmp_fitting = 0.0f;
    }

}