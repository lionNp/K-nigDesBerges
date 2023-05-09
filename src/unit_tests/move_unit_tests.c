#include "assertions.c"
#include "move_generator.h"
#include "move_util.h"

void main()
{
    char fen[] = "8/8/8/8/8/8/4P3/8";
    import_gamesting(bitfields, fen);

    int counts[2] = {0, 0};  

    int piece_array[16];
    
    field legal_moves[16];
    field legal_moves_piece[16];

    generate_moves(legal_moves, legal_moves_piece, piece_array, counts);

}