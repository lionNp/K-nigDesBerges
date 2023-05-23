#ifndef BIT_BOARDS_UTIL_H
#define BIT_BOARDS_UTIL_H

#include "common.h"

// import a FEN string and populate bitboards
void import_gamestring(field boards[], char gamestring[]);

// print a random bitboard
void print_board(field board);

// print a random move bitboard
void print_move_2(field board);
void print_move(field start, field end);

void white_piece_print(int piece_type);
void black_piece_print(int piece_type);

// print current chessboard
void print_full_board();

// check if game is finished | i.e. 3 repeated moves, checkmate (in check and no legal moves), stalemate
bool game_finished(int total_legal_moves);

// set bit in field at index
void set_bit_by_index(field *board, int n);

// add piece to board at coordinates
void add_to_board_coords(field *board, int x, int y);

// prints all defined bitboards
void print_all_boards(field *boards);

// counts all pieces on the provided board
int get_piece_count(field board);

// takes a board and splits every piece into a seperate board containing only that piece
void get_single_piece_boards(field bitfields, field single_pieces_color[], int size_single_pieces_color);

// print a position
void print_pos(field position);

#endif
