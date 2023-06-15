#include "evaluation.h"
#include "math.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include <stdlib.h>
#include "move_generator.h"

float evaluation(){
    float total_rating = 0.0f;
    // evaluate move
    //stopwatch eval_time = start_stopwatch();
    //field time_pos = 0UL;
    //field time_con = 0UL;
    //field time_mat = 0UL;
    float position = evaluate_position();
    //time_pos = stop_stopwatch(eval_time);
    
    float control = evaluate_control();
    //time_con = stop_stopwatch(eval_time);
    
    float material = evaluate_material();

    float pawns = evaluate_pawns();
    //if(pawns != 0){
    //    printf("%f\n", pawns);
    //    print_full_board();
    //}
        
    //time_mat = stop_stopwatch(eval_time);
    //printf("con: %ld\n", time_con);
    //printf("pos: %ld\n", time_pos);
    //printf("mat: %ld\n", time_mat);
    //printf("Material: %f\nCrontrol: %f\nPosition: %f\n", material, control, position);
    total_rating = 20 * material + position + 2 * control + pawns;
    //printf("eval: %f took: %ldμs\n",total_rating,  time);
    return total_rating;
}

float evaluate_pawns(){
    float passed_p = 0.0f;
    float stacked_p = 0.0f;
    int pp_value = 3;
    int sp_value = 1;
    field black_pawns = bitfields[pawn] & bitfields[black];
    field white_pawns = bitfields[pawn] & bitfields[white];
    for(int k = 0; k < 8; k++){
        field black_pawns_file = black_pawns & files[k];
        field white_pawns_file = white_pawns & files[k];
        field pawns_in_file = bitfields[pawn] & files[k];

        int black_pawns_in_file = get_piece_count(black_pawns_file);
        int white_pawns_in_file = get_piece_count(white_pawns_file);

        if(white_pawns_in_file > 1)
            stacked_p -= sp_value * (white_pawns_in_file - 1);
        if(black_pawns_in_file > 1)
            stacked_p += sp_value * (black_pawns_in_file - 1);
        // check and redo
        if(k == 0){
            field pp_white = black_pawns_file | (black_pawns & files[1]);
            field pp_black = white_pawns_file | (white_pawns & files[1]);
            if(!pp_white && white_pawns_file)
                passed_p += pp_value;
            if(!pp_black && black_pawns_file)
                passed_p -= pp_value;
        }
        else if(k == 7){
            field pp_white = black_pawns_file | (black_pawns & files[6]);
            field pp_black = white_pawns_file | (white_pawns & files[6]);
            if(!pp_white && white_pawns_file)
                passed_p += pp_value;
            if(!pp_black && black_pawns_file)
                passed_p -= pp_value;
        }
        else{
            field pp_white = black_pawns_file | (black_pawns & files[k-1] & files[k+1]);
            field pp_black = white_pawns_file | (white_pawns & files[k-1] & files[k+1]);
            if(!pp_white && white_pawns_file)
                passed_p += pp_value;
            if(!pp_black && black_pawns_file)
                passed_p -= pp_value;
        }
    }
    //if(passed_p + stacked_p)
    //    printf("%f\n", passed_p + stacked_p);
    return passed_p + stacked_p;
}

float evaluate_material(){
    float material = 0.0f;

    material += get_piece_count(bitfields[white] & bitfields[king]) * 100;
    
    material += get_piece_count(bitfields[white] & bitfields[queen]) * 9;

    material += get_piece_count(bitfields[white] & bitfields[rook]) * 5;

    material += get_piece_count(bitfields[white] & bitfields[bishop]) * 3;

    material += get_piece_count(bitfields[white] & bitfields[knight]) * 3;

    material += get_piece_count(bitfields[white] & bitfields[pawn]) * 1;

    material -= get_piece_count(bitfields[black] & bitfields[king]) * 100;

    material -= get_piece_count(bitfields[black] & bitfields[queen]) * 9;

    material -= get_piece_count(bitfields[black] & bitfields[rook]) * 5;

    material -= get_piece_count(bitfields[black] & bitfields[bishop]) * 3;

    material -= get_piece_count(bitfields[black] & bitfields[knight]) * 3;

    material -= get_piece_count(bitfields[black] & bitfields[pawn]) * 1;

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

float evaluate_control(){
    field white_ds[16] = {0UL};
    field black_ds[16] = {0UL};
    generate_attacked_squares(white_ds, white);
    generate_attacked_squares(black_ds, black);
    int w_c_control = 0;
    int b_c_control = 0;
    for(int i = 0; i < 16; i++){
        w_c_control += get_piece_count(white_ds[i] & koth);
        b_c_control += get_piece_count(black_ds[i] & koth);
        white_ds[0] |= white_ds[i];
        black_ds[0] |= black_ds[i];
    }
    field w_danger = white_ds[0] & (white_ds[0] ^ black_ds[0]);
    field b_danger = black_ds[0] & (white_ds[0] ^ black_ds[0]);
    int w_control = get_piece_count(w_danger);
    int b_control = get_piece_count(b_danger);
    return (float) (w_control - b_control + w_c_control - b_c_control);
}

float evaluate_position(){
    float position = 0.0f;
    field max_board = bitfields[white];
    field min_board = bitfields[black];
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
                    position += white_pawn_values[bit];
                    break;

                case king:
                    if(max_pieces & koth)
                        return winning_move;       
                    position += white_king_values[bit];
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
                    position -= black_pawn_values[bit];
                    break;

                case king:
                    if(min_pieces & koth)
                        return losing_move;       
                    position -= black_king_values[bit];
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

int min_rating(float rating[], int move_count){
    int min_rating_index = 0;
    for(int c = 0; c < move_count; c++){
        if(rating[c] < rating[min_rating_index])
            min_rating_index = c; 
    } 
    return min_rating_index;
}