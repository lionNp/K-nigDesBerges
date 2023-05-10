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
    printf("%d pseudo legal moves took an avg of %lu us\n \n", counts[0], t);
    assert(counts[0] == 20);

    //repeat from here
    import_gamestring(bitfields, "r1bqk1nr/ppp2ppp/2n5/3pp3/1bB1P3/5N2/PPPP1PPP/RNBQK2R b");
    time = start_stopwatch();
    t = 0UL;
    for(int i = 0; i < 1000; i++){
        counts[0] = 0;
        counts[1] = 0;
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    printf("%d pseudo legal moves took an avg of %lu us\n \n", counts[0], t);
    assert(counts[0] == 43);

    //repeat from here
    import_gamestring(bitfields, "r1bqk1nr/ppp2ppp/2n5/3pp3/1bB1P3/5N2/PPPP1PPP/RNBQK2R w KQkq");
    time = start_stopwatch();
    t = 0UL;
    for(int i = 0; i < 1000; i++){
        counts[0] = 0;
        counts[1] = 0;
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
    }
    t = stop_stopwatch(time);
    t /= 1000;
    printf("%d pseudo legal moves took an avg of %lu us\n \n", counts[0], t);
    assert(counts[0] == 31);

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
    printf("%d pseudo legal moves took an avg of %lu us\n \n", counts[0], t);
    assert(counts[0] == 45);

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
    printf("%d pseudo legal moves took an avg of %lu us\n \n", counts[0], t);
    assert(counts[0] == 45);

    
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
    printf("%d pseudo legal moves took an avg of %lu us\n \n", counts[0], t);
    assert(counts[0] == 42);
}

void test_move_generator_legal()
{
    int counts[2] = {0, 0};  
    int piece_array[16];
    field legal_moves[16];
    field legal_moves_piece[16];
    
    //repeat from here
    import_gamestring(bitfields, "8/8/4r3/b7/1P5b/4P3/r4Q2/rRN1K3 w");
    field attacked_squares[16];
    for(int i = 0; i < 16; i++)
        attacked_squares[i] = 0UL;
    generate_attacked_squares(attacked_squares);
    field danger = 0UL;
    for(int i = 0; i < 16; i++)
        danger |= attacked_squares[i];
    field king_position = bitfields[is_player_white] & bitfields[king];
    field king_pinned = pinned_piece_check(king_position);
    print_move_board(king_pinned);
    print_move_board(danger);
    stopwatch time = start_stopwatch();
    field t = 0UL;
    for(int i = 0; i < 1000; i++){
        counts[0] = 0;
        counts[1] = 0;
        generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
        legal_moves[0] ^= legal_moves[0] & danger;
        //for(int k = 0; k < 16; k++){
        //    if(legal_moves_piece[k] & king_pinned)
        //}
    }
    t = stop_stopwatch(time);
    t /= 1000;
    print_move_board(legal_moves[0]);
    printf("%d pseudo legal moves took an avg of %lu us\n \n", counts[0], t);
    assert(counts[0] == 20);
}

int main(){
    //test_move_generator();
    test_move_generator_legal();
    return 0;
}