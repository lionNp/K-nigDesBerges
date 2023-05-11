#include "common.h"
#include "assertions.h"
#include "bit_boards_util.h"
#include "move_generator.h"
#include "move_util.h"
#include "stopwatch_utils.h"
#include <stdbool.h>
#include <assert.h>

void test_move_generator()
{
    int counts[2] = {0, 0};  
    int piece_array[16];
    field legal_moves[16];
    field legal_moves_piece[16];

    //repeat from here
    import_gamestring(bitfields, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
    stopwatch time = start_stopwatch();
    field t = 0UL;
    for(int i = 0; i < 1000; i++){
        counts[0] = 0;
        counts[1] = 0;
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    printf("%d legal moves took an avg of %lu μs\n \n", counts[0], t);
    assert(counts[0] == 20);

    //repeat from here
    import_gamestring(bitfields, "6k1/2P4p/r1pPb2r/p2qppN1/p1B2P1P/1NbP1Q2/R2n1K2/2B4R w -");
    time = start_stopwatch();
    t = 0UL;
    for(int i = 0; i < 1000; i++){
        counts[0] = 0;
        counts[1] = 0;
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    printf("%d legal moves took an avg of %lu μs\n \n", counts[0], t);
    assert(counts[0] == 49);

    //repeat from here
    import_gamestring(bitfields, "8/5p2/2PP2k1/2p1P3/5K1P/1R2n3/p7/8 w -");
    time = start_stopwatch();
    t = 0UL;
    for(int i = 0; i < 1000; i++){
        counts[0] = 0;
        counts[1] = 0;
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    printf("%d legal moves took an avg of %lu μs\n \n", counts[0], t);
    assert(counts[0] == 19);
}

int main(){
    test_move_generator();
    return 0;
}
