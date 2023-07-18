#include "hashset.h"
#include "common.h"
#include "bit_boards_util.h"
#include "move_executer.h"
#include "move_generator.h"

void main()
{
    char fen[] = "r1bq1b1r/pp1p2pp/n1p1kp1n/4p3/1PB1P3/BRN5/P1PP1PPP/3QK1NR w Kha - 0 1";
    import_gamestring(bitfields, game_string);

    field from[max_move_count];
    field to[max_move_count];
    int piece_type[max_move_count];

    int moves = generate_moves(from, to, piece_type);

    for(int counter = 0; counter < 9; counter++)
    {
        for(int i = 0; i < moves; i++)
        {
            field captured[8] = {0ul};
            make_move(piece_type[i], from[i], to[i], captured);

            hashset_add();

            unmake_move(piece_type[i], from[i], to[i], captured);
        }

        if(counter % 3 == 0)
            hashset_clear();
    }

    printf("max duplicates: %d\n", hashset_duplicates());
    print_full_board();
}