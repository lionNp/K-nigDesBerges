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

int best_move_for_position(char* fen_string) {
    //initilize board
    import_gamestring(bitfields, fen_string);
    print_full_board();

    int count_total_moves = 0;
    // total match duration
    stopwatch total_time = start_stopwatch();
    field match_duration;

    //reset moves iterated though
    num_moves_iterated = 0;

    for(int r = 0; r < 1; r++)
    {    
        //time
        stopwatch turn_time = start_stopwatch();
        field t = 0UL;
        //initilizing moves
        field moves_from[max_move_count] = {0UL};
        field moves_to[max_move_count] = {0UL};
        int piece_idx[max_move_count] = {0};
        //alpha beta initilizing
        float alpha = losing_move;
        float beta = winning_move;
        bool max_player = is_player_white;
        //get moves and set rating
        int move_count = generate_moves(moves_from, moves_to, piece_idx);
        for(int i = 0; i < move_count; i++)
            print_move(moves_from[i], moves_to[i]);
        if(move_count == 0)
        {
            printf("you lost!\n");
            return 0;
        }

        float rating[move_count];
        float final_rating[move_count];
        int break_after_ms = 1000000 * 10;
        int num_moves_full_depth = 0;
        int depth = 0;
        //shouldnt we increse depth in increments of 2, starting at 1? so as to not get half-false results
        for(; depth < 5; depth++){
            for(int i = 0; i < move_count; i++){
                field captured[8] = {0UL};
                make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
                is_player_white = !is_player_white;
                rating[i] = alphabeta(depth, alpha, beta, max_player);
                is_player_white = !is_player_white;
                unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            }

            //set final rating when whole depth is seached
            for(int i=0; i< move_count; i++)
                final_rating[i] = rating[i];

            //set number of moves seached
            num_moves_full_depth = num_moves_iterated;
            field run_time = stop_stopwatch(turn_time);
            printf("After %ldus passed,\n", run_time);
            printf("Moves: %d searched in depth %d\n", num_moves_iterated, depth);
            int idx = max_rating(final_rating, move_count);
            printf("This is our best move: ");
            print_move(moves_from[idx], moves_to[idx]);
        }

        

        //field captured[8] = {0UL};
        //make_move(piece_idx[idx], moves_from[idx], moves_to[idx], captured);

        //printf("After %d moves searched total (including partially searched depths),\n", num_moves_iterated);
        //printf("After %d moves searched not including partially searched depths,\n", num_moves_full_depth);
        

        //printf("Count total move: %d\n", move_count);

        is_player_white = 1 - is_player_white;
    }

    return 0;
}

void main()
{
    char* fen_1 = "r2qk2r/p1ppn1pp/bpnb1p2/4p3/4P3/2NPBN2/PPP1BPPP/R2Q1RK1 w Qkq";
    char* fen_2 = "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq";
    char* fen_3 = "r3r1k1/p4ppp/2Q1b3/4N3/5q2/4RP2/PPB3PP/R5K1 w - -";
    best_move_for_position(fen_3);
}