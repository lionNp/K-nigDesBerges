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


int get_piece_count(field board) {
    int n = 0;
    for(int i = 0; i < 64; i++) {
        if(board % 2 == 1)
            n++;
        board = board>>1;
    }
    return n;
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
void print_board(field board)
{
    for(int y = 7; y >= 0; y--)
    {
        for(int x = 7; x >= 0; x--)
        {
            field bit = 1UL << (x + 8*y);
            if((bit & board))
            {
                bool found = false;
                for(int i = king; i <= pawn; i++)
                {
                    if((bit & bitfields[i]))
                    {
                        bool white = (bit & bitfields[1]);
                        switch(i)
                        {
                            case king:
                                printf("%c ", white ? 'K' : 'k');
                                found = true;
                                break;
                            case queen:
                                printf("%c ", white ? 'Q' : 'q');
                                found = true;
                                break;
                            case rook:
                                printf("%c ", white ? 'R' : 'r');
                                found = true;
                                break;
                            case bishop:
                                printf("%c ", white ? 'B' : 'b');
                                found = true;
                                break;
                            case knight:
                                printf("%c ", white ? 'N' : 'n');
                                found = true;
                                break;
                            case pawn:
                                printf("%c ", white ? 'P' : 'p');
                                found = true;
                                break;
                        }
                    }
                }
                if(!found) 
                    printf("1 ");
            }
            else
                printf("0 ");
        }
        printf("\n");
    }
    printf("\n");
}

void print_move_board(field board){
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

void print_full_board(){
    for(int y = 7; y >= 0; y--){
        for(int x = 7; x >= 0; x--){
            field bit = 1UL << (x + 8*y);
            if((bit & (bitfields[0]))){
                for(int i = 2; i < 8; i++){
                    if((bit & bitfields[i])){
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
            else if((bit & (bitfields[1]))){
                for(int i = 2; i < 8; i++){
                    if((bit & bitfields[i])){
                        switch(i){
                            case 2:
                                printf("%c ", 'K');
                                break;
                            case 3:
                                printf("%c ", 'Q');
                                break;
                            case 4:
                                printf("%c ", 'R');
                                break;
                            case 5:
                                printf("%c ", 'B');
                                break;
                            case 6:
                                printf("%c ", 'N');
                                break;
                            case 7:
                                printf("%c ", 'P');
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

void add_to_board_coords(field *board, int x, int y){
    *board = *board | ((field) 1<<(7-x + 8*y));
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
