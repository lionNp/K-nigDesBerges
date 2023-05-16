#include "common.h"
#include "stopwatch_utils.h"
#include "alpha_beta_v2.h"
#include "bit_boards_util.h"
#include "move_executer.h"

#include <stdlib.h>

void main()
{
    char fen[] = "8/8/8/2pp4/4P3/8/8/7R w - - 0 1";
    import_gamestring(bitfields, fen);

    ab_node* next_best_move = malloc(sizeof(ab_node));

    printf("Game State:\n");
    print_full_board();

    alpha_beta_search(next_best_move, 1, INT_MIN, INT_MIN, true);
    
    printf("Best move with piece %s:\n", get_piece_name(next_best_move->piece_type));
    print_board(next_best_move->piece_from);

    printf("Move target:\n");
    print_board(next_best_move->piece_to);

    printf("With Rating: %f\n", next_best_move->rating);

    printf("Performing Move\n");
    field captured[8];
    make_move(next_best_move->piece_type, next_best_move->piece_from, next_best_move->piece_to, captured);

    printf("Game State:\n");
    print_full_board();

    free(next_best_move);
}