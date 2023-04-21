typedef uint64_t field;

void print_board(field board){
    /*
        bits will be printied from top left (highest value bit)
        to bottom right (lowest value bit)    
    */

    for(int y=7; y>=0; y--){
        for(int x=7; x>=0; x--){
            if((board >> (x+ 8*y))%2 != 0){
                printf("%d ", 1);
            }
            else{
                printf("%d ", 0);
            }
        }
        printf("\n");
    }
    printf("\n");
}

field add_to_board_coords(field board, int x, int y){

    board = board | ((field) 1<<(7-x + 8*y));

    return board;
}