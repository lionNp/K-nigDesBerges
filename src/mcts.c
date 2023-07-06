#include "move_util.h"
#include "bit_boards_util.h"
#include "common.h"
#include "evaluation.h"
#include "move_executer.h"
#include "move_generator.h"
#include <math.h>
#include "mcts.h"
#include "stopwatch_utils.h"
#include <string.h>
#include "hash_boards.h"

float rollout(){
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    float rating[max_move_count];
    bool player = is_player_white;
    field captured[8] = {0UL};
    bool castle_flags_left[2];
    bool castle_flags_right[2];
    memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
    memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));
    int counter = 5;
    while(){
        int move_count = generate_moves(moves_from, moves_to, piece_idx);
        int rnd = rand(move_count);
        make_move(piece_idx[rnd], moves_from[rnd], moves_to[rnd], captured);
        if(game_finished(move_count)) break;
        if(!counter) break;
        is_player_white = !is_player_white;
        counter--;
    }
    is_player_white = player;
    memcpy(castle_left, castle_flags_left, sizeof(castle_left));
    memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    return evaluation();
}


float ucb(){

}


float mcts(node parent){
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    float rating[max_move_count];
    field captured[8] = {0UL};
    bool castle_flags_left[2];
    bool castle_flags_right[2];
    int depth = 5;
    while(depth > 0){
        int move_count = generate_moves(moves_from, moves_to, piece_idx);
        node root = {.board_state = bitfields, .n = 1, .terminal = false};
        root.rating = rollout();
        if(game_finished(move_count))
            root.terminal = true;
        for(int i = 0; i < move_count; i++){
            memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
            memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));
            make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
            node temp = {.board_state = bitfields, .n = 1, .terminal = false};
            root->children[i] = temp;
            root->children[i].parent = root;
            root->children[i].rating = rollout();
            unmake_move(piece_idx[i], moves_from[i], moves_to[i], caputred);
            memcpy(castle_left, castle_flags_left, sizeof(castle_left));
            memcpy(castle_right, castle_flags_right, sizeof(castle_right));
        }
        depth--;
    }
    
   
}