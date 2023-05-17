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

int main() {
    //initilize board
    import_gamestring(bitfields, game_string);

    int count_total_moves = 0;
    // total match duration
    stopwatch total_time = start_stopwatch();
    field match_duration;

    while(!gameover)
    //for(int r = 0; r < 1; r++)
    { 
        //time
        stopwatch turn_time = start_stopwatch();
        field t = 0UL;
        //initilizing moves
        field captured[8];
        field moves_from[max_move_count];
        field moves_to[max_move_count];
        int piece_idx[max_move_count];
        //alpha beta initilizing
        float alpha = -9999.0f;
        float beta = 9999.0f;
        bool max_player = is_player_white;
        //get moves and set rating
        int move_count = generate_moves(moves_from, moves_to, piece_idx);
        float rating[move_count];

        if(move_count == 0){
            printf("No more moves\n");
            gameover = true;
            break;
        }

        for(int depth = 0; depth < 3; depth++){   //t < 1000
            for(int i = 0; i < move_count; i++){
                make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
                is_player_white = !is_player_white;
                rating[i] = alphabeta(depth, alpha, beta, max_player); //alphabeta(depth, -9999.0f, rating[i]);
                is_player_white = !is_player_white;
                unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            }
            t = stop_stopwatch(turn_time);
            //printf("time: %ldμs at depth: %d with %d moves\n", t, depth, move_count);
        }

        int idx = random_max_rating(rating, move_count);
        //print_full_board();
        // make move
        make_move(piece_idx[idx], moves_from[idx], moves_to[idx], captured);
        hashset_add(bitfields[is_player_white] ^ bitfields[!is_player_white]);
        //print_full_board();
        // print results
        //switch sides
        count_total_moves++;
        gameover = game_finished(move_count);
        if(gameover){
            field match_duration = stop_stopwatch(total_time);
            printf("Game Over\n");
            print_full_board();
            printf("Match took %d moves took %luμs\n", count_total_moves, match_duration);
            return 0;
        }
        is_player_white = 1 - is_player_white;
    }
    if(gameover){
            field tt = stop_stopwatch(total_time);
            printf("Game Over\n");
            print_full_board();
            printf("Match took %d moves took %luμs\n", count_total_moves, tt);
        }
    return 0;
}

// test in main ? :(
void test_add_to_board_coords(){
    field board = (field) 0;

    printf("board:\n");
    print_board(board);
    printf("add to board at piece_index=3, y=2\n");
    add_to_board_coords(&board, 3, 4);
    print_board(board);
}

void test_bitfield_print(){
    field bitfield = (field) 0b1000101001100001111100011010001001011111001100011001000111111000;
    print_board(bitfield);
}
