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
    //assert(counts[0] == 20);

    //repeat from here
    import_gamestring(bitfields, "6k1/2P4p/r1pPb2r/p2qppN1/p1B2P1P/1NbP1Q2/R2n1K2/2B4R w - - 0 1");
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
    //assert(counts[0] == 43);

    //repeat from here
    import_gamestring(bitfields, "4k2r/r2n1pbp/3B2p1/p1p3P1/2p4P/7B/PP2K3/1R4NR w k");
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
    //assert(counts[0] == 31);

    //repeat from here
    import_gamestring(bitfields, "r3k2r/1ppb1ppp/p1q2n2/1n1pp3/PbB1P1N1/1PP1QN1P/R2P1PP1/2B1K2R w Kkq");
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
    //assert(counts[0] == 45);

    //repeat from here
    import_gamestring(bitfields, "r3k2r/1ppb1ppp/p1q2n2/1n1pp3/PbB1P1N1/1PP1QN1P/R2P1PP1/2B1K2R b Kkq");
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
    //assert(counts[0] == 45);

    
    //repeat from here
    import_gamestring(bitfields, "r3k2r/1pp2p1p/5np1/3PQ3/BPq3P1/3P4/3N1PP1/5RK1 b --");
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
    //assert(counts[0] == 42);

    //repeat from here
    import_gamestring(bitfields, "6r1/p5k1/3Q4/2N5/5P2/1p6/P5KP/4qR2 w --");
    time = start_stopwatch();
    t = 0UL;
    for(int i = 0; i < 1; i++){
        counts[0] = 0;
        counts[1] = 0;
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    printf("%d legal moves took an avg of %lu μs\n \n", counts[0], t);
    //assert(counts[0] == 42);
}



int main(){
    test_move_generator();
    return 0;
}
