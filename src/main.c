#include "main.h"
#include <math.h>
#include "common.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include "evaluation.h"
#include "stopwatch_utils.h"
#include "move_generator.h"
#include "move_executer.h"

int main() {
    
    import_gamesting(bitfields, game_string);

    // print game start
    /*
    printf("%s board:\n", is_player_white ? "Whites" : "Blacks");
    print_board(bitfields[is_player_white]);
    printf("%s board:\n", is_player_white ? "Blacks" : "Whites");
    print_board(bitfields[!is_player_white]);
    printf("\n");
    */
    bool gameover = false;

    for(int p = 0; p < 20; p++) // TODO: to be moved into own file
    { 
        // measure performance starting here
        stopwatch time = start_stopwatch();

        //counts[0]: total move count. counts[1]: total piece count (player)
        int counts[2] = {0, 0};  

        //contains all player pieces
        int piece_array[16];
        
        //legal_moves[i] are all moves for piece "legal_moves_piece[i]".
        field legal_moves[16];
        field legal_moves_piece[16];
        
        // generate all pseudo_moves
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);

        field moves_from[counts[0]];
        field moves_to[counts[0]];
        int piece_index[counts[0]];
        float rating[counts[0]];
        int count = 0;
        field captured[8];
        //iterate over every moveset for a piece
        for(int i = 0; i < counts[1]; i++)
        {
            // get number of moves for current piece i
            int piece_count = get_piece_count(legal_moves[i]);

            // split moves for piece into single bitboards
            field single_moves[piece_count];
            get_single_piece_boards(legal_moves[i], single_moves, piece_count);

            // foreach move the current piece could do, evaluate that move
            for(int k = 0; k < piece_count; k++)
            {
                make_move(piece_array[i], legal_moves_piece[i], single_moves[k], captured);
                rating[count] = evaluation(single_moves[k], legal_moves_piece[i], piece_array[i]);
                unmake_move(piece_array[i], legal_moves_piece[i], single_moves[k], captured);
                piece_index[count] = piece_array[i];
                moves_from[count] = legal_moves_piece[i];
                moves_to[count] = single_moves[k];
                count++;
            }
        }
        // TODO MOVE TO FIND MAX FUNCTION
        //find first maximum rating
        int max_rating_index = 0;
        int total_legal_moves = counts[0];
        for(int c = 0; c < counts[0]; c++){
            if(rating[c] == illegal_move)
                total_legal_moves--;
            else if(rating[c] > rating[max_rating_index])
                max_rating_index = c;
        }
        
        game_finished(gameover, total_legal_moves);
        if(gameover)
            return 0;
  
        // make move
        make_move(piece_index[max_rating_index], moves_from[max_rating_index], moves_to[max_rating_index], captured);

        //unmake_move(piece_index[max_rating_index], moves[max_rating_index], moves[max_rating_index + 1], captured);

        // print results
        field t = stop_stopwatch(time);
        printf("Of all %d moves %d were legal moves and took %lu us\n", counts[0], total_legal_moves, t);
        /*
        printf("Best move for %s:\n", is_player_white ? "white" : "black");
        print_board(moves_from[max_rating_index]);
        printf("to:\n");
        print_board(moves_to[max_rating_index]);
        */
        printf("board:\n");
        print_full_board();
        //switch sides
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
