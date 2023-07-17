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

    

    bool player = is_player_white;

    bool castle_flags_left[2];
    bool castle_flags_right[2];
    memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
    memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));

    int depth = 3;
    int move_count;
    int i = 0;
    for(; i < depth; i++){
        field moves_from[max_move_count];
        field moves_to[max_move_count];
        int piece_idx[max_move_count];
        field captured[8] = {0UL};
        move_count = generate_moves(moves_from, moves_to, piece_idx);
        if(game_finished(move_count)) break;
        int rnd = rand() % move_count;
        make_move(piece_idx[rnd], moves_from[rnd], moves_to[rnd], captured);
        is_player_white = !is_player_white;
    }
    float eval = evaluation();
    is_player_white = player;
    memcpy(castle_left, castle_flags_left, sizeof(castle_left));
    memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    memcpy(bitfields, boards, sizeof(boards));
    return eval;
}

float mcts(node *parent, int depth){
    if(depth == 0)
        return rollout();

    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int piece_idx[max_move_count];
    int move_count = generate_moves(moves_from, moves_to, piece_idx);

    field moves_from_2[max_move_count];
    field moves_to_2[max_move_count];
    int piece_idx_2[max_move_count];
    field captured[8] = {0UL};

    bool castle_flags_left[2];
    bool castle_flags_right[2];
    

    int i = 0;
    for(; i < move_count; i++){
        memcpy(castle_flags_left, castle_left, sizeof(castle_flags_left));
        memcpy(castle_flags_right, castle_right, sizeof(castle_flags_right));
        make_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        int move_count_2 = generate_moves(moves_from_2, moves_to_2, piece_idx_2);
        parent->children[i].children = (node *) malloc(sizeof(node) * move_count_2);
        parent->children[i].from = moves_from[i];
        parent->children[i].to = moves_to[i];
        parent->children[i].idx = piece_idx[i];
        is_player_white = !is_player_white;
        parent->children[i].rating = mcts(&parent->children[i], depth - 1);
        is_player_white = !is_player_white;
        if(i == 0){
            parent->next = &parent->children[i];
        }
        else{
            if(is_player_white){
                if(parent->next->rating < parent->children[i].rating){
                    parent->next = &parent->children[i];
                    parent->pv = i;
                }
            }
            else{
                if(parent->next->rating > parent->children[i].rating){
                    parent->next = &parent->children[i];
                    parent->pv = i;
                }
            }
        }
        unmake_move(piece_idx[i], moves_from[i], moves_to[i], captured);
        memcpy(castle_left, castle_flags_left, sizeof(castle_left));
        memcpy(castle_right, castle_flags_right, sizeof(castle_right));
    }
    return parent->next->rating;
}