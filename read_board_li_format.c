#include "boards.h"

int main(){
    char feld_string[] = "dd3-Kf5";

    field bitfield_fig[figuren_anz];
    for(int i=0; i<figuren_anz; i++){
        bitfield_fig[i] = (field) 0;

        printf("%ld \n",bitfield_fig[i]);
    }


    // test_bitfield_print();
    // test_add_to_board_coords();

    printf("done\n");

    return 0;
}
void test_add_to_board_coords(){
    field board = (field) 0;

    printf("board:\n");
    print_board(board);
    printf("add to board at x=3, y=2\n");
    board = add_to_board_coords(board, 3, 4);
    print_board(board);
}

void test_bitfield_print(){
    field bitfield = (field) 0b1000011011000111100111011110100111001001001111010100101111111010;
    print_board(bitfield);
}