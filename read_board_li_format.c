#include "boards.h"

int main(){
    //read situation string
    field bitfield_fig[figuren_anz];
    char feld_string[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    import_string(bitfield_fig, feld_string);

    //get single pieces
    field board_to_split = (bitfield_fig[b] & bitfield_fig[w]) | bitfield_fig[r] | bitfield_fig[n];

    int num_p = num_pieces(board_to_split);
    field single_pieces_color[num_p];
    get_single_pieces(board_to_split, single_pieces_color, num_p);
    /*for(int i=0; i<num_p; i++){
        print_board(single_pieces_color[i]);
    }*/
    
    //run tests if needed
    // test_bitfield_print();
    // test_add_to_board_coords();
    // print_all_boards(bitfield_fig);

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