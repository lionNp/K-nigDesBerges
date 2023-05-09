#include "common.h"
#include "assertions.h"
#include "bit_boards_util.h"
#include "move_generator.h"
#include "stopwatch_utils.h"
#include <stdbool.h>

void test_move_generator()
{
    import_gamestring(bitfields, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
    int move_count = 0;
    int counts[2] = {0, 0};  
    int piece_array[16];
    field legal_moves[16];
    field legal_moves_piece[16];
    stopwatch time = start_stopwatch();
    field t = 0UL;
    for(int i = 0; i < 1000; i++){
        counts[0] = 0;
        counts[1] = 0;
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
        t = stop_stopwatch(time);
    }
    t /= 1000;
    printf("%d pseudo legal moves took an avg of %lu us\n", counts[0], t);
}

int main()
{
    test_move_generator();
    return 0;
}