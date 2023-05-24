#include "evaluation.h"
#include "math.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include <stdlib.h>
#include "move_generator.h"

float evaluation(bool max_player)
{
    float total_rating = 0.0f;
    // evaluate move
    //stopwatch eval_time = start_stopwatch();
    //field time_pos = 0UL;
    //field time_con = 0UL;
    //field time_mat = 0UL;
    float position = evaluate_position(max_player);
    //time_pos = stop_stopwatch(eval_time);
    
    float control = evaluate_control(max_player);
    //time_con = stop_stopwatch(eval_time);
    
    float material = evaluate_material(max_player);
    //time_mat = stop_stopwatch(eval_time);
    //printf("con: %ld\n", time_con);
    //printf("pos: %ld\n", time_pos);
    //printf("mat: %ld\n", time_mat);
    //printf("Material: %f\nCrontrol: %f\nPosition: %f\n", material, control, position);
    total_rating = 25 * material + 1 * position + 1 * control;
    //printf("eval: %f took: %ldμs\n",total_rating,  time);
    return total_rating;
}

float evaluate_material(bool max_player){
    float material = 0.0f;
    /*
    field max_board = bitfields[max_player];
    field min_board = bitfields[!max_player];
    field board;

    for(int i = 0; i < 64; i++){
        board = 1UL << i;
        if(board & max_board)
            material += piece_value(board);
        else if(board & min_board)
            material -= piece_value(board);
    }
*/

    material += get_piece_count(bitfields[max_player] & bitfields[king]) * 100;
    
    material += get_piece_count(bitfields[max_player] & bitfields[queen]) * 9;

    material += get_piece_count(bitfields[max_player] & bitfields[rook]) * 5;

    material += get_piece_count(bitfields[max_player] & bitfields[bishop]) * 3;

    material += get_piece_count(bitfields[max_player] & bitfields[knight]) * 3;

    material += get_piece_count(bitfields[max_player] & bitfields[pawn]) * 1;

    material -= get_piece_count(bitfields[!max_player] & bitfields[king]) * 100;

    material -= get_piece_count(bitfields[!max_player] & bitfields[queen]) * 9;

    material -= get_piece_count(bitfields[!max_player] & bitfields[rook]) * 5;

    material -= get_piece_count(bitfields[!max_player] & bitfields[bishop]) * 3;

    material -= get_piece_count(bitfields[!max_player] & bitfields[knight]) * 3;

    material -= get_piece_count(bitfields[!max_player] & bitfields[pawn]) * 1;

    return material;
}

float piece_value(field board){
    float value = 0;
    for(int i = 7; i > 1; i--){
        if(board & bitfields[i]){
            switch(i){
                case pawn:
                    value = 1;
                    break;
                case knight:
                    value = 3;
                    break;
                case bishop:
                    value = 3;
                    break;
                case rook:
                    value = 5;
                    break;
                case queen:
                    value = 9;
                    break;
                case king:
                    value = 100;
                    break;
            }
            break;
        }
    }
    return value;
}

float evaluate_control(bool max_player){
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
    return (float) (max_control - min_control);
}

float evaluate_position(bool max_player){
    float position = 0.0f;
    field max_board = bitfields[max_player];
    field min_board = bitfields[!max_player];
    for(int i = 2; i < 8; i++){
        field max_pieces = max_board & bitfields[i];
        int max_piece_count = get_piece_count(max_pieces);
        field single_max_piece_boards[max_piece_count];
        get_single_piece_boards(max_pieces, single_max_piece_boards, max_piece_count);
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
        }
        field min_pieces = min_board & bitfields[i];
        int min_piece_count = get_piece_count(min_pieces);
        field single_min_piece_boards[min_piece_count];
        get_single_piece_boards(min_pieces, single_min_piece_boards, min_piece_count);
        for(int k = 0; k < min_piece_count; k++){
            int bit = log2(single_min_piece_boards[k]);
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
    return position;
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