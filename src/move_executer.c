#include "move_util.h"
#include "bit_boards_util.h"
#include "common.h"
#include <math.h>

void make_move(int piece, field moves_from, field moves_to, field captured[]){
    if(piece == king){
        if(log2(moves_from) - log2(moves_to) == 2){
            bitfields[rook] ^= ((moves_to >> 1) |  (moves_to << 1));
            bitfields[is_player_white] ^= ((moves_to >> 1) |  (moves_to << 1));
        }
        if(log2(moves_from) - log2(moves_to) == -2){
            bitfields[rook] ^= ((moves_to << 2) |  (moves_to >> 1));
            bitfields[is_player_white] ^= ((moves_to << 2) |  (moves_to >> 1));
        }
    }
    // move current_piece in piece_board
    // remove piece from old square
    bitfields[is_player_white] ^= moves_from;
    bitfields[piece] ^= moves_from;

    // eliminate all pieces from moved to square
    if(moves_to & bitfields[!is_player_white]) // if for loop necessary
        for(int i = 0; i < 8; i++){
             captured[i] = (moves_to & bitfields[i]);
             bitfields[i] ^= (moves_to & bitfields[i]);
        }
             

    // add piece at moved to square and check for promotion
    if(piece == pawn & (moves_to & (row_1 | row_8))){
        bitfields[queen] ^= moves_to;
        bitfields[is_player_white] ^= moves_to;
    }
    else{
        bitfields[is_player_white] ^= moves_to;
        bitfields[piece] ^= moves_to;
    }

    if(piece == king)
    {
        castle_left[is_player_white] = false;
        castle_right[is_player_white] = false;
    }
    else if(piece == rook)
    {
        if(moves_from & h_file)
            castle_right[is_player_white] = false;

        else if(moves_from & a_file)
            castle_left[is_player_white] = false;
    }
}