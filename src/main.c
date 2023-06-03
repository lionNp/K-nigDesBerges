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
    //for(int r = 0; r < 5; r++)
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

        float rating[move_count];
        float final_rating[move_count];
        if(move_count == 0){
            printf("No more moves\n");
            printf("%s won!\n", !is_player_white ? "white" : "black");
            gameover = true;
            break;
        }
        int break_after = 2000000;
        int depth = 0;
        num_moves_iterated = 0;
        num_moves_trans = 0;
        for(; t < break_after; depth++){
            for(int i = 0; i < move_count; i++){
                if(stop_stopwatch(turn_time) > break_after) break;
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
            for(int i = 0; i < move_count; i++)
                final_rating[i] = rating[i];
            t = stop_stopwatch(turn_time);
            //printf("After %ldus passed,\n", t);
            //printf("Moves: %d searched in depth %d\n", num_moves_iterated, depth);
        }
        float round_time = ((float) t / 1000000);
        printf("time: %fs at depth: %d\n", round_time, depth);
        printf("Total moves: %d --- Evaluated Moves: %d%%\n", num_moves_iterated, 100 * num_moves_trans / num_moves_iterated);
        int idx = 0;
        if(is_player_white)
            idx = max_rating(final_rating, move_count);
        else
            idx = min_rating(final_rating, move_count);
        print_full_board();
        //make move
        field captured[8] = {0UL};
        //print_move(moves_from[idx] ^ moves_to[idx]);
        castle_flags(piece_idx[idx], moves_from[idx]);
        print_move(moves_from[idx], moves_to[idx]);
        make_move(piece_idx[idx], moves_from[idx], moves_to[idx], captured);
        hashset_add(bitfields[is_player_white] | bitfields[!is_player_white]);
        // save move in struct
        // struct includes current hashtable

        
        //print results
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
