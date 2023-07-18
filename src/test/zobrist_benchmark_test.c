#include "common.h"
#include "zobrist_hash.h"
#include "bit_boards_util.h"
#include "move_executer.h"
#include "hash_boards.h"

void main ()
{
    init_zobrist();

    char fen[] = "rnbqkbnr/ppppp1pp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1";
    import_gamestring(bitfields, game_string);

    field captured[8] = {0UL};

    field hash = calculate_hash_value();
    field new_hash = 0ul;
    int iterations = 100000;

    stopwatch zobrist_timer = start_stopwatch();
    for(int i = 0; i < iterations; i++)
    {
        new_hash = update_hash(hash, 2048ul, 524288ul, pawn, captured);
        put_score(new_hash, 1.0f);
        new_hash = update_hash(hash, 524288ul, 2048ul, pawn, captured);
        put_score(new_hash, 1.0f);
    }
    field time = stop_stopwatch(zobrist_timer);

    printf("%d iterations with zobrist: %lu\n", iterations, time);

    stopwatch hash_timer = start_stopwatch();
    for(int i = 0; i < iterations; i++)
    {
        hash_boards();
        hash_boards();
    }
    field hash_time = stop_stopwatch(hash_timer);

    printf("%d iterations with hash_boards: %lu\n", iterations, hash_time);

    stopwatch hash2_timer = start_stopwatch();
    for(int i = 0; i < iterations; i++)
    {
        hash_boards2();
        hash_boards2();
    }
    field hash2_time = stop_stopwatch(hash2_timer);

    printf("%d iterations with hash_boards2: %lu\n", iterations, hash2_time);
}