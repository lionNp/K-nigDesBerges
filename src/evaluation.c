#include "evaluation.h"
#include "math.h"
#include "bit_boards_util.h"
#include "move_util.h"

float evaluation(field bitfield[], field move_to, field move_from, int piece){
    float rating = 0.0;
    // make move
    // move piece in turn_board
    bitfield[is_player_white] ^= move_from;
    bitfield[is_player_white] ^= move_to;
    // elimenate piece if taken
    int flag = 0;
    if(move_to & bitfield[!is_player_white]){
        bitfield[!is_player_white] ^= (move_to & bitfield[!is_player_white]);
        flag = 1;
    }
    // check for check
    field king_position = 0UL;
    if(piece == king)
        king_position = bitfield[is_player_white] & move_to;
    else
        king_position = bitfield[is_player_white] & bitfield[king];
    field checked = in_check(king_position, bitfield);
    if(checked != (field) 0){ 
        // set castle flag
        castle_left[is_player_white] = false;
        castle_right[is_player_white] = false;
        // unmake move
        bitfield[is_player_white] ^= move_from;
        bitfield[is_player_white] ^= move_to;   
        if(flag)
            bitfield[!is_player_white] |= move_to;
        return 0;
    }
    // evaluate move
    int pos_to = log2(move_to);
    int pos_from = log2(move_from);
    switch(piece){
        case pawn: 
            if(is_player_white == 0)
                rating += black_pawn_values[pos_to] - black_pawn_values[pos_from];
            else 
                rating += white_pawn_values[pos_to] - white_pawn_values[pos_from];
            break;
        case king: 
            if(is_player_white == 0)
                rating += black_king_values[pos_to] - black_king_values[pos_from];
            else 
                rating += white_king_values[pos_to] - white_king_values[pos_from];
            break;
        case queen: 
            rating += queen_values[pos_to] - queen_values[pos_from];
            break;
        case rook: 
            rating += rook_values[pos_to] - rook_values[pos_from];
            break;
        case knight: 
            rating += knight_values[pos_to] - knight_values[pos_from];
            break;
        case bishop: 
            rating += bishop_values[pos_to] - bishop_values[pos_from];
            break;
    }   

    // matertial
    //field turn_king = bitfield[is_player_white] & bitfield[king];
    //rating += get_piece_count(turn_king)*100;
    field turn_queen = bitfield[is_player_white] & bitfield[queen];
    rating += get_piece_count(turn_queen) * 9;
    field turn_rook = bitfield[is_player_white] & bitfield[rook];
    rating += get_piece_count(turn_rook) * 5;
    field turn_bishop = bitfield[is_player_white] & bitfield[bishop];
    rating += get_piece_count(turn_bishop) * 3;
    field turn_knight = bitfield[is_player_white] & bitfield[knight];
    rating += get_piece_count(turn_knight) * 3;
    field turn_pawns = bitfield[is_player_white] & bitfield[pawn];
    rating += get_piece_count(turn_pawns) * 1;

    //field turn_next_king = bitfield[!is_player_white] & bitfield[king];
    //rating -= get_piece_count(turn_next_king)*100;
    field turn_next_queen = bitfield[!is_player_white] & bitfield[queen];
    rating -= get_piece_count(turn_next_queen) * 9;
    field turn_next_rook = bitfield[!is_player_white] & bitfield[rook];
    rating -= get_piece_count(turn_next_rook) * 5;
    field turn_next_bishop = bitfield[!is_player_white] & bitfield[bishop];
    rating -= get_piece_count(turn_next_bishop) * 3;
    field turn_next_knight = bitfield[!is_player_white] & bitfield[knight];
    rating -= get_piece_count(turn_next_knight) * 3;
    field turn_next_pawns = bitfield[!is_player_white] & bitfield[pawn];
    rating -= get_piece_count(turn_next_pawns) * 1;


    //unmake move
    // move piece in piece_board
    // move piece in turn_board
    bitfield[is_player_white] ^= move_from;
    bitfield[is_player_white] ^= move_to;   
    // elimenate piece if taken
    if(flag)
        bitfield[!is_player_white] |= move_to;

    return rating;
}