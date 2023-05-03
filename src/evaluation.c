#include "evaluation.h"
#include "math.h"
#include "bit_boards_util.h"
#include "move_util.h"

float evaluation(field bitfield[], field move_to, field move_from, int piece){
    float rating = 0.0;
    // make move
    // move piece in turn_board
    bitfield[turn] ^= move_from;
    bitfield[turn] ^= move_to;
    // elimenate piece if taken
    int flag = 0;
    if(move_to & bitfield[!turn]){
        bitfield[!turn] ^= (move_to & bitfield[!turn]);
        flag = 1;
    }
    // check for check
    field king_position = 0UL;
    if(piece == king)
        king_position = bitfield[turn] & move_to;
    else
        king_position = bitfield[turn] & bitfield[king];
    field checked = in_check(king_position, bitfield);
    
    if(checked){ 
        // unmake move
        bitfield[turn] ^= move_from;
        bitfield[turn] ^= move_to;   
        if(flag)
            bitfield[!turn] |= move_to;
        rating = -9999;
        return rating;
    }
    // evaluate move
    int pos_to = log2(move_to);
    int pos_from = log2(move_from);
    switch(piece){
        case pawn: 
            if(turn == 0)
                rating += black_pawn_values[pos_to] - black_pawn_values[pos_from];
            else 
                rating += white_pawn_values[pos_to] - white_pawn_values[pos_from];
            break;
        case king: 
            if(turn == 0)
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
    //field turn_king = bitfield[turn] & bitfield[king];
    //rating += num_pieces(turn_king)*100;
    field turn_queen = bitfield[turn] & bitfield[queen];
    rating += num_pieces(turn_queen) * 9;
    field turn_rook = bitfield[turn] & bitfield[rook];
    rating += num_pieces(turn_rook) * 5;
    field turn_bishop = bitfield[turn] & bitfield[bishop];
    rating += num_pieces(turn_bishop) * 3;
    field turn_knight = bitfield[turn] & bitfield[knight];
    rating += num_pieces(turn_knight) * 3;
    field turn_pawns = bitfield[turn] & bitfield[pawn];
    rating += num_pieces(turn_pawns) * 1;

    //field turn_next_king = bitfield[!turn] & bitfield[king];
    //rating -= num_pieces(turn_next_king)*100;
    field turn_next_queen = bitfield[!turn] & bitfield[queen];
    rating -= num_pieces(turn_next_queen) * 9;
    field turn_next_rook = bitfield[!turn] & bitfield[rook];
    rating -= num_pieces(turn_next_rook) * 5;
    field turn_next_bishop = bitfield[!turn] & bitfield[bishop];
    rating -= num_pieces(turn_next_bishop) * 3;
    field turn_next_knight = bitfield[!turn] & bitfield[knight];
    rating -= num_pieces(turn_next_knight) * 3;
    field turn_next_pawns = bitfield[!turn] & bitfield[pawn];
    rating -= num_pieces(turn_next_pawns) * 1;


    //unmake move
    // move piece in piece_board
    // move piece in turn_board
    bitfield[turn] ^= move_from;
    bitfield[turn] ^= move_to;   
    // elimenate piece if taken
    if(flag)
        bitfield[!turn] |= move_to;

    return rating;
}