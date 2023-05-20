#include "evaluation.h"
#include "math.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include <stdlib.h>
#include "move_generator.h"

float evaluation(bool max_player)
{
    float total_rating = 0.0f;
    float position = 0.0f;
    float control = 0.0f;
    float material = 0.0f;
    // evaluate move
    stopwatch eval_time = start_stopwatch();
    field time = 0UL;
    field max_board = bitfields[max_player];
    field min_board = bitfields[!max_player];

    for(int i = 2; i < 8; i++){
        field max_pieces = max_board & bitfields[i];
        int max_piece_count = get_piece_count(max_pieces);
        field single_max_piece_boards[max_piece_count];
        get_single_piece_boards(max_pieces, single_max_piece_boards, max_piece_count);
        
        field min_pieces = min_board & bitfields[i];
        int min_piece_count = get_piece_count(min_pieces);
        field single_min_piece_boards[min_piece_count];
        get_single_piece_boards(min_pieces, single_min_piece_boards, min_piece_count);
        for(int k = 0; k < max_piece_count; k++){
            int bit = log2(single_max_piece_boards[k]);
            switch(i)
            {
                case pawn: 
                    position += max_player
                        ? white_pawn_values[bit]
                        : black_pawn_values[bit];
                    break;

                case king:
                    if(max_pieces & koth)
                        return winning_move;       
                    position += max_player
                        ? white_king_values[bit]
                        : black_king_values[bit];
                    break;

                case queen: 
                    position += queen_values[bit];
                    break;

                case rook: 
                    position += rook_values[bit];
                    break;

                case knight: 
                    position += knight_values[bit];
                    break;

                case bishop: 
                    position += bishop_values[bit];
                    break;
            }
            bit = log2(single_min_piece_boards[k]);
            for(int k = 0; k < min_piece_count; k++){
                switch(i)
                {
                    case pawn: 
                        position -= !max_player
                            ? white_pawn_values[bit]
                            : black_pawn_values[bit];
                        break;

                    case king:
                        if(min_pieces & koth)
                            return losing_move;       
                        position -= !max_player
                            ? white_king_values[bit]
                            : black_king_values[bit];
                        break;

                    case queen: 
                        position -= queen_values[bit];
                        break;

                    case rook: 
                        position -= rook_values[bit];
                        break;

                    case knight: 
                        position -= knight_values[bit];
                        break;

                    case bishop: 
                        position -= bishop_values[bit];
                        break;
                }
            }
        }
    }

    
    field max_danger_squares[16] = {0UL};
    field min_danger_squares[16] = {0UL};
    generate_attacked_squares(max_danger_squares, max_player);
    generate_attacked_squares(min_danger_squares, !max_player);
    for(int i = 0; i < 16; i++){
        max_danger_squares[0] |= max_danger_squares[i];
        min_danger_squares[0] |= min_danger_squares[i];
    }
    field max_danger = max_danger_squares[0] & (max_danger_squares[0] ^ min_danger_squares[0]);
    field min_danger = min_danger_squares[0] & (max_danger_squares[0] ^ min_danger_squares[0]);
    int max_control = get_piece_count(max_danger);
    int min_control = get_piece_count(min_danger);
    control = max_control - min_control;
    
    // increase rating by piece count * piece_multiplier
    // matertial
    //field turn_king = bitfields[is_player_white] & bitfields[king];
    //rating += get_piece_count(turn_king)*100;
    field turn_king = bitfields[max_player] & bitfields[king];
    material += get_piece_count(turn_king) * 100;
    
    field turn_queen = bitfields[max_player] & bitfields[queen];
    material += get_piece_count(turn_queen) * 9;

    field turn_rook = bitfields[max_player] & bitfields[rook];
    material += get_piece_count(turn_rook) * 5;

    field turn_bishop = bitfields[max_player] & bitfields[bishop];
    material += get_piece_count(turn_bishop) * 3;

    field turn_knight = bitfields[max_player] & bitfields[knight];
    material += get_piece_count(turn_knight) * 3;

    field turn_pawns = bitfields[max_player] & bitfields[pawn];
    material += get_piece_count(turn_pawns) * 1;

    //decrease rating by enemy players piece count * piece_multiplier
    //field turn_next_king = bitfields[!is_player_white] & bitfields[king];
    //rating -= get_piece_count(turn_next_king)*100;
    field turn_next_king = bitfields[!max_player] & bitfields[king];
    material -= get_piece_count(turn_next_king) * 100;
    
    field turn_next_queen = bitfields[!max_player] & bitfields[queen];
    material -= get_piece_count(turn_next_queen) * 9;

    field turn_next_rook = bitfields[!max_player] & bitfields[rook];
    material -= get_piece_count(turn_next_rook) * 5;

    field turn_next_bishop = bitfields[!max_player] & bitfields[bishop];
    material -= get_piece_count(turn_next_bishop) * 3;

    field turn_next_knight = bitfields[!max_player] & bitfields[knight];
    material -= get_piece_count(turn_next_knight) * 3;

    field turn_next_pawns = bitfields[!max_player] & bitfields[pawn];
    material -= get_piece_count(turn_next_pawns) * 1;


    total_rating = 3 * material + 0.2 * position + 1 * control;
    time = stop_stopwatch(eval_time);
    //printf("eval: %f took: %ldμs\n",total_rating,  time);
    return total_rating;
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

int max_rating(float rating[], int move_count){
    int max_rating_index = 0;
    for(int c = 0; c < move_count; c++){
        if(rating[c] > rating[max_rating_index])
            max_rating_index = c; 
    } 
    return max_rating_index;
}