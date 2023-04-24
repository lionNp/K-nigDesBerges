#include "boards.h"

int main(){
    //read situation string
    field bitfield_fig[figuren_anz];
    char feld_string[] = "rnbqkbnr/2pppppp/ppB2B2/3B4/8/8/PPPPPPPP/RN1QK1NR";
    import_string(bitfield_fig, feld_string);

    //get single pieces
    //field board_to_split = (bitfield_fig[b] & bitfield_fig[w]) | bitfield_fig[r] | bitfield_fig[n];

    //int num_p = num_pieces(board_to_split);
    //field single_pieces_color[num_p];
    //get_single_pieces(board_to_split, single_pieces_color, num_p);
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
    field legal_diag_moves_bishop = find_legal_diag_moves(bitfield_fig[w], bitfield_fig[bl], (field) 1 << 28);

    printf("Board white:\n");
    print_board(bitfield_fig[w]);
    printf("Board black:\n");
    print_board(bitfield_fig[bl]);
    printf("Moves for bishop at postion 28 (d4)\n");
    print_board(legal_diag_moves_bishop);

    /*int num_moves = num_pieces(legal_diag_moves_bishop);
    field single_moves[num_moves];
    get_single_pieces(legal_diag_moves_bishop, single_moves, num_moves);
    for(int i=0; i<num_moves; i++){
        print_board(single_moves[i]);
    }
    printf("for a total of %d moves\n", num_moves);*/
    

    //printf("Board white:\n");
    //print_board(bitfield_fig[w]);
    //printf("White Bishops:\n");
    //print_board(bitfield_fig[bl] & bitfield_fig[p]);
    //printf("Board black:\n");
    //print_board(bitfield_fig[bl]);*/
    
    struct timeval stop, start;
    gettimeofday(&start, NULL);


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
    printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    

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