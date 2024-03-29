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
#include "mcts.h"
#include <stdlib.h>

int main() {
    //initilize board
    import_gamestring(bitfields, game_string);

    //set chess clock;
    total_remaining_time = default_expected_move_num * default_time_per_move;

    int count_total_moves = 0;
    // total match duration
    stopwatch total_time = start_stopwatch();
    field match_duration;
    int total_pieces = 32;

    printf("start mcts\n");
    int mcts_depth = 4;
    node *root = (node *) malloc(sizeof(node));
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    int move_count = generate_moves(moves_from, moves_to, piece_idx);
    
    root->children = (node *) malloc(sizeof(node) * move_count);
    root->rating = mcts(root, mcts_depth);

    //for(int i = 0; i < 20; i++)
    //    printf("%f\n", root->children[i].rating);
    print_move_2(root->children[root->pv].from ^ root->children[root->pv].to);
    print_move_2(root->children[root->pv].next->from ^ root->children[root->pv].next->to);
    print_move_2(root->children[root->pv].next->next->from ^ root->children[root->pv].next->next->to);
    print_move_2(root->children[root->pv].next->next->next->from ^ root->children[root->pv].next->next->next->to);
    printf("%f\n", root->rating);
    printf("%d\n", root->pv);
    printf("end mcts\n");
    
    //printf("%f\n", root->rating);

    while(!gameover)
    //for(int r = 0; r < 1; r++)
    {
        //time
        stopwatch turn_time = start_stopwatch();
        field t = 0UL;
        //initilizing moves
        
        //alpha beta initilizing
        float alpha = losing_move;
        float beta = winning_move;
        bool max_player = is_player_white;
        //get moves and set rating
        move_count = generate_moves(moves_from, moves_to, piece_idx);
        
        bool castle_flags_left[2];
        bool castle_flags_right[2];

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

        int break_after = default_time_per_move;
        
        int depth = 0;
        num_moves_iterated = 0;
        num_moves_trans = 0;

        float pv_score = 0.0f;
        float score;

        //set time for each iteration dynamically
        break_after = calc_time_budget(move_count);
        //break_after = 120000000;
        //break_after = 1200000;
        for(; t < break_after; depth++){ 
            for(int i = 0; i < move_count; i++){

                //"almost done: if iteration is 70% done with depth, let it finish"
                if(stop_stopwatch(turn_time) > break_after && ((float) i / (float) move_count < 0.7f) )
                    break;

                //if(stop_stopwatch(turn_time) > break_after && ((float) i / (float) move_count >= 0.7f))
                //    printf("depth almost done, letting finish\n");

                field captured[8] = {0UL};

                memcpy(castle_flags_left,castle_left,sizeof(castle_flags_left));
                memcpy(castle_flags_right,castle_right,sizeof(castle_flags_right));

                make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
                is_player_white = !is_player_white;
                rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta(depth, alpha, beta, max_player);
                /*
                if(depth < 3)
                    rating[i] = (1 + (depth % 2) * tempo_bonus) * alphabeta(depth, alpha, beta, max_player);
                else
                    rating[i] = (1 + (depth % 2) * tempo_bonus) * pvs(depth, alpha, beta, max_player);
                */
                is_player_white = !is_player_white;
                unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);

                memcpy(castle_left,castle_flags_left,sizeof(castle_left));
                memcpy(castle_right,castle_flags_right,sizeof(castle_right));
            }
            
            for(int i = 0; i < move_count; i++)
                final_rating[i] = rating[i];

            //sort the moves depending on rating
            sort_moves(final_rating, moves_from, moves_to, piece_idx, move_count);
            /*
            if(depth > 2){
                int null_move_pruning = 1;
                printf("move count Pre: %d\n", move_count);
                if(is_player_white){
                    for(int i = 0; i < move_count; i++){
                        if(final_rating[0] * 0.2 < final_rating[i]){
                            null_move_pruning += i;
                            break;
                        }
                    }
                }
                else{
                    for(int i = 0; i < move_count; i++){
                        if(final_rating[0] * 0.2 > final_rating[i]){
                            null_move_pruning += i;
                            break;
                        }
                    }
                }
                move_count = null_move_pruning;
                printf("move count Post: %d\n", move_count);
            }
            */
            //filter_moves_quietsearch();
            t = stop_stopwatch(turn_time);
        }

        //dekrement chess clock and increment total moves done
        total_remaining_time -= t;
        total_moves++;

        float round_time = ((float) t / 1000000);
        printf("Time: %fs       --- depth: %d\n", round_time, depth);
        printf("Total Moves: %d --- Evaluated Moves: %d\n", num_moves_iterated, num_moves_trans);
        printf("Collosions: %d\n", num_hash_collisions);
        printf("Collisionchance: %f\n", (float) num_hash_collisions/num_moves_iterated);
        //printf("Hash collisions: %d\n", num_hash_collisions);
        num_hash_collisions = 0;

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
        
        // ###################### added reset hash on captured
        for (int i = 0; i < 8; i++){
            if(captured[i]){
                memset(hash_table, 0, hash_prime * sizeof(float_t));
                break;
            }
        }
        // ######################
        print_full_board();
        hashset_add();
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
