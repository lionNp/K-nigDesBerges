#include <string.h>

#include "bit_boards_util.h"

field shift_diag_up(field diag, int n){
    return diag << 8*n;
}

field shift_diag_down(field diag, int n){
    return diag >> 8*n;
}

void get_single_pieces(field input, field single_pieces_color[], int num_p){
    for(int i = 0; i < num_p; i++){
        single_pieces_color[i] = 0;
    }
    int n = 0;
    for(int i = 0; n < num_p; i++){
        if(input % 2 == 1){
            single_pieces_color[n] += 1UL << i;
            n++;
        }
        input = input >> 1;
    }
}

int num_pieces(field board){
    int n = 0;
    for(int i=0; i<64; i++){
        if(board % 2 == 1)
            n++;
        board = board>>1;
    }
    return n;
}

void import_string(field bitfield_fig[], char gamestring[]) {

    //initialise empty boards
    for(int i = 0; i < figure_count; i++){
        bitfield_fig[i] = (field) 0;
    }

    int str_len = strlen(gamestring);

    printf("Gamestring: %s\n", gamestring);

    int i = 0;
    int iterate=0;
    for(; iterate < str_len; iterate++){
        char c = gamestring[iterate];
        if(c == ' ') 
            break;
        switch(c){
            case 'r': add_to_board_br_to_tl(&bitfield_fig[rook], i); add_to_board_br_to_tl(&bitfield_fig[black], i); break;
            case 'n': add_to_board_br_to_tl(&bitfield_fig[knight], i); add_to_board_br_to_tl(&bitfield_fig[black], i); break;
            case 'b': add_to_board_br_to_tl(&bitfield_fig[bishop], i); add_to_board_br_to_tl(&bitfield_fig[black], i); break;
            case 'q': add_to_board_br_to_tl(&bitfield_fig[queen], i); add_to_board_br_to_tl(&bitfield_fig[black], i); break;
            case 'k': add_to_board_br_to_tl(&bitfield_fig[king], i); add_to_board_br_to_tl(&bitfield_fig[black], i); break;
            case 'p': add_to_board_br_to_tl(&bitfield_fig[pawn], i); add_to_board_br_to_tl(&bitfield_fig[black], i); break;

            case 'R': add_to_board_br_to_tl(&bitfield_fig[rook], i); add_to_board_br_to_tl(&bitfield_fig[white], i); break;
            case 'N': add_to_board_br_to_tl(&bitfield_fig[knight], i); add_to_board_br_to_tl(&bitfield_fig[white], i); break;
            case 'B': add_to_board_br_to_tl(&bitfield_fig[bishop], i); add_to_board_br_to_tl(&bitfield_fig[white], i); break;
            case 'Q': add_to_board_br_to_tl(&bitfield_fig[queen], i); add_to_board_br_to_tl(&bitfield_fig[white], i); break;
            case 'K': add_to_board_br_to_tl(&bitfield_fig[king], i); add_to_board_br_to_tl(&bitfield_fig[white], i); break;
            case 'P': add_to_board_br_to_tl(&bitfield_fig[pawn], i); add_to_board_br_to_tl(&bitfield_fig[white], i); break;

            case '/':  i--; break;

            default: i += (c - 48); i--; break; //skip ones
        }

        i++;
    }
    iterate++;
    if(gamestring[iterate] == 'b') turn = 0;
}

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

void add_to_board_coords(field *board, int x, int y){
    *board = *board | ((field) 1<<(7-x + 8*y));
}

void add_to_board_br_to_tl(field *board, int n){
    *board = *board | ((field) 1 << (63-n));
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
    uint64_t rows[] = {row_1, row_2, row_3, row_4, row_5, row_6, row_7, row_8};
    int r= 0;
    int c = 0;
    int stop = 0;

    while(c < 8){
        while(r < 8 && stop == 0){
            if((col[c] & rows[r]) == position)
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
