#include "evaluation.h"
#include "math.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include <stdlib.h>

float evaluation(field move_from, field move_to, int piece)
{
    float rating = 0.0f;
    // evaluate move
    int pos_to = log2(move_to);
    int pos_from = log2(move_from);
    
    switch(piece)
    {
        case pawn: 
            rating += is_player_white
                ? white_pawn_values[pos_to] - white_pawn_values[pos_from]
                : black_pawn_values[pos_to] - black_pawn_values[pos_from];
            break;

        case king: 
            if(move_to & koth)
                return winning_move;
            if((pos_from - pos_to) == 2)
                rating += rook_values[pos_to + 1] - rook_values[pos_to - 1];
            else if((pos_from - pos_to) == -2)
                rating += rook_values[pos_to - 1] - rook_values[pos_to + 2];
                
            rating += is_player_white
                ? white_king_values[pos_to] - white_king_values[pos_from]
                : black_king_values[pos_to] - black_king_values[pos_from];
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

    rating /= 5;
    
    // increase rating by piece count * piece_multiplier
    // matertial
    //field turn_king = bitfields[is_player_white] & bitfields[king];
    //rating += get_piece_count(turn_king)*100;
    field turn_king = bitfields[is_player_white] & bitfields[king];
    rating += get_piece_count(turn_king) * 100;
    
    int promotion = 9;
    if(piece == pawn & (move_to & (rank_1 | rank_8)))
        promotion = 18;
    field turn_queen = bitfields[is_player_white] & bitfields[queen];
    rating += get_piece_count(turn_queen) * promotion;

    field turn_rook = bitfields[is_player_white] & bitfields[rook];
    rating += get_piece_count(turn_rook) * 5;

    field turn_bishop = bitfields[is_player_white] & bitfields[bishop];
    rating += get_piece_count(turn_bishop) * 3;

    field turn_knight = bitfields[is_player_white] & bitfields[knight];
    rating += get_piece_count(turn_knight) * 3;

    field turn_pawns = bitfields[is_player_white] & bitfields[pawn];
    rating += get_piece_count(turn_pawns) * 1;

    //decrease rating by enemy players piece count * piece_multiplier
    //field turn_next_king = bitfields[!is_player_white] & bitfields[king];
    //rating -= get_piece_count(turn_next_king)*100;
    field turn_next_king = bitfields[!is_player_white] & bitfields[king];
    rating += get_piece_count(turn_next_king) * 100;
    
    field turn_next_queen = bitfields[!is_player_white] & bitfields[queen];
    rating -= get_piece_count(turn_next_queen) * 9;

    field turn_next_rook = bitfields[!is_player_white] & bitfields[rook];
    rating -= get_piece_count(turn_next_rook) * 5;

    field turn_next_bishop = bitfields[!is_player_white] & bitfields[bishop];
    rating -= get_piece_count(turn_next_bishop) * 3;

    field turn_next_knight = bitfields[!is_player_white] & bitfields[knight];
    rating -= get_piece_count(turn_next_knight) * 3;

    field turn_next_pawns = bitfields[!is_player_white] & bitfields[pawn];
    rating -= get_piece_count(turn_next_pawns) * 1;

    return rating;
}

int random_max_rating(float rating[], int move_count){
    int max_rating_indices[3] = {0, 0, 0};
    int max_rating_index = 0;
    for(int c = 0; c < move_count; c++){
        if(rating[c] > rating[max_rating_index]){
            max_rating_index = c;
            max_rating_indices[2] = max_rating_indices[1];
            max_rating_indices[1] = max_rating_indices[0];
            max_rating_indices[0] = c;
        }     
    }
    int comp = 0;
    int non_zero = 0;
    for(int i = 0; i < 3; i++){
        //printf("%d ", max_rating_indices[i]);
        comp += rating[max_rating_indices[i]];     
    }
    //printf("\n");
    int idx = max_rating_index;
    if((comp / 3 * rating[max_rating_index]) > 0.6)
        idx = max_rating_indices[rand() % 2];
    
    return idx;
}