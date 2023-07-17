#include "common.h"
#include "zobrist_hash.h"
#include "bit_boards_util.h"
#include "move_executer.h"

void main ()
{
    init_zobrist();

    char fen[] = "rnbqkbnr/ppp1pppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    import_gamestring(bitfields, fen);

    uint64_t hash = calculate_hash_value();
    float score = 1.0f;

    put_score(hash, score);

    field captured[8] = {0UL};
    make_move(pawn, 2048ul, 524288ul, captured);
    //field new_hash = update_hash(hash, 2048ul, 524288ul, pawn);
    field new_hash = calculate_hash_value();
    print_move_2(hash);
    print_move_2(new_hash);
    //print_move_2(n_hash);
    float new_score = 1.5f;

    put_score(new_hash, new_score);

    printf("Hash: %lu\n", hash);
    printf("Score: %f\n", get_score(hash));

    printf("New Hash: %lu\n", new_hash);
    printf("New Score: %f\n", get_score(new_hash));
}