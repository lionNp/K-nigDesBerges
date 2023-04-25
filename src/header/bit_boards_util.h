#include <string.h>
#include <stdint.h>

typedef uint64_t field;
#define figuren_anz 8

field down_diag = 0x8040201008040201;
field up_diag = 0x102040810204080;
field horizontal = 0xFF;
field vertical = 0x8080808080808080;

enum figuren_util{
    bl = 0,
    w = 1,
    k = 2,
    q = 3,
    r = 4,
    b = 5,
    n = 6,
    p = 7
};

field shift_diag_up(field diag, int n);
field shift_diag_down(field diag, int n);
void import_string(field boards[], char gamestring[]);
void print_board(field board);
void add_to_board_br_to_tl(field *board, int n);
void add_to_board_coords(field *board, int x, int y);
void print_all_boards(field *boards);
int num_pieces(field board);
void get_single_pieces(field bitfield_fig, field single_pieces_color[], int size_single_pieces_color);

#include "bit_boards_util.c"