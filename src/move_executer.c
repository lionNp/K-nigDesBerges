#include "move_util.h"
#include "bit_boards_util.h"
#include "common.h"
#include <math.h>

void make_move(int piece_index[], field moves[], int max_rating_index){
    if(piece_index[max_rating_index] == king){
        if(log2(moves[max_rating_index]) - log2(moves[max_rating_index+1]) == 2){
            bitfields[rook] ^= ((moves[max_rating_index+1] >> 1) |  (moves[max_rating_index+1] << 1));
            bitfields[is_player_white] ^= ((moves[max_rating_index+1] >> 1) |  (moves[max_rating_index+1] << 1));
        }
        if(log2(moves[max_rating_index]) - log2(moves[max_rating_index+1]) == -2){
            bitfields[rook] ^= ((moves[max_rating_index+1] << 2) |  (moves[max_rating_index+1] >> 1));
            bitfields[is_player_white] ^= ((moves[max_rating_index+1] << 2) |  (moves[max_rating_index+1] >> 1));
        }
    }
    // move current_piece in piece_board
    // remove piece from old square
    bitfields[is_player_white] ^= moves[max_rating_index];
    bitfields[piece_index[max_rating_index]] ^= moves[max_rating_index];

    // eliminate all pieces from moved to square
    for(int i = 0; i < 8; i++)
        bitfields[i] ^= (moves[max_rating_index+1] & bitfields[i]); 

    // add piece at moved to square
    bitfields[is_player_white] ^= moves[max_rating_index+1];
    bitfields[piece_index[max_rating_index]] ^= moves[max_rating_index+1];

    if(piece_index[max_rating_index] == king)
    {
        castle_left[is_player_white] = false;
        castle_right[is_player_white] = false;
    }
    else if(piece_index[max_rating_index] == rook)
    {
        if(moves[max_rating_index] & h_file)
            castle_right[is_player_white] = false;

        else if(moves[max_rating_index] & a_file)
            castle_left[is_player_white] = false;
    }
}