#include "common.h"
#include "zobrist_hash.h"
#include "bit_boards_util.h"
#include "move_executer.h"
#include "move_generator.h"
#include "evaluation.h"

void main()
{
    init_zobrist();

    char fen[] = "r1bq1b1r/pp1p2pp/n1p1kp1n/4p3/1PB1P3/BRN5/P1PP1PPP/3QK1NR w Kha - 0 1";
    import_gamestring(bitfields, fen);

    field from[max_move_count];
    field to[max_move_count];
    int piece_type[max_move_count];

    int moves = generate_moves(from, to, piece_type);

    for(int i = 0; i < moves; i++)
    {
        field captured[8] = {0ul};
        make_move(piece_type[i], from[i], to[i], captured);

        field hash = calculate_hash_value();
        put_score(hash, evaluation());

        unmake_move(piece_type[i], from[i], to[i], captured);
    }

    printf("After all %d moves a total of %d collisions occured !\n", moves, num_hash_collisions);
}