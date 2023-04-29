#include <string.h>

#include "bit_boards_util.h"

field shift_diag_up(field diag, int n){
    return diag << 8*n;
}

field shift_diag_down(field diag, int n){
    return diag >> 8*n;
}

void get_single_pieces(field bitfield_fig, field single_pieces_color[], int num_p){
    for(int i=0; i<num_p; i++){
        single_pieces_color[i] = 0;}

    //printf("num pieces: %d\n", num_p);
    
    int n = 0;
    for(int i=0; n<num_p; i++){
        if(bitfield_fig % 2 == 1){
            single_pieces_color[n] += (field) 1 << i;
            n++;
        }
        bitfield_fig = bitfield_fig >> 1;
    }
}

int num_pieces(field board){
    int n = 0;
    for(int i=0; i<64; i++){
        if(board % 2 == 1){
            n++;
        }
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
    for(int iterate=0; iterate<str_len; iterate++){
        char c = gamestring[iterate];

        switch(c){
            case 'r': add_to_board_br_to_tl(&bitfield_fig[r], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); break;
            case 'n': add_to_board_br_to_tl(&bitfield_fig[n], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); break;
            case 'b': add_to_board_br_to_tl(&bitfield_fig[b], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); break;
            case 'q': add_to_board_br_to_tl(&bitfield_fig[q], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); break;
            case 'k': add_to_board_br_to_tl(&bitfield_fig[k], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); break;
            case 'p': add_to_board_br_to_tl(&bitfield_fig[p], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); break;

            case 'R': add_to_board_br_to_tl(&bitfield_fig[r], i); add_to_board_br_to_tl(&bitfield_fig[w], i); break;
            case 'N': add_to_board_br_to_tl(&bitfield_fig[n], i); add_to_board_br_to_tl(&bitfield_fig[w], i); break;
            case 'B': add_to_board_br_to_tl(&bitfield_fig[b], i); add_to_board_br_to_tl(&bitfield_fig[w], i); break;
            case 'Q': add_to_board_br_to_tl(&bitfield_fig[q], i); add_to_board_br_to_tl(&bitfield_fig[w], i); break;
            case 'K': add_to_board_br_to_tl(&bitfield_fig[k], i); add_to_board_br_to_tl(&bitfield_fig[w], i); break;
            case 'P': add_to_board_br_to_tl(&bitfield_fig[p], i); add_to_board_br_to_tl(&bitfield_fig[w], i); break;

            case '/':  i--; break;

            default: i += (c - 48); i--; break; //skip ones
        }

        i++;
    }
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

void print_moves(field *boards){
    for(int i = 0; i < 32; i++){
        printf("Board %i \n", i);
        print_board(boards[i]);
    }    
}

void print_all_boards(field *boards){
    printf("White Board:\n");
    print_board(boards[w]);
    printf("Black Board:\n");
    print_board(boards[bl]);
    printf("Rook Board:\n");
    print_board(boards[r]);
    printf("Knight Board:\n");
    print_board(boards[n]);
    printf("Bishop Board:\n");
    print_board(boards[b]);
    printf("Queen Board:\n");
    print_board(boards[q]);
    printf("King Board:\n");
    print_board(boards[k]);
    printf("Pawn Board:\n");
    print_board(boards[p]);
}