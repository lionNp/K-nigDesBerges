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
    field boards[8];
    memcpy(boards, bitfields, sizeof(boards));

    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    field captured[8] = {0UL};

    bool player = is_player_white;

    bool castle_flags_left[2];
    bool castle_flags_right[2];
    memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
    memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));

    int depth = 5;
    int move_count;

    while(depth > 0){
        if(!depth) break;
        move_count = generate_moves(moves_from, moves_to, piece_idx);
        if(game_finished(move_count)) break;
        int rnd = rand() % move_count;
        make_move(piece_idx[rnd], moves_from[rnd], moves_to[rnd], captured);
        is_player_white = !is_player_white;
        depth--;
    }
    float eval = evaluation();
    is_player_white = player;
    memcpy(castle_left, castle_flags_left, sizeof(castle_left));
    memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    memcpy(bitfields, boards, sizeof(boards));
    return eval;
}

float mcts(node *parent, int depth){
    if(depth == 0) rollout();
    
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    field captured[8] = {0UL};

    bool castle_flags_left[2];
    bool castle_flags_right[2];
     
    int move_count = generate_moves(moves_from, moves_to, piece_idx);

    for(int i = 0; i < 8; i++)
        parent->board_state[i] = bitfields[i];
    parent->n = 1;
    parent->terminal = false;

    if(game_finished(move_count)){
        parent->terminal = true;
        return evaluation();
    }

    parent->children = (node *) malloc(sizeof(node) * move_count);

    for(int i = 0; i < move_count; i++){
        memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
        memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));
        make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        
        for(int i = 0; i < 8; i++)
            parent->children[i].board_state[i] = bitfields[i];
        parent->children[i].n = 1;
        parent->children[i].terminal = false;
        parent->children[i].parent = parent;
        parent->children[i].rating = rollout();
        
        unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        memcpy(castle_left, castle_flags_left, sizeof(castle_left));
        memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    }

    for(int i = 0; i < move_count; i++){
        memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
        memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));
        make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        parent->children[i].rating = mcts(&parent->children[i], depth - 1);
        unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        memcpy(castle_left, castle_flags_left, sizeof(castle_left));
        memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    }

    if(is_player_white){
        for(int i = 0; i < move_count; i++){
            if(parent->children[i].rating > parent->rating)
                parent->rating = parent->children[i].rating;
                parent->pv = i;
        }
    }
    else{
        for(int i = 0; i < move_count; i++){
            if(parent->children[i].rating < parent->rating)
                parent->rating = parent->children[i].rating;
                parent->pv = i;
        }
    }
    return parent->rating;
}