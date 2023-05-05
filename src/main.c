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

    //TODO: ???
    if(is_player_white) {
        printf("Whites board:\n");
        print_board(bitfields[is_player_white]);
        printf("Blacks board:\n");
        print_board(bitfields[!is_player_white]);
        printf("\n");
    } else {
        printf("Blacks board:\n");
        print_board(bitfields[is_player_white]);
        printf("Whites board:\n");
        print_board(bitfields[!is_player_white]);
        printf("\n");
    }

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

        //castle check
        /*
        field castling = (field) 0;
        if(is_player_white){
            if(castle_left[is_player_white]){
                field rooks = bitfields[is_player_white] & bitfields[rook];
            }
            if(castle_right[is_player_white]){
                field rooks = bitfields[is_player_white] & bitfields[rook];
            }
            count[0]++;
        }
        else{
            if(castle_left[is_player_white]){
                field rooks = bitfields[is_player_white] & bitfields[rook];
                
            }
            if(castle_right[is_player_white]){
                field rooks = bitfields[is_player_white] & bitfields[rook];
            }
            count[0]++;
        }
        */
        field moves[2*counts[0]];
        int piece_arr[2*counts[0]];
        int rating[2*counts[0]];
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
                rating[count] = evaluation(bitfields, single_moves[k], legal_moves_piece[i], piece_array[i]);
                piece_arr[count] = piece_array[i];
                moves[count] = legal_moves_piece[i];
                count++;
                
                //whats happening here ?
                rating[count] = 0;
                piece_arr[count] = 0;
                moves[count] = single_moves[k];
                count++;
            }
        }
        
        //find first maximum rating
        int loc = 0;
        int total_legal_moves = counts[0];
        for(int c = 0; c < 2*counts[0]; c++){
            if(rating[c] == -9999)
                total_legal_moves--;
            if(rating[c] > rating[loc])
                    loc = c;
        }
            
        // make move
        // move current_piece in piece_board
        bitfields[is_player_white] ^= moves[loc];
        bitfields[piece_arr[loc]] ^= moves[loc];
        for(int i = 0; i < 8; i++)
            bitfields[i] ^= (moves[loc+1] & bitfields[i]); 
        bitfields[is_player_white] ^= moves[loc+1];
        bitfields[piece_arr[loc]] ^= moves[loc+1];

        if(piece_arr[loc] == king){
            castle_left[is_player_white] = false;
            castle_right[is_player_white] = false;
        }else if(piece_arr[loc] == rook){
            if(moves[loc] & h_file)
                castle_right[is_player_white] = false;
            if(moves[loc] & a_file)
                castle_left[is_player_white] = false;
        }

        field t = stop_stopwatch(time);
        printf("Of all %d moves %d were legal moves and took %lu us\n", counts[0], total_legal_moves, t);
        if(is_player_white){
            printf("Best move for white:\n");
            print_board(moves[loc]);
            printf("to:\n");
            print_board(moves[loc+1]);
            printf("Whites board:\n");
            print_board(bitfields[is_player_white]);
            printf("Blacks board:\n");
            print_board(bitfields[!is_player_white]);
            printf("\n");
        }
        else{
            printf("Best move for black:\n");
            print_board(moves[loc]);
            printf("to:\n");
            print_board(moves[loc+1]);
            printf("Blacks board:\n");
            print_board(bitfields[is_player_white]);
            printf("Whites board:\n");
            print_board(bitfields[!is_player_white]);
            printf("\n");
        }
        //switch sides here
        is_player_white = 1 - is_player_white;
    }
    
    return 0;
}

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
