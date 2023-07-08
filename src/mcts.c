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
#include <stdlib.h>

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
    int depth = 5;
    int move_count = 0;
    while(depth > 0){
        move_count = generate_moves(moves_from, moves_to, piece_idx);
        int rnd = rand() % move_count;
        make_move(piece_idx[rnd], moves_from[rnd], moves_to[rnd], captured);
        if(game_finished(move_count)) break;
        if(!depth) break;
        is_player_white = !is_player_white;
        depth--;
    }
    is_player_white = player;
    memcpy(castle_left, castle_flags_left, sizeof(castle_left));
    memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    return evaluation();
}

float mcts(node *parent, int depth){
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    float rating[max_move_count];
    field captured[8] = {0UL};
    bool castle_flags_left[2];
    bool castle_flags_right[2];
    int move_count = generate_moves(moves_from, moves_to, piece_idx);
    node *root = (node *) malloc(sizeof(node));
    for(int i = 0; i < 8; i++)
        root->board_state[i] = bitfields[i];
    root->n = 1;
    root->terminal = false;
    root->rating = rollout();
    if(game_finished(move_count)){
        root->terminal = true;
        return 0.0f;
    }
    for(int i = 0; i < move_count; i++){
        memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
        memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));
        make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        node *temp  = (node *) malloc(sizeof(node));
        for(int i = 0; i < 8; i++)
            temp->board_state[i] = bitfields[i];
        temp->n = 1;
        temp->terminal = false;
        root->children[i] = temp;
        root->children[i]->parent = root;
        root->children[i]->rating = rollout();
        unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        memcpy(castle_left, castle_flags_left, sizeof(castle_left));
        memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    }
    for(int i = 0; i < move_count; i++){
        memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
        memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));
        make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        mcts(root->children[i], depth - 1);
        unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        memcpy(castle_left, castle_flags_left, sizeof(castle_left));
        memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    }
    if(is_player_white){
        for(int i = 0; i < move_count; i++){
            if(root->children[i]->rating > root->rating)
                root->rating = root->children[i]->rating;
                root->pv = i;
        }
    }
    else{
        for(int i = 0; i < move_count; i++){
            if(root->children[i]->rating < root->rating)
                root->rating = root->children[i]->rating;
                root->pv = i;
        }
    }
    return root->rating;
}