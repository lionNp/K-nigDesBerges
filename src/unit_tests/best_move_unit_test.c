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
        if(move_count == 0)
        {
            printf("you lost!\n");
            return 0;
        }

        float rating[move_count];
        float final_rating[move_count];
        int break_after_ms = 1000000 * 10;
        int num_moves_full_depth = 0;

        //shouldnt we increse depth in increments of 2, starting at 1? so as to not get half-false results
        for(int depth = 0; depth < 3; depth++){ //t < break_after_ms
            for(int i = 0; i < move_count; i++){    // <- t < 1 000 000
                //if(stop_stopwatch(turn_time) > break_after_ms) break;
                field captured[8] = {0UL};
                bool castle_flags_left[2];
                bool castle_flags_right[2];
                memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
                memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

                make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
                is_player_white = !is_player_white;
                rating[i] = alphabeta(depth, alpha, beta, max_player);
                is_player_white = !is_player_white;
                unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

                memcpy(castle_left,castle_flags_left,sizeof(castle_left));
                memcpy(castle_right,castle_flags_right,sizeof(castle_right));
            }

            //set final rating when whole depth is seached
            for(int i=0; i< move_count; i++)
                final_rating[i] = rating[i];

            //set number of moves seached
            num_moves_full_depth = num_moves_iterated;
            printf("Moves: %d searched in depth %d\n", num_moves_iterated, depth);
        }

        int idx = max_rating(final_rating, move_count);

        field captured[8] = {0UL};
        make_move(piece_idx[idx], moves_from[idx], moves_to[idx], captured);

        field run_time = stop_stopwatch(turn_time);
        
        printf("After %ldms passed,\n", run_time/1000);
        //printf("After %d moves searched total (including partially searched depths),\n", num_moves_iterated);
        //printf("After %d moves searched not including partially searched depths,\n", num_moves_full_depth);
        printf("This is our best move: ");
        
        print_move(moves_from[idx], moves_to[idx]);

        //printf("Count total move: %d\n", move_count);

        is_player_white = 1 - is_player_white;
    }

    return 0;
}

void main()
{
    char* fen = "r1bq4/pp1p1k1p/2p2p1p/2b5/3Nr1Q1/2N1P3/PPPK1PPP/3R1B1R w - -";
    best_move_for_position(fen);
}