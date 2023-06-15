#include <string.h>
#include "move_util.h"
#include "bit_boards_util.h"
#include "hashset.h"


void get_single_piece_boards(field board, field single_piece_boards[], int piece_count) 
{
    //set result boards to 0
    for(int i = 0; i < piece_count; i++) {
        single_piece_boards[i] = 0;
    }

    int pieces_found = 0;
    for(int i = 0; pieces_found < piece_count; i++) 
    {
        if(board % 2 == 1) 
        {
            single_piece_boards[pieces_found] = (1ul << i);
            pieces_found++;
        }
        board = board>>1;
    }
}

//magic
int get_piece_count(field board) {
    board = board - ((board >> 1) & 0x5555555555555555);
    board = (board & 0x3333333333333333) + ((board >> 2) & 0x3333333333333333);
    board = (board + (board >> 4)) & 0x0F0F0F0F0F0F0F0F;
    return (board * 0x0101010101010101) >> 56;
    /*
    int n = 0;
    board = board - ((board >> 1) & 0x5555555555555555);
    board = (board & 0x3333333333333333) + ((board >> 2) & 0x3333333333333333);
    board = (board + (board >> 4)) & 0x0F0F0F0F0F0F0F0F;
    return (board * 0x0101010101010101) >> 56;
}

//a bit better
int get_piece_count3(field board) {
    int n = 0;
    for(int i = 0; i < 64; i++) {
        n += (board<<i) >> 63; 
    }
    return n;
}

//old
int get_piece_count2(field board) {
    int n = 0;
    for(int i = 0; i < 64; i++) {
        if(board % 2 == 1)
            n++;
        board = board>>1;
    }
    return n;
    */
}

bool game_finished(int total_legal_moves){
    if((bitfields[is_player_white] & bitfields[king]) & koth){
        printf("%s won!\n", is_player_white ? "white" : "black");
        return true;
    }

    if(hashset_duplicates() == 3){
        printf("Match ended in a draw by repetition!\n"); // -> REMI
        return true;
    }

    if(total_legal_moves == 0){
        if(in_check(bitfields[is_player_white] & bitfields[king])){
            printf("%s won!\n", !is_player_white ? "white" : "black");
            return true;
        }   
        printf("Match ended in a forced draw!\n"); // -> REMI
        return true;
    }
    return false;
}

void import_gamestring(field bitfields[], char gamestring[]) 
{
    //initialise empty boards
    for(int i = 0; i < figure_count; i++) {
        bitfields[i] = (field) 0;
    }

    int str_len = strlen(gamestring);

    printf("Gamestring: %s\n", gamestring);

    int i = 0;
    int iterate = 0;
    for(; iterate < str_len; iterate++) 
    {
        char c = gamestring[iterate];
        if(c == ' ')
            break;
        switch(c) 
        {
            case 'r': 
                set_bit_by_index(&bitfields[rook], i); 
                set_bit_by_index(&bitfields[black], i); 
                break;

            case 'n': 
                set_bit_by_index(&bitfields[knight], i);
                set_bit_by_index(&bitfields[black], i); 
                break;

            case 'b': 
                set_bit_by_index(&bitfields[bishop], i);
                set_bit_by_index(&bitfields[black], i); 
                break;

            case 'q': 
                set_bit_by_index(&bitfields[queen], i); 
                set_bit_by_index(&bitfields[black], i); 
                break;

            case 'k': 
                set_bit_by_index(&bitfields[king], i); 
                set_bit_by_index(&bitfields[black], i); 
                break;

            case 'p': 
                set_bit_by_index(&bitfields[pawn], i); 
                set_bit_by_index(&bitfields[black], i); 
                break;

            case 'R': 
                set_bit_by_index(&bitfields[rook], i); 
                set_bit_by_index(&bitfields[white], i); 
                break;

            case 'N': 
                set_bit_by_index(&bitfields[knight], i); 
                set_bit_by_index(&bitfields[white], i); 
                break;

            case 'B': 
                set_bit_by_index(&bitfields[bishop], i); 
                set_bit_by_index(&bitfields[white], i); 
                break;

            case 'Q': 
                set_bit_by_index(&bitfields[queen], i); 
                set_bit_by_index(&bitfields[white], i); 
                break;
                
            case 'K': 
                set_bit_by_index(&bitfields[king], i); 
                set_bit_by_index(&bitfields[white], i); 
                break;

            case 'P': 
                set_bit_by_index(&bitfields[pawn], i); 
                set_bit_by_index(&bitfields[white], i); 
                break;

            case '/':  
                i--;           //dont count '/' as field index
                break;

            default:           //skip n fields if 'c' is a number
                i += (c - 48); //c - 48 == actual number value
                i--;
                break;
        }
        i++;
    }
    iterate++;
    if(gamestring[iterate] == 'b')
        is_player_white = 0;
    else if(gamestring[iterate] == 'w')
        is_player_white = 1;
    iterate++;
    iterate++;
    castle_right[1] = false;
    castle_left[1] = false;
    castle_right[0] = false;
    castle_left[0] = false;
    for(int i = iterate; i < str_len;i++){
        char c = gamestring[i];
        switch(c){
            default:
                break;
            case 'K':
                castle_right[1] = true;
                break;
            case 'Q':
                castle_left[1] = true;
                break;
            case 'k':
                castle_right[0] = true;
                break;
            case 'q':
                castle_left[0] = true;
                break;
        }
        if(c == ' ')
            break;
    }
}
/*
    bits will be printied from top left (highest value bit)
    to bottom right (lowest value bit)    
*/
void print_board(field board){
    for(int y = 7; y >= 0; y--){
        for(int x = 7; x >= 0; x--){
            field bit = 1UL << (x + 8*y);
            if((bit & board))
            {
                for(int i = 2; i < 8; i++)
                {
                    if((bit & bitfields[i]))
                    {
                        switch(i){
                            case 2:
                                printf("%c ", 'k');
                                break;
                            case 3:
                                printf("%c ", 'q');
                                break;
                            case 4:
                                printf("%c ", 'r');
                                break;
                            case 5:
                                printf("%c ", 'b');
                                break;
                            case 6:
                                printf("%c ", 'n');
                                break;
                            case 7:
                                printf("%c ", 'p');
                                break;
                        }
                    }
                }
            }
            else
                printf("%d ", 0);
        }
        printf("\n");
    }
    printf("\n");
}

