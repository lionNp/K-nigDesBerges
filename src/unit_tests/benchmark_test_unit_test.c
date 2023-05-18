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
    int piece_idx[max_move_count];
    field moves_from[max_move_count];
    field moves_to[max_move_count];
    int move_count = 0;
    //repeat from here
    import_gamestring(bitfields, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
    stopwatch time = start_stopwatch();
    field t = 0UL;
    for(int i = 0; i < 1000; i++){
        move_count = generate_moves(moves_from, moves_to, piece_idx);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    printf("%d legal moves took an avg of %lu μs\n \n", move_count, t);
    assert(move_count == 20);

    //repeat from here
    import_gamestring(bitfields, "6k1/2P4p/r1pPb2r/p2qppN1/p1B2P1P/1NbP1Q2/R2n1K2/2B4R w -");
    time = start_stopwatch();
    t = 0UL;
    for(int i = 0; i < 1000; i++){
        move_count = generate_moves(moves_from, moves_to, piece_idx);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    for(int i = 0; i < 1000; i++){
        move_count = generate_moves(moves_from, moves_to, piece_idx);
    }
        
    printf("%d legal moves took an avg of %lu μs\n \n", move_count, t);
    assert(move_count == 49);

    //repeat from here
    import_gamestring(bitfields, "8/5p2/2PP2k1/2p1P3/5K1P/1R2n3/p7/8 w -");
    time = start_stopwatch();
    t = 0UL;
    for(int i = 0; i < 1000; i++){
        move_count = generate_moves(moves_from, moves_to, piece_idx);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    printf("%d legal moves took an avg of %lu μs\n \n", move_count, t);
    assert(move_count == 19);
}

int main(){
    test_move_generator();
    return 0;
}
