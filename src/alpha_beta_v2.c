#include "stopwatch_utils.h"
#include "evaluation.h"
#include "move_executer.h"
#include "move_generator.h"
#include "alpha_beta_v2.h"
#include "bit_boards_util.h"

#include <stdlib.h>

float max(float a, float b)
{
    return a > b ? a : b;
}

float min(float a, float b)
{
    return a < b ? a : b;
}

void copy_node(ab_node* target, ab_node* source)
{
    target->piece_type = source->piece_type;
    target->piece_from = source->piece_from;
    target->piece_to = source->piece_to;
    target->rating = source->rating;
}

void alpha_beta_search(ab_node* node, int depth, float alpha, float beta, bool is_maxing)
{
    if(depth == 0)
    {
        node->rating = evaluation(node->piece_from, node->piece_to, node->piece_type);

        printf("Evaluation from: \n");
        print_board(node->piece_from);
        printf("Evaluation to: \n");
        print_board(node->piece_to);
        printf("Evaluation rating: %f\n", node->rating);

        return;
    }

    is_player_white = is_maxing;

    //generate child nodes
    field from[max_move_count];
    field to[max_move_count];
    int piece_types[max_move_count];
    int moves = generate_moves(from, to, piece_types);

    if(is_maxing)
    {
        float maxValue = INT_MIN;
        for(int i = 0; i < moves; i++)
        {
            is_player_white = is_maxing;

            field captured[8];
            make_move(piece_types[i], from[i], to[i], captured);

            ab_node* move_node = malloc(sizeof(ab_node));
            move_node->piece_type = piece_types[i];
            move_node->piece_from = from[i];
            move_node->piece_to = to[i];

            alpha_beta_search(move_node, depth - 1, alpha, beta, false);

            is_player_white = is_maxing;

            unmake_move(piece_types[i], from[i], to[i], captured);

            if(move_node->rating > maxValue)
            {
                maxValue = move_node->rating;
                copy_node(node, move_node);
            }
            free(move_node);

            if(maxValue > beta)
                break;

            alpha = max(alpha, maxValue);
        }

        return;
    }
    else
    {
        float blackMaxValue = INT_MIN;
        for(int i = 0; i < moves; i++)
        {
            is_player_white = is_maxing;

            field captured[8];
            make_move(piece_types[i], from[i], to[i], captured);

            ab_node* move_node = malloc(sizeof(ab_node));
            move_node->piece_type = piece_types[i];
            move_node->piece_from = from[i];
            move_node->piece_to = to[i];

            alpha_beta_search(move_node, depth - 1, alpha, beta, true);

            is_player_white = is_maxing;

            unmake_move(piece_types[i], from[i], to[i], captured);

            if(move_node->rating > blackMaxValue)
            {
                blackMaxValue = move_node->rating;
                copy_node(node, move_node);
            }
            free(move_node);

            if(-blackMaxValue < alpha)
                break;

            beta = min(beta, -blackMaxValue);
        }

        return;
    }
}