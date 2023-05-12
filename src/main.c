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

int main() {
    
    import_gamestring(bitfields, game_string);

    // set to true if game is finished
    bool gameover = false;
    stopwatch time = start_stopwatch();
    
    int count_total_moves = 0;
    for(int p = 0; !gameover; p++)
    { 
        print_full_board();
        // measure performance starting here
        field moves_from[max_move_count];
        field moves_to[max_move_count];
        int piece_idx[max_move_count] = {0};
        int move_count = generate_moves(moves_from, moves_to, piece_idx);
        // generate all pseudo_moves
            
        float rating[max_move_count];
        field captured[8];
        //iterate over every moveset for a piece
        for(int i = 0; i < move_count; i++){
            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            rating[i] = evaluation(moves_from[i], moves_to[i], piece_idx[i]);
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        }
        // TODO MOVE TO FIND MAX FUNCTION
        //find first maximum rating
        int max_rating_indices[5] = {0, 0, 0};
        int max_rating_index = 0;
        for(int c = 0; c < move_count; c++){
            if(rating[c] == illegal_move)
                move_count--;
            else if(rating[c] > rating[max_rating_index]){
                max_rating_index = c;
                max_rating_indices[4] = max_rating_indices[3];
                max_rating_indices[3] = max_rating_indices[2];
                max_rating_indices[2] = max_rating_indices[1];
                max_rating_indices[1] = max_rating_indices[0];
                max_rating_indices[0] = c;
            }     
        }
        int non_zero = 0;
        for(int i = 0; i < 5; i++){
            if(max_rating_indices[i] != 0)
                non_zero++;
        }
        
        int idx = max_rating_index;
        if(((max_rating_indices[0] + max_rating_indices[1] + max_rating_indices[2] + max_rating_indices[3] + max_rating_indices[4]) / (5 * rating[max_rating_index])) > 0.2)
            idx = max_rating_indices[rand() % non_zero-1];
        // make move
        make_move(piece_idx[idx], moves_from[idx], moves_to[idx], captured);
        hashset_add(bitfields[is_player_white] ^ bitfields[!is_player_white]);
        //unmake_move(piece_index[max_rating_index], moves[max_rating_index], moves[max_rating_index + 1], captured);

        // print results
        field t = stop_stopwatch(time);
        //switch sides
        count_total_moves++;
        gameover = game_finished(move_count);
        if(gameover){
            field t = stop_stopwatch(time);
            printf("Game Over\n");
            print_full_board();
            printf("Match with %d moves took %luμs\n", count_total_moves, t);
            break;
        }
        is_player_white = 1 - is_player_white;
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
