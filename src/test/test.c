#include <pthread.h>
#include <sys/time.h>
#include "bit_boards_util.h"
#include "move_util.h"
#include "common.h"

int main(){
    //read situation string
    field bitfield_fig[figure_count];
    char feld_string[] = "rn1qkbnr/pppppppp/8/b7/3PP3/8/PPP2PPP/RNBQKBNR";
    //"1n1rk1n1/pp1ppppp/b2q2b1/8/2p5/3K3r/PPPPPPPP/RNBQ1BNR";
    import_gamesting(bitfield_fig, feld_string);

    //get single pieces
    //field board_to_split = (bitfield_fig[bishop] & bitfield_fig[white]) | bitfield_fig[rook] | bitfield_fig[knight];

    //int num_p = get_piece_count(board_to_split);
    //field single_pieces_color[num_p];
    //get_single_piece_boards(board_to_split, single_pieces_color, num_p);
    /*for(int i=0; i<num_p; i++){
        print_board(single_pieces_color[i]);
    }*/
    
    //run tests if needed
    // test_bitfield_print();
    // test_add_to_board_coords();
    print_all_boards(bitfield_fig);

    //print_board(down_diag);
    //print_board(up_diag);
    //print_board(horizontal);
    //print_board(vertical);

    /*for(int i=0; i<8; i++){
        print_board(shift_diag_up(up_diag, i));
    }*/


    /*//imagine we are white
    field legal_diag_moves_bishop = find_legal_diag_moves(bitfield_fig[white], bitfield_fig[black], (field) 1 << 28);

    printf("Board white:\n");
    print_board(bitfield_fig[white]);
    printf("Board black:\n");
    print_board(bitfield_fig[black]);
    printf("Moves for bishop at postion 28 (d4)\n");
    print_board(legal_diag_moves_bishop);

    /*int num_moves = get_piece_count(legal_diag_moves_bishop);
    field single_moves[num_moves];
    get_single_piece_boards(legal_diag_moves_bishop, single_moves, num_moves);
    for(int i=0; i<num_moves; i++){
        print_board(single_moves[i]);
    }
    printf("for a total of %d moves\n", num_moves);*/
    

    //printf("Board white:\n");
    //print_board(bitfield_fig[white]);
    //printf("White Bishops:\n");
    //print_board(bitfield_fig[black] & bitfield_fig[pawn]);
    //printf("Board black:\n");
    //print_board(bitfield_fig[black]);*/
    
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    // all black moves

    field pawns = bitfield_fig[black] & bitfield_fig[pawn];
    int num_moves_p = get_piece_count(pawns);
    field single_pawns[num_moves_p];
    get_single_piece_boards(pawns, single_pawns, num_moves_p);
    for(int i=0; i<num_moves_p; i++){
        field legal_moves_pawn = find_legal_pawn_moves(bitfield_fig[black], bitfield_fig[white], single_pawns[i]);
        //print_board(legal_diag_moves_bishop);
    }

    field rooks = bitfield_fig[black] & bitfield_fig[rook];
    int num_moves_r = get_piece_count(rooks);
    field single_rooks[num_moves_r];
    get_single_piece_boards(rooks, single_rooks, num_moves_r);
    for(int i=0; i<num_moves_r; i++){
        field legal_moves_rook = find_legal_rook_moves(bitfield_fig[black], bitfield_fig[white], single_rooks[i]);
        //print_board(legal_diag_moves_bishop);
    }

    field bishops = bitfield_fig[black] & bitfield_fig[bishop];
    int num_moves_b = get_piece_count(bishops);
    field single_bishops[num_moves_b];
    get_single_piece_boards(bishops, single_bishops, num_moves_b);
    for(int i=0; i<num_moves_b; i++){
        field legal_moves_bishop = find_legal_diag_moves(bitfield_fig[black], bitfield_fig[white], single_bishops[i]);
        //print_board(legal_diag_moves_bishop);
    }

    field queen = bitfield_fig[black] & bitfield_fig[queen];
    int num_moves_q = get_piece_count(bishops);
    field single_queen[num_moves_q];
    get_single_piece_boards(single_queen[0], single_queen, 1);
    for(int i=0; i<num_moves_q; i++){
        field legal_moves_queen_1 = find_legal_diag_moves(bitfield_fig[black], bitfield_fig[white], single_queen[i]);
        field legal_moves_queen_2 = find_legal_rook_moves(bitfield_fig[black], bitfield_fig[white], single_queen[i]);
        field legal_qeen_moves = legal_moves_queen_1 | legal_moves_queen_2;
        //print_board(legal_diag_moves_bishop);
    }

    
    gettimeofday(&stop, NULL);
    printf("all moves took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    

    gettimeofday(&start, NULL);
    // check for chess:
    field king = bitfield_fig[white] & bitfield_fig[king];
    //field in_check_from = in_check(bitfield_fig[white], bitfield_fig[black], king, white, bitfield_fig);
    gettimeofday(&stop, NULL);
    printf("chess check took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 

    printf("\nchess from\n");
    //print_board(in_check_from);


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