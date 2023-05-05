#include "main.h"
#include <math.h>
#include "common.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include "evaluation.h"
#include "stopwatch_utils.h"
#include "move_generator.h"

int main() {
    
    import_gamesting(bitfields, game_string);

    // print game start
    printf("%s board:\n", is_player_white ? "Whites" : "Blacks");
    print_board(bitfields[is_player_white]);
    printf("%s board:\n", is_player_white ? "Blacks" : "Whites");
    print_board(bitfields[!is_player_white]);
    printf("\n");

    for(int p = 0; p < 1; p++) // TODO: to be moved into own file
    { 
        // measure performance starting here
        stopwatch* time = start_stopwatch();

        //counts[0]: total move count. counts[1]: total piece count (player)
        int counts[2] = {0, 0};  

        //contains all player pieces
        int piece_array[16];
        
        //legal_moves[i] are all moves for piece "legal_moves_piece[i]".
        field legal_moves[16];
        field legal_moves_piece[16];
        
        // generate all pseudo_moves
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);

        int arr_size = 2 * counts[0];
        field moves[arr_size];
        int piece_index[arr_size];
        float rating[arr_size];
        int count = 0;

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
                rating[count] = evaluation(single_moves[k], legal_moves_piece[i], piece_array[i]);
                piece_index[count] = piece_array[i];
                moves[count] = legal_moves_piece[i];
                count++;
                
                //whats happening here ?
                rating[count] = 0;
                piece_index[count] = 0;
                moves[count] = single_moves[k];
                count++;
            }
        }
        // TODO MOVE TO FIND MAX FUNCTION
        //find first maximum rating
        int max_rating_index = 0;
        int total_legal_moves = counts[0];
        for(int c = 0; c < arr_size; c++){
            if(rating[c] == illegal_move)
                total_legal_moves--;
            else if(rating[c] > rating[max_rating_index])
                max_rating_index = c;
        }

        if((bitfields[is_player_white] & bitfields[king]) & koth){
            printf("Player won!\n");
            return 0;
        }

        if(total_legal_moves == 0){
            if(in_check(bitfields[is_player_white] & bitfields[king])){
                printf("Opponent won!\n");
                return 0;
            }   
            printf("Match ended in a forced draw!\n"); // -> REMI
            return 0;
        } 

        // TODO CREATE MOVE FUNCTION    
        // make move
        // move rook if castle
        if(piece_index[max_rating_index] == king){
            if(log2(moves[max_rating_index]) - log2(moves[max_rating_index+1]) == 2){
                bitfields[rook] ^= ((moves[max_rating_index+1] >> 1) |  (moves[max_rating_index+1] << 1));
                bitfields[is_player_white] ^= ((moves[max_rating_index+1] >> 1) |  (moves[max_rating_index+1] << 1));
            }
            if(log2(moves[max_rating_index]) - log2(moves[max_rating_index+1]) == -2){
                bitfields[rook] ^= ((moves[max_rating_index+1] << 2) |  (moves[max_rating_index+1] >> 1));
                bitfields[is_player_white] ^= ((moves[max_rating_index+1] << 2) |  (moves[max_rating_index+1] >> 1));
            }
        }
        // move current_piece in piece_board
        // remove piece from old square
        bitfields[is_player_white] ^= moves[max_rating_index];
        bitfields[piece_index[max_rating_index]] ^= moves[max_rating_index];

        // eliminate all pieces from moved to square
        for(int i = 0; i < 8; i++)
            bitfields[i] ^= (moves[max_rating_index+1] & bitfields[i]); 

        // add piece at moved to square
        bitfields[is_player_white] ^= moves[max_rating_index+1];
        bitfields[piece_index[max_rating_index]] ^= moves[max_rating_index+1];

        if(piece_index[max_rating_index] == king)
        {
            castle_left[is_player_white] = false;
            castle_right[is_player_white] = false;
        }
        else if(piece_index[max_rating_index] == rook)
        {
            if(moves[max_rating_index] & h_file)
                castle_right[is_player_white] = false;

            else if(moves[max_rating_index] & a_file)
                castle_left[is_player_white] = false;
        }

        // print results
        field t = stop_stopwatch(time);
        printf("Of all %d moves %d were legal moves and took %lu us\n", counts[0], total_legal_moves, t);
        
        printf("Best move for %s:\n", is_player_white ? "white" : "black");
        print_board(moves[max_rating_index]);
        printf("to:\n");
        print_board(moves[max_rating_index+1]);
        printf("%s board:\n", is_player_white ? "Whites" : "Blacks");
        print_board(bitfields[is_player_white]);
        printf("%s board:\n", is_player_white ? "Blacks" : "Whites");
        print_board(bitfields[!is_player_white]);
        printf("\n");

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