void print_move_2(field board){
    for(int y = 7; y >= 0; y--){
        for(int x = 7; x >= 0; x--){
            field bit = 1UL << (x + 8*y);
            if((bit & board))
                printf("%d ", 1);
            else
                printf("%d ", 0);
        }
        printf("\n");
    }
    printf("\n");
}

void white_piece_print(int piece_type)
{
    switch (piece_type)
    {
        case king:
            printf("K ");
            break;
        case queen:
            printf("Q ");
            break;
        case rook:
            printf("R ");
            break;
        case bishop:
            printf("B ");
            break;
        case knight:
            printf("N ");
            break;
        case pawn:
            printf("P ");
            break;
    }
}

void black_piece_print(int piece_type)
{
    switch (piece_type)
    {
        case king:
            printf("k ");
            break;
        case queen:
            printf("q ");
            break;
        case rook:
            printf("r ");
            break;
        case bishop:
            printf("b ");
            break;
        case knight:
            printf("n ");
            break;
        case pawn:
            printf("p ");
            break;
    }
}

void print_full_board(){
    for(int y = 7; y >= 0; y--){
        for(int x = 7; x >= 0; x--){
            field bit = 1UL << (x + 8*y);
            if((bit & (bitfields[0]))){
                for(int i = 2; i < 8; i++){
                    if((bit & bitfields[i])){
                        black_piece_print(i);
                    }
                }
            }
            else if((bit & (bitfields[1]))){
                for(int i = 2; i < 8; i++){
                    if((bit & bitfields[i])){
                        white_piece_print(i);
                    }
                }
            }
            else
                printf("%d ", 0);
        }
        printf("\n");
    }
    printf("\n");
}

void add_to_board_coords(field *board, int x, int y){
    *board |= ((field) 1 << (7-x + 8*y));
}

//n=0 is top left, n=63 is bottom right
void set_bit_by_index(field *board, int n) {
    *board |= ((field) 1 << (63-n));
}

void print_all_boards(field *boards){
    printf("White Board:\n");
    print_board(boards[white]);
    printf("Black Board:\n");
    print_board(boards[black]);
    printf("Rook Board:\n");
    print_board(boards[rook]);
    printf("Knight Board:\n");
    print_board(boards[knight]);
    printf("Bishop Board:\n");
    print_board(boards[bishop]);
    printf("Queen Board:\n");
    print_board(boards[queen]);
    printf("King Board:\n");
    print_board(boards[king]);
    printf("Pawn Board:\n");
    print_board(boards[pawn]);
}

void print_pos(field position){
    char start[] = "xx";
    uint64_t col[] = {a_file, b_file, c_file, d_file, e_file, f_file, g_file, h_file};
    uint64_t ranks[] = {rank_1, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8};
    int r= 0;
    int c = 0;
    int stop = 0;

    while(c < 8){
        while(r < 8 && stop == 0){
            if((col[c] & ranks[r]) == position)
                stop = 1;
            r++;
        }
        if(stop > 0)
            break;
        r = 0;
        c++;
    }

    switch(c){
        case 0:
            start[0] = 'a';
            break;
        case 1:
            start[0] = 'b';
            break;
        case 2:
            start[0] = 'c';
            break;
        case 3:
            start[0] = 'd';
            break;
        case 4:
            start[0] = 'e';
            break;
        case 5:
            start[0] = 'f';
            break;
        case 6:
            start[0] = 'g';
            break;
        case 7:
            start[0] = 'h';
            break;
    }
    start[1] = r + '0';
    printf("%s", start);
}

void print_move(field start, field end){
    print_pos(start);
    printf("-");
    print_pos(end);
    printf("\n");
}