#include <string.h>

typedef uint64_t field;
#define figuren_anz 8

enum figuren_util{
    bl,
    w,
    k,
    q,
    r,
    b,
    n,
    p
};

void print_board(field board);
void add_to_board_br_to_tl(field *board, int n);
void add_to_board_coords(field *board, int x, int y);
void import_string(field bitfield_fig[], char gamestring[]);
void print_all_boards(field *boards);