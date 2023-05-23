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

    int count_total_moves = 0;
    // total match duration
    stopwatch total_time = start_stopwatch();
    field match_duration;

    
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

    //shouldnt we increse depth in increments of 2, starting at 1? so as to not get half-false results
    for(int depth = 0; t < 1000000; depth++){ //t < 1000
        for(int i = 0; i < move_count; i++){    // <- t < 1 000 000
            field captured[8] = {0UL};
            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            is_player_white = !is_player_white;
            rating[i] = alphabeta(depth, alpha, beta, max_player);
            is_player_white = !is_player_white;
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            // <- put stopwatch here?
        }
        t = stop_stopwatch(turn_time);
        printf("time: %ldμs at depth: %d\n", t, depth);
    }

    int idx = max_rating(rating, move_count);

    //best move in idx
    printf("next best move:\n");
    char from[3];
    print_position_as_spoken(moves_from[idx], from);
    printf("from: %s\n", from);
    //print_board(moves_from[idx]);
    char to[3];
    print_position_as_spoken(moves_to[idx], from);
    printf("to: %s\n", to);
    //print_board(moves_to[idx]);

    return 0;
}

void main()
{
    char* fen = "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq - 0 1";
    char* best_move = "e5 -> f7";
    printf(" For board %s\nthe best possible move is %s.\nOur programm returned:\n", fen, best_move);
    best_move_for_position(fen);

    fen = "rnbk1r2/pp3p1p/1qp5/3pQ3/8/5N1P/PP2KbP1/R1B2B1R w HAq - 0 1";
    best_move = "c1 -> g5";
    printf(" For board %s\nthe best possible move is %s.\nOur programm returned:\n", fen, best_move);
    best_move_for_position(fen);

}