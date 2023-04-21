#include "bords_util.h"

void import_string(field bitfield_fig[], char gamestring[]){
    printf("%s\n", gamestring);

    int str_len = strlen(gamestring);

    int i = 0;
    for(int iterate=0; iterate<str_len; iterate++){
        char c = gamestring[iterate];
        printf("%c", c);

        switch(c){
            case 'r': add_to_board_br_to_tl(&bitfield_fig[r], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); printf(" at %d\n", i); break;
            case 'n': add_to_board_br_to_tl(&bitfield_fig[n], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); printf(" at %d\n", i); break;
            case 'b': add_to_board_br_to_tl(&bitfield_fig[b], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); printf(" at %d\n", i); break;
            case 'q': add_to_board_br_to_tl(&bitfield_fig[q], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); printf(" at %d\n", i); break;
            case 'k': add_to_board_br_to_tl(&bitfield_fig[k], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); printf(" at %d\n", i); break;
            case 'p': add_to_board_br_to_tl(&bitfield_fig[p], i); add_to_board_br_to_tl(&bitfield_fig[bl], i); printf(" at %d\n", i); break;

            case 'R': add_to_board_br_to_tl(&bitfield_fig[r], i); add_to_board_br_to_tl(&bitfield_fig[w], i); printf(" at %d\n", i); break;
            case 'N': add_to_board_br_to_tl(&bitfield_fig[n], i); add_to_board_br_to_tl(&bitfield_fig[w], i); printf(" at %d\n", i); break;
            case 'B': add_to_board_br_to_tl(&bitfield_fig[b], i); add_to_board_br_to_tl(&bitfield_fig[w], i); printf(" at %d\n", i); break;
            case 'Q': add_to_board_br_to_tl(&bitfield_fig[q], i); add_to_board_br_to_tl(&bitfield_fig[w], i); printf(" at %d\n", i); break;
            case 'K': add_to_board_br_to_tl(&bitfield_fig[k], i); add_to_board_br_to_tl(&bitfield_fig[w], i); printf(" at %d\n", i); break;
            case 'P': add_to_board_br_to_tl(&bitfield_fig[p], i); add_to_board_br_to_tl(&bitfield_fig[w], i); printf(" at %d\n", i); break;

            case '/': printf(" skip\n"); i--; break;

            default: i += (c - 48); printf("skip %d\n", c-48); i--; break; //skip ones
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
    *board = *board | ((field) 1 << 63-n);
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