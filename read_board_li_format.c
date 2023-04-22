#include "boards.h"

int main(){
    //read situation string
    field bitfield_fig[figuren_anz];
    char feld_string[] = "rnbqkbnr/pppppppp/8/8/2B2B2/8/PPPPPPPP/RN1QK1NR";
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
    // print_all_boards(bitfield_fig);

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
    

    printf("Board white:\n");
    print_board(bitfield_fig[w]);
    printf("White Bishops:\n");
    print_board(bitfield_fig[w] & bitfield_fig[b]);
    printf("Board black:\n");
    print_board(bitfield_fig[bl]);


    printf("moves white bishops\n");
    field white_bishops = bitfield_fig[w] & bitfield_fig[b];
    int num_moves = num_pieces(white_bishops);
    field single_bishops[num_moves];
    get_single_pieces(white_bishops, single_bishops, num_moves);
    for(int i=0; i<num_moves; i++){
        field legal_diag_moves_bishop = find_legal_diag_moves(bitfield_fig[w], bitfield_fig[bl], single_bishops[i]);
        print_board(legal_diag_moves_bishop);
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