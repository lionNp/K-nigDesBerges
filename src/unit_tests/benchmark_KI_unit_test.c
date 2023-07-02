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

    for(; depth < 5; depth++){ 
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = !is_player_white;
            //rating[i] = (1 + (depth % 2) * tempo_bonus) * pvs_no_tt(depth, alpha, beta, max_player);
            if(depth < 2)
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
    int idx = 0;
    if(is_player_white)
        idx = max_rating(final_rating, move_count);
    else
        idx = min_rating(final_rating, move_count);
    print_move(moves_from[idx], moves_to[idx]);
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
    int idx = 0;
    if(is_player_white)
        idx = max_rating(final_rating, move_count);
    else
        idx = min_rating(final_rating, move_count);
    print_move(moves_from[idx], moves_to[idx]);
    float round_time = ((float) t / 1000000);
    return round_time;
}

float benchmark_nmp(char* fen_string){
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

    for(; depth < 5; depth++){ 
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = !is_player_white;
            rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta_nmp(depth, alpha, beta, max_player);
            is_player_white = !is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

            memcpy(castle_left,castle_flags_left,sizeof(castle_left));
            memcpy(castle_right,castle_flags_right,sizeof(castle_right));
        }

        for(int i = 0; i < move_count; i++)
            final_rating[i] = rating[i];

        sort_moves(final_rating, moves_from, moves_to, piece_idx, move_count);
        /*
        if(depth > 2){
            int null_move_pruning = 1;
            if(is_player_white){
                for(int i = 0; i < move_count; i++){
                    if(final_rating[0] - 2 < final_rating[i]){
                        null_move_pruning += i;
                        break;
                    }
                }
            }
            else{
                for(int i = 0; i < move_count; i++){
                    if(final_rating[0] + 2 > final_rating[i]){
                        null_move_pruning += i;
                        break;
                    }
                }
            }
            move_count = null_move_pruning;
        }
        */
        //filter_moves_quietsearch();
        t = stop_stopwatch(turn_time);
    }
    int idx = 0;
    if(is_player_white)
        idx = max_rating(final_rating, move_count);
    else
        idx = min_rating(final_rating, move_count);
    print_move(moves_from[idx], moves_to[idx]);
    
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
    int idx = 0;
    if(is_player_white)
        idx = max_rating(final_rating, move_count);
    else
        idx = min_rating(final_rating, move_count);
    print_move(moves_from[idx], moves_to[idx]);
    
    float round_time = ((float) t / 1000000);
    return round_time;
}

float benchmark_abqs(char* fen_string){
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

    for(; depth < 5; depth++){ 
        for(int i = 0; i < move_count; i++){
            field captured[8] = {0UL};
            bool castle_flags_left[2];
            bool castle_flags_right[2];

            memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
            memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = !is_player_white;
            rating[i] = (1 + (depth % 2) * tempo_bonus) * abqs(depth, alpha, beta, max_player);
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
    int idx = 0;
    if(is_player_white)
        idx = max_rating(final_rating, move_count);
    else
        idx = min_rating(final_rating, move_count);
    print_move(moves_from[idx], moves_to[idx]);
    
    float round_time = ((float) t / 1000000);
    return round_time;
}

/*
void main(){
    char* fen_1 = "r2qk2r/p1ppn1pp/bpnb1p2/4p3/4P3/2NPBN2/PPP1BPPP/R2Q1RK1 w Qkq";
    char* fen_2 = "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq";
    char* fen_3 = "r3r1k1/p4ppp/2Q1b3/4N3/5q2/4RP2/PPB3PP/R5K1 w - -";
    float ab;
    float tt;
    float pvs;
    float nmp;

    printf("%s\n", fen_1);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running AB\n");
    ab = benchmark_ab(fen_1);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running TT\n");
    tt = benchmark_tt(fen_1);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running PVS\n");
    pvs = benchmark_pvs(fen_1);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running NMP\n");
    nmp = benchmark_nmp(fen_1);
    printf("PVS (%fs) is %f times faster than AB (%fs).\n", pvs, ab/pvs, ab);
    printf("Transp. Tables (%fs) are %f times faster than AB (%fs).\n", tt, ab/tt, ab);
    printf("NMP (%fs) is %f times faster than AB (%fs).\n\n", nmp, ab/nmp, ab);

    printf("%s\n", fen_2);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running AB\n");
    ab = benchmark_ab(fen_2);
    printf("Running TT\n");
    tt = benchmark_tt(fen_2);
    printf("Running PVS\n");
    pvs = benchmark_pvs(fen_2);
    printf("Running NMP\n");
    nmp = benchmark_nmp(fen_2);
    printf("PVS (%fs) is %f times faster than AB (%fs).\n", pvs, ab/pvs, ab);
    printf("Transp. Tables (%fs) are %f times faster than AB (%fs).\n", tt, ab/tt, ab);
    printf("NMP (%fs) is %f times faster than AB (%fs).\n\n", nmp, ab/nmp, ab);

    printf("%s\n", fen_3);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running AB\n");
    ab = benchmark_ab(fen_3);
    printf("Running TT\n");
    tt = benchmark_tt(fen_3);
    printf("Running PVS\n");
    pvs = benchmark_pvs(fen_3);
    printf("Running NMP\n");
    nmp = benchmark_nmp(fen_3);
    printf("PVS (%fs) is %f times faster than AB (%fs).\n", pvs, ab/pvs, ab);
    printf("Transp. Tables (%fs) are %f times faster than AB (%fs).\n", tt, ab/tt, ab);
    printf("NMP (%fs) is %f times faster than AB (%fs).\n\n", nmp, ab/nmp, ab);
}
*/

void main(){
    char* fen_1 = "r2qk2r/p1ppn1pp/bpnb1p2/4p3/4P3/2NPBN2/PPP1BPPP/R2Q1RK1 w Qkq";
    char* fen_2 = "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq";
    char* fen_3 = "r3r1k1/p4ppp/2Q1b3/4N3/5q2/4RP2/PPB3PP/R5K1 w - -";
    float abqs;
    float ab;
    float tt;
    float pvs;
    float nmp;

    printf("%s\n", fen_1);
    memset(hash_table, 0, hash_prime * sizeof(float_t));
    printf("Running AB\n");
    ab = benchmark_ab(fen_1);
    printf("Running QS\n");
    abqs = benchmark_abqs(fen_1);
    printf("QS (%fs) is %f times faster than AB (%fs).\n\n", abqs, ab/abqs, ab);
    printf("Running AB\n");
    ab = benchmark_ab(fen_2);
    printf("Running QS\n");
    abqs = benchmark_abqs(fen_2);
    printf("QS (%fs) is %f times faster than AB (%fs).\n\n", abqs, ab/abqs, ab);
    printf("Running AB\n");
    ab = benchmark_ab(fen_3);
    printf("Running QS\n");
    abqs = benchmark_abqs(fen_3);
    printf("QS (%fs) is %f times faster than AB (%fs).\n\n", abqs, ab/abqs, ab);
}