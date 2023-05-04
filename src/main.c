#include "main.h"
#include <math.h>
#include "common.h"
#include "bit_board_utils.h"
#include "move_util.h"
#include "evaluation.h"
#include "stopwatch_utils.h"

int main() {
    
    field bitfields[figure_count];
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

        // all moves
        int bit = 0;
        int piece_count = 0;
        int move_count = 0;
        field checked = (field) 0;

        int current_piece = king;
        
        int piece_array[16];
        int x = 0;

        field legal_moves[16];
        field legal_moves_piece[16];

        for(; current_piece<=pawn; current_piece++)
        {
            field player_piece_board = bitfields[is_player_white] & bitfields[current_piece];
            int num_moves = get_piece_count(player_piece_board);

            field single_piece_boards[num_moves];
            get_single_piece_boards(player_piece_board, single_piece_boards, num_moves);

            for(int y = 0; y < num_moves; y++)
            {
                piece_count++;
                piece_array[x] = current_piece;
                legal_moves_piece[x] = single_piece_boards[y];

                switch(current_piece)
                {
                    case pawn:
                        legal_moves[x] = find_legal_pawn_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                        move_count += get_piece_count(legal_moves[x]);
                        break;

                    case rook:
                        legal_moves[x] = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                        move_count += get_piece_count(legal_moves[x]);
                        break;

                    case bishop:
                        legal_moves[x] = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                        move_count += get_piece_count(legal_moves[x]);
                        break;

                    case knight:
                        bit = log2(single_piece_boards[y]);
                        legal_moves[x] = knight_moves[bit] ^ (knight_moves[bit] & bitfields[is_player_white]);
                        move_count += get_piece_count(legal_moves[x]);
                        break;

                    case queen: ;
                        field legal_moves_queen_1 = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                        field legal_moves_queen_2 = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                        legal_moves[x] = legal_moves_queen_1 | legal_moves_queen_2;
                        move_count += get_piece_count(legal_moves[x]);
                        break;
                        
                    case king:
                        bit = log2(single_piece_boards[y]);
                        legal_moves[x] = king_moves[bit] ^ (king_moves[bit] & bitfields[is_player_white]); 
                        move_count += get_piece_count(legal_moves[x]);
                }
                x++;
            }
        }
        
        //castle check
        field castling = (field) 0;
        if(is_player_white){
            if(castle_left[is_player_white]){
                field rooks = bitfields[is_player_white] & bitfields[rook];
            }
            if(castle_right[is_player_white]){
                field rooks = bitfields[is_player_white] & bitfields[rook];
            }
            //move_count++;
        }
        else{
            if(castle_left[is_player_white]){
                field rooks = bitfields[is_player_white] & bitfields[rook];
                
            }
            if(castle_right[is_player_white]){
                field rooks = bitfields[is_player_white] & bitfields[rook];
            }
            //move_count++;
        }
        field moves[2*move_count];
        int piece_arr[2*move_count];
        int rating[2*move_count];
        int count = 0;
        for(int i = 0; i < piece_count; i++){
            // get number of moves for current_piece
            int piece_count = get_piece_count(legal_moves[i]);
            // split moves
            field single_move[piece_count];
            get_single_piece_boards(legal_moves[i], single_move, piece_count);
            // match specific moves with current_piece and evaluate move
            for(int k = 0; k < piece_count; k++){
                rating[count] = evaluation(bitfields, single_move[k], legal_moves_piece[i], piece_array[i]);
                piece_arr[count] = piece_array[i];
                moves[count] = legal_moves_piece[i];
                count++;
                rating[count] = 0;
                piece_arr[count] = 0;
                moves[count] = single_move[k];
                count++;
            }
        }
        
        //find first maximum rating
        int loc = 0;
        int total_legal_moves = move_count;
        for(int c = 0; c < 2*move_count; c++){
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
        printf("Of all %d moves %d were legal moves and took %lu us\n", move_count, total_legal_moves, t);
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
