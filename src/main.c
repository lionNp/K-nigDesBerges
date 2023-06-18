#include "main.h"
#include <math.h>
#include "common.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include "evaluation.h"
#include "stopwatch_utils.h"
#include "move_generator.h"
#include "move_executer.h"
#include <stdlib.h>
#include "hashset.h"
#include "alphabeta.h"

int main() {
    //initilize board
    import_gamestring(bitfields, game_string);

    int count_total_moves = 0;
    // total match duration
    stopwatch total_time = start_stopwatch();
    field match_duration;
    int total_pieces = 32;
    while(!gameover)
    //for(int r = 0; r < 1; r++)
    {
        //time
        stopwatch turn_time = start_stopwatch();
        field t = 0UL;
        //initilizing moves
        field moves_from[max_move_count];
        field moves_to[max_move_count];
        int piece_idx[max_move_count];
        //alpha beta initilizing
        float alpha = losing_move;
        float beta = winning_move;
        bool max_player = is_player_white;
        //get moves and set rating
        int move_count = generate_moves(moves_from, moves_to, piece_idx);


        //Nullzugsuche für die erste Iterationsebene
        bool null_zug_suche = true;
        if(null_zug_suche)
        {
            float baseline_evaluation = evaluation();

            //if black invert
            if(!is_player_white)
            {
                baseline_evaluation *= -1.0f;
            }

            float evaluations[move_count];

            //first iteration
            for(int i=0; i<move_count; i++)
            {
                
                field captured[8] = {0UL};
                make_move(piece_idx[i], moves_from[i], moves_to[i], captured);

                field extra_moves_from[max_move_count];
                field extra_moves_to[max_move_count];
                int extra_piece_idx[max_move_count];
                
                field extra_captured[8] = {0UL};

                //player doesnt get changed

                int extra_move_count = generate_moves(extra_moves_from, extra_moves_to, extra_piece_idx);
                for(int extra_i; extra_i < extra_move_count; extra_i++)
                {
                    make_move(extra_piece_idx[i], extra_moves_from[i], extra_moves_to[i], extra_captured);

                    float extra_evaluation = evaluation();

                    if(is_player_white)
                    {
                        if(evaluations[i] < extra_evaluation)
                        {
                            evaluations[i] = extra_evaluation;
                        }
                    }

                    //black evaluation will be negated
                    else
                    {
                        if(evaluations[i] < extra_evaluation * -1.0f)
                        {
                            evaluations[i] = extra_evaluation * -1.0f;
                        }
                    }

                    unmake_move(extra_piece_idx[i], extra_moves_from[i], extra_moves_to[i], extra_captured);
                }
                //at this point evaluations is an array containing the abs of the evaluation of the best move 
                //possible when doing a double move

                unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            }

            //flag lame moves
            int lame_moves_count = 0;
            for(int i=0; i<move_count; i++)
            {
                if(baseline_evaluation / evaluations[i] < 0.2f)
                {
                    //printf("langweiliger zug gefunden\n");
                    moves_from[i] = 0UL;
                    moves_to[i] = 0UL;

                    lame_moves_count++;
                }
            }

            //remove lame moves by shifting everything to the start and changing move_count
            printf("removed %d/%d moves due to them being not volatil enough\n", lame_moves_count, move_count);
            move_count -= lame_moves_count;
            int real_i = 0;
            for(int i=0; i<move_count + lame_moves_count; i++)
            {
                //if move didnt get flagged, everything is fine
                if(moves_from[real_i] != 0UL && moves_to[real_i] != 0UL)
                {
                    real_i++;
                    continue;
                }

                //if it did get flagged, 
                for(int f=i; f<move_count + lame_moves_count; f++)
                {
                    //copy next move to real_i field
                    if(moves_from[f] != 0UL)
                    {
                        moves_from[real_i] = moves_from[f];
                        moves_to[real_i] = moves_to[f];
                        piece_idx[real_i] = piece_idx[f];

                        moves_from[f] = 0UL;
                        moves_to[f] = 0UL;
                        piece_idx[f] = 0;
                        break;
                    }
                }

            }
        }

        float rating[move_count];
        float final_rating[move_count];
        
        if(move_count == 0){
            printf("check mate!\n");
            printf("%s won!\n", !is_player_white ? "white" : "black");
            field tt = stop_stopwatch(total_time);
            print_full_board();
            printf("Match took %d moves took %luμs\n", count_total_moves, tt);
            return 0;
        }
        int break_after = 2000000;
        int depth = 0;
        num_moves_iterated = 0;
        num_moves_trans = 0;

        /* reset hashtable if piece taken
        */
        /*
        int new_piece_count = get_piece_count(bitfields[white] | bitfields[black]);
        if(total_pieces != new_piece_count){
            total_pieces = new_piece_count;
            hash_table = {0UL};
        }
        */

       float pv_score = 0.0f;
       float score;

        for(; t < break_after; depth++){ 
            for(int i = 0; i < move_count; i++){

                if(stop_stopwatch(turn_time) > break_after) break;

                field captured[8] = {0UL};
                bool castle_flags_left[2];
                bool castle_flags_right[2];

                memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
                memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

                make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
                is_player_white = !is_player_white;
                //rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta(depth, alpha, beta, max_player);
                if(depth < 3)
                    rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta(depth, alpha, beta, max_player);
                else
                    rating[i] = (1 + (depth % 2) * tempo_bonus) * pvs(depth, alpha, beta, max_player);
                is_player_white = !is_player_white;
                unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

                memcpy(castle_left,castle_flags_left,sizeof(castle_left));
                memcpy(castle_right,castle_flags_right,sizeof(castle_right));
            }

            for(int i = 0; i < move_count; i++)
                final_rating[i] = rating[i];

            //sort the moves depending on rating
            sort_moves(final_rating, moves_from, moves_to, piece_idx, move_count);
            //filter_moves_quietsearch();
            t = stop_stopwatch(turn_time);
        }

        float round_time = ((float) t / 1000000);
        printf("time: %fs at depth: %d\n", round_time, depth);
        printf("Total moves: %d --- Evaluated Moves: %d%%\n", num_moves_iterated, 100 * num_moves_trans / num_moves_iterated);
        int idx = 0;
        if(is_player_white)
            idx = max_rating(final_rating, move_count);
        else
            idx = min_rating(final_rating, move_count);
        
        //make move
        field captured[8] = {0UL};
        castle_flags(piece_idx[idx], moves_from[idx]);
        print_move(moves_from[idx], moves_to[idx]);
        make_move(piece_idx[idx], moves_from[idx], moves_to[idx], captured);
        print_full_board();
        hashset_add(bitfields[is_player_white] | bitfields[!is_player_white]);
        // save move in struct
        // struct includes current hashtable

        
        //print results
        //switch sides
        count_total_moves++;
        gameover = game_finished(move_count);
        if(gameover){
            field match_duration = stop_stopwatch(total_time);
            printf("Game Over\n");
            print_full_board();
            printf("Match took %d moves took %luμs\n", count_total_moves, match_duration);
            return 0;
        }
        is_player_white = 1 - is_player_white;
    }
    return 0;
}

// test in main ? :(
void test_add_to_board_coords(){
    field board = (field) 0;

    printf("board:\n");
    print_board(board);
    printf("add to board at piece_index=3, y=2\n");
    add_to_board_coords(&board, 3, 4);
    print_board(board);
}

void test_bitfield_print(){
    field bitfield = (field) 0b1000101001100001111100011010001001011111001100011001000111111000;
    print_board(bitfield);
}
