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
        field moves_from[max_move_count];
        field moves_to[max_move_count];
        int piece_idx[max_move_count];
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
        int k = 0;

        int num_moves_full_depth = 0;

        //shouldnt we increse depth in increments of 2, starting at 1? so as to not get half-false results
        for(int depth = 0; depth < 3; depth++){ //t < break_after_ms
            for(int i = 0; i < move_count; i++){    // <- t < 1 000 000
                //if(stop_stopwatch(turn_time) > break_after_ms) break;
                field captured[8] = {0UL};
                make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
                is_player_white = !is_player_white;
                rating[i] = alphabeta(depth, alpha, beta, max_player);
                is_player_white = !is_player_white;
                unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
                // <- put stopwatch here?
            }

            //break if subtree took too long
            //t = stop_stopwatch(turn_time);
            //printf("time: %ldμs at depth: %d\n", t, depth);
            //if (t > break_after_ms)
              //  break;

            //set final rating when whole depth is seached
            for(int i=0; i< move_count; i++)
                final_rating[i] = rating[i];

            //set number of moves seached
            num_moves_full_depth = num_moves_iterated;

            //printf("Depth: %d", k);
            //k++;
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
    char* fen = "r3k1r1/2pp1p2/bpn1pqpn/p1b4p/6P1/BP1P1P1N/P1PQP2P/RN2KB1R b KQ - 0 1";
    char* best_move = "dunno";
    printf("For board %s\nthe best possible move is %s.\nOur programm returned:\n", fen, best_move);
    best_move_for_position(fen);    


    // #################### others above ###################

    /*char* fen = "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq - 0 1";
    char* best_move = "e5 -> f7";
    printf("For board %s\nthe best possible move is %s.\nOur programm returned:\n", fen, best_move);
    best_move_for_position(fen);
    
    printf("\n\n");

    fen = "rnbk1r2/pp3p1p/1qp5/3pQ3/8/5N1P/PP2KbP1/R1B2B1R w HAq - 0 1";
    best_move = "c1 -> g5";
    printf("For board %s\nthe best possible move is %s.\nOur programm returned:\n", fen, best_move);
    best_move_for_position(fen);

    printf("\n\n");

    fen = "1k1r4/2p5/4B3/p4R2/1p3q1p/7P/PPQ1K3/8 b - - 0 1";
    best_move = "f4-h2";
    printf("For board %s\nthe best possible move is %s.\nOur programm returned:\n", fen, best_move);
    best_move_for_position(fen);
    
    printf("\n\n");

    fen = "3rr1k1/1pp2ppp/p2b4/1b1qP3/1P2p3/P1Q1P3/1B1N1PPP/2R1R1K1 b - - 0 1b";
    best_move = "d6-e5";
    printf("For board %s\nthe best possible move is %s.\nOur programm returned:\n", fen, best_move);
    best_move_for_position(fen);*/
}