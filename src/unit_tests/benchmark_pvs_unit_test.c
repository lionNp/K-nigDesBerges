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
#include <string.h>

float benchmark_pvs(char* fen_string){
    //initilize board
    import_gamestring(bitfields, fen_string);

    int count_total_moves = 0;
    // total match duration
    stopwatch total_time = start_stopwatch();
    //time
    stopwatch turn_time = start_stopwatch();
    field t = 0UL;
    //initilizing moves
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    //alpha beta initilizing
    float alpha = losing_move;
    float beta = winning_move;
    bool max_player = is_player_white;
    //get moves and set rating
    int move_count = generate_moves(moves_from, moves_to, piece_idx);

    float rating[move_count];
    float final_rating[move_count];
    
    int depth = 0;
    num_moves_trans = 0;
    num_moves_iterated = 0;
    float score;

    for(; depth < 5; depth++){ 
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = !is_player_white;
            if(depth < 3)
                rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta_without_tt(depth, alpha, beta, max_player);
            else
                rating[i] = (1 + (depth % 2) * tempo_bonus) * pvs_no_tt(depth, alpha, beta, max_player);
            is_player_white = !is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));
        }

        for(int i = 0; i < move_count; i++)
            final_rating[i] = rating[i];
        sort_moves(final_rating, moves_from, moves_to, piece_idx, move_count);
        t = stop_stopwatch(turn_time);
    }
    float round_time = ((float) t / 1000000);
    return round_time;
}

float benchmark_tt(char* fen_string){
        //initilize board
    import_gamestring(bitfields, fen_string);

    int count_total_moves = 0;
    // total match duration
    stopwatch total_time = start_stopwatch();
    //time
    stopwatch turn_time = start_stopwatch();
    field t = 0UL;
    //initilizing moves
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    //alpha beta initilizing
    float alpha = losing_move;
    float beta = winning_move;
    bool max_player = is_player_white;
    //get moves and set rating
    int move_count = generate_moves(moves_from, moves_to, piece_idx);

    float rating[move_count];
    float final_rating[move_count];
    
    int depth = 0;
    num_moves_trans = 0;
    num_moves_iterated = 0;
    float pv_score = 0.0f;
    float score;

    for(; depth < 5; depth++){ 
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = !is_player_white;
            rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta(depth, alpha, beta, max_player);
            is_player_white = !is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));
        }

        for(int i = 0; i < move_count; i++)
            final_rating[i] = rating[i];

        //sort the moves depending on rating
        sort_moves(final_rating, moves_from, moves_to, piece_idx, move_count);
        //filter_moves_quietsearch();
        t = stop_stopwatch(turn_time);
    }
    float round_time = ((float) t / 1000000);
    return round_time;
}

float benchmark_ab(char* fen_string){
    //initilize board
    import_gamestring(bitfields, fen_string);

    int count_total_moves = 0;
    // total match duration
    stopwatch total_time = start_stopwatch();
    //time
    stopwatch turn_time = start_stopwatch();
    field t = 0UL;
    //initilizing moves
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    //alpha beta initilizing
    float alpha = losing_move;
    float beta = winning_move;
    bool max_player = is_player_white;
    //get moves and set rating
    int move_count = generate_moves(moves_from, moves_to, piece_idx);

    float rating[move_count];
    float final_rating[move_count];
    
    int depth = 0;
    num_moves_trans = 0;
    num_moves_iterated = 0;
    float pv_score = 0.0f;
    float score;

    for(; depth < 5; depth++){ 
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = !is_player_white;
            rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta_without_tt(depth, alpha, beta, max_player);
            is_player_white = !is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));
        }

        for(int i = 0; i < move_count; i++)
            final_rating[i] = rating[i];

        //sort the moves depending on rating
        sort_moves(final_rating, moves_from, moves_to, piece_idx, move_count);
        //filter_moves_quietsearch();
        t = stop_stopwatch(turn_time);
    }
    float round_time = ((float) t / 1000000);
    return round_time;
}

void main(){
    char* fen_1 = "r2qk2r/p1ppn1pp/bpnb1p2/4p3/4P3/2NPBN2/PPP1BPPP/R2Q1RK1 w Qkq";
    char* fen_2 = "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq";
    char* fen_3 = "r3r1k1/p4ppp/2Q1b3/4N3/5q2/4RP2/PPB3PP/R5K1 w - -";
    printf("Running PVS\n");
    float pvs = benchmark_pvs(fen_1);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running TT\n");
    float tt = benchmark_tt(fen_1);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running AB\n");
    float ab = benchmark_ab(fen_1);
    printf("PVS (%f) is %f times faster than AB (%f).\n", pvs, ab/pvs, ab);
    printf("Transp. Tables (%f) are %f times faster than AB (%f).\n\n", tt, ab/tt, ab);

    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running PVS\n");
    pvs = benchmark_pvs(fen_2);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running TT\n");
    tt = benchmark_tt(fen_2);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running AB\n");
    ab = benchmark_ab(fen_2);
    printf("PVS (%f) is %f times faster than AB (%f).\n", pvs, ab/pvs, ab);
    printf("Transp. Tables (%f) are %f times faster than AB (%f).\n\n", tt, ab/tt, ab);

    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running PVS\n");
    pvs = benchmark_pvs(fen_3);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running TT\n");
    tt = benchmark_tt(fen_3);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running AB\n");
    ab = benchmark_ab(fen_3);
    printf("PVS (%f) is %f times faster than AB (%f).\n", pvs, ab/pvs, ab);
    printf("Transp. Tables (%f) are %f times faster than AB (%f).\n\n", tt, ab/tt, ab);
}