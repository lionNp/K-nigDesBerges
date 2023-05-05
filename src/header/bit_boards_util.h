#ifndef BIT_BOARDS_UTIL_H
#define BIT_BOARDS_UTIL_H

#include "common.h"

field shift_diag_up(field diag, int n);
field shift_diag_down(field diag, int n);
void import_gamesting(field boards[], char gamestring[]);
void print_board(field board);
void game_finished(bool gameover, int total_legal_moves);
void set_bit_by_index(field *board, int n);
void add_to_board_coords(field *board, int x, int y);
void print_moves(field *boards, char *wpiece);
void print_all_boards(field *boards);
int get_piece_count(field board);
void get_single_piece_boards(field bitfields, field single_pieces_color[], int size_single_pieces_color);
void print_pos(field position);
void print_move(field start, field end);

#endif
