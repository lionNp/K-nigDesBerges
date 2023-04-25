#include "main.h"

int main(){
    //read situation string
    field bitfield_fig[figuren_anz];
    char feld_string[] = "rn1qkbnr/pppppppp/8/b7/3PP3/8/PPP2PPP/RNBQKBNR";
    //"1n1rk1n1/pp1ppppp/b2q2b1/8/2p5/3K3r/PPPPPPPP/RNBQ1BNR";
    import_string(bitfield_fig, feld_string);

    print_all_boards(bitfield_fig);
    
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    // all black moves
    field pawns = bitfield_fig[bl] & bitfield_fig[p];
    int num_moves_p = num_pieces(pawns);
    field single_pawns[num_moves_p];
    get_single_pieces(pawns, single_pawns, num_moves_p);
    for(int i=0; i<num_moves_p; i++){
        field legal_moves_pawn = find_legal_pawn_moves(bitfield_fig[bl], bitfield_fig[w], single_pawns[i], black);
        //print_board(legal_diag_moves_bishop);
    }

    field rooks = bitfield_fig[bl] & bitfield_fig[r];
    int num_moves_r = num_pieces(rooks);
    field single_rooks[num_moves_r];
    get_single_pieces(rooks, single_rooks, num_moves_r);
    for(int i=0; i<num_moves_r; i++){
        field legal_moves_rook = find_legal_rook_moves(bitfield_fig[bl], bitfield_fig[w], single_rooks[i]);
        //print_board(legal_diag_moves_bishop);
    }

    field bishops = bitfield_fig[bl] & bitfield_fig[b];
    int num_moves_b = num_pieces(bishops);
    field single_bishops[num_moves_b];
    get_single_pieces(bishops, single_bishops, num_moves_b);
    for(int i=0; i<num_moves_b; i++){
        field legal_moves_bishop = find_legal_diag_moves(bitfield_fig[bl], bitfield_fig[w], single_bishops[i]);
        //print_board(legal_diag_moves_bishop);
    }

    field queen = bitfield_fig[bl] & bitfield_fig[q];
    int num_moves_q = num_pieces(bishops);
    field single_queen[num_moves_q];
    get_single_pieces(single_queen[0], single_queen, 1);
    for(int i=0; i<num_moves_q; i++){
        field legal_moves_queen_1 = find_legal_diag_moves(bitfield_fig[bl], bitfield_fig[w], single_queen[i]);
        field legal_moves_queen_2 = find_legal_rook_moves(bitfield_fig[bl], bitfield_fig[w], single_queen[i]);
        field legal_qeen_moves = legal_moves_queen_1 | legal_moves_queen_2;
        //print_board(legal_diag_moves_bishop);
    }

    gettimeofday(&stop, NULL);
    printf("all moves took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    

    gettimeofday(&start, NULL);
    // check for chess:
    field king = bitfield_fig[w] & bitfield_fig[k];
    field in_chess_from = check_for_chess(bitfield_fig[w], bitfield_fig[bl], king, white, bitfield_fig);
    gettimeofday(&stop, NULL);
    printf("chess check took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 

    printf("\nchess from\n");
    print_board(in_chess_from);

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