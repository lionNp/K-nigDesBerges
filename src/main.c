#include "main.h"
#include <math.h>
#include "common.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include "evaluation.h"
#include "stopwatch_utils.h"

int main(){
    /*
    field king_m[64];
    init_king_moves(king_m);
    for(int i = 0; i < 63; i++){
        printf("0x%lxull, ", king_m[i]);
    }
    printf("\n");
    */
    //read situation string
    field bitfield_fig[figure_count];
    char feld_string[] = game_string;
    
    import_string(bitfield_fig, feld_string);

    if(turn){
            printf("Whites board:\n");
            print_board(bitfield_fig[turn]);
            printf("Blacks board:\n");
            print_board(bitfield_fig[!turn]);
            printf("\n");
        }
    else{
        printf("Blacks board:\n");
        print_board(bitfield_fig[turn]);
        printf("Whites board:\n");
        print_board(bitfield_fig[!turn]);
        printf("\n");
    }

    struct timeval stop, start;

    for(int p = 0; p < 4; p++){
        // measure performance starting here
        stopwatch* time = start_stopwatch();

        // all moves
        field legal_moves[16];
        field legal_moves_piece[16];
        int piece_array[16];
        int x = 0;
        int bit = 0;
        int piece_count = 0;
        int move_count = 0;
        int piece = king;
        field checked = (field) 0;
        for(; piece<=pawn; piece++){
            field pieces = bitfield_fig[turn] & bitfield_fig[piece];
            int num_moves = num_pieces(pieces);
            field single_piece[num_moves];
            get_single_pieces(pieces, single_piece, num_moves);
            for(int y = 0; y < num_moves; y++){
                piece_count++;
                piece_array[x] = piece;
                legal_moves_piece[x] = single_piece[y];
                switch(piece){
                    case pawn:
                        legal_moves[x] = find_legal_pawn_moves(bitfield_fig[turn], bitfield_fig[!turn], single_piece[y]);
                        move_count += num_pieces(legal_moves[x]);
                        break;
                    case rook:
                        legal_moves[x] = find_legal_rook_moves(bitfield_fig[turn], bitfield_fig[!turn], single_piece[y]);
                        move_count += num_pieces(legal_moves[x]);
                        break;
                    case bishop:
                        legal_moves[x] = find_legal_diag_moves(bitfield_fig[turn], bitfield_fig[!turn], single_piece[y]);
                        move_count += num_pieces(legal_moves[x]);
                        break;
                    case knight:
                        bit = log2(single_piece[y]);
                        legal_moves[x] = knight_moves[bit] ^ (knight_moves[bit] & bitfield_fig[turn]);
                        move_count += num_pieces(legal_moves[x]);
                        break;
                    case queen:
                        field legal_moves_queen_1 = find_legal_diag_moves(bitfield_fig[turn], bitfield_fig[!turn], single_piece[y]);
                        field legal_moves_queen_2 = find_legal_rook_moves(bitfield_fig[turn], bitfield_fig[!turn], single_piece[y]);
                        legal_moves[x] = legal_moves_queen_1 | legal_moves_queen_2;
                        move_count += num_pieces(legal_moves[x]);
                        break;
                    case king:
                        bit = log2(single_piece[y]);
                        legal_moves[x] = king_moves[bit] ^ (king_moves[bit] & bitfield_fig[turn]); 
                        move_count += num_pieces(legal_moves[x]);
                }
            x++;
            }
        }
        //castle check
        field castling = (field) 0;
        if(turn){
            if(castle_left[turn]){
                field rooks = bitfield_fig[turn] & bitfield_fig[rook];
            }
            if(castle_right[turn]){
                field rooks = bitfield_fig[turn] & bitfield_fig[rook];
            }
            //move_count++;
        }
        else{
            if(castle_left[turn]){
                field rooks = bitfield_fig[turn] & bitfield_fig[rook];
                
            }
            if(castle_right[turn]){
                field rooks = bitfield_fig[turn] & bitfield_fig[rook];
            }
            //move_count++;
        }
        field moves[2*move_count];
        int piece_arr[2*move_count];
        int rating[2*move_count];
        int count = 0;
        for(int i = 0; i < piece_count; i++){
            // get number of moves for piece
            int num_moves = num_pieces(legal_moves[i]);
            // split moves
            field single_move[num_moves];
            get_single_pieces(legal_moves[i], single_move, num_moves);
            // match specific moves with piece and evaluate move
            for(int k = 0; k < num_moves; k++){
                rating[count] = evaluation(bitfield_fig, single_move[k], legal_moves_piece[i], piece_array[i]);
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
        int c = 0;
        for(c = 1; c < 2*move_count; c++)
            if(rating[c] > rating[loc])
                    loc = c;
        // make move
        // move piece in piece_board
        bitfield_fig[turn] ^= moves[loc];
        bitfield_fig[piece_arr[loc]] ^= moves[loc];
        for(int i = 0; i < 8; i++)
            bitfield_fig[i] ^= (moves[loc+1] & bitfield_fig[i]); 
        bitfield_fig[turn] ^= moves[loc+1];
        bitfield_fig[piece_arr[loc]] ^= moves[loc+1];

        if(piece_arr[loc] == king){
            castle_left[turn] = false;
            castle_right[turn] = false;
        }else if(piece_arr[loc] == rook){
            if(moves[loc] & h_file)
                castle_right[turn] = false;
            if(moves[loc] & a_file)
                castle_left[turn] = false;
        }

        field t = stop_stopwatch(time);
        printf("all moves took %lu us\n", t);

        if(turn){
            printf("Best move for white:\n");
            print_board(moves[loc]);
            printf("to:\n");
            print_board(moves[loc+1]);
            printf("Whites board:\n");
            print_board(bitfield_fig[turn]);
            printf("Blacks board:\n");
            print_board(bitfield_fig[!turn]);
            printf("\n");
        }
        else{
            printf("Best move for black:\n");
            print_board(moves[loc]);
            printf("to:\n");
            print_board(moves[loc+1]);
            printf("Blacks board:\n");
            print_board(bitfield_fig[turn]);
            printf("Whites board:\n");
            print_board(bitfield_fig[!turn]);
            printf("\n");
        }
        //switch sides here
        turn = 1 - turn;
    }
    
    return 0;
}

void test_add_to_board_coords(){
    field board = (field) 0;

    printf("board:\n");
    print_board(board);
    printf("add to board at x=3, y=2\n");
    add_to_board_coords(&board, 3, 4);
    print_board(board);
}

void test_bitfield_print(){
    field bitfield = (field) 0b1000101001100001111100011010001001011111001100011001000111111000;
    print_board(bitfield);
}
