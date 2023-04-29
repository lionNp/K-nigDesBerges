#ifndef BIT_BOARDS_UTIL_H
#define BIT_BOARDS_UTIL_H

#include "common.h"

field shift_diag_up(field diag, int n);
field shift_diag_down(field diag, int n);
void import_string(field boards[], char gamestring[]);
void print_board(field board);
void add_to_board_br_to_tl(field *board, int n);
void add_to_board_coords(field *board, int x, int y);
void print_moves(field *boards);
void print_all_boards(field *boards);
int num_pieces(field board);
void get_single_pieces(field bitfield_fig, field single_pieces_color[], int size_single_pieces_color);

#endif