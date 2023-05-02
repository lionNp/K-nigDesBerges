#include "evaluation.h"
#include "math.h"
#include "bit_boards_util.h"

float evaluation(field bitfield[], field move_to, field move_from, int piece){
    float rating = 0.0;
    // make move
    // move piece in turn_board
    bitfield[turn] ^= move_from;
    bitfield[turn] ^= move_to;
    // move piece in piece_board
    bitfield[piece] ^= move_from;
    bitfield[piece] |= move_to;

    for(int i = 2; i < 8; i++)
        bitfield[i] ^= (move_to & bitfield[i]); 
    
    // elimenate piece if taken
    bitfield[!turn] ^= (move_to & bitfield[!turn]);

    int pos_to = log2(move_to);
    int pos_from = log2(move_from);
    // evaluate
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

    /*
    //unmake move
    // move piece in piece_board
    bitfield[piece] ^= move_to;
    bitfield[piece] ^= move_from;
    // move piece in color_board
    bitfield[turn] ^= move_to;
    bitfield[turn] ^= move_from;
    // restore piece if moved onto
    bitfield[!turn] = bitfield_opponent;
    */

    return rating;
}