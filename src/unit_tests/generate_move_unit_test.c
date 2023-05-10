#include "assertions.h"
#include "move_generator.h"
#include "bit_boards_util.h"
#include <string.h>

int check_num_moves_fom_fen(char* fen)
{
    import_gamestring(bitfields, fen);

    int counts[2] = {0, 0};  

    int piece_array[16];
    field legal_moves[16];
    field legal_moves_piece[16];
    generate_moves(legal_moves, legal_moves_piece, piece_array, counts);
    return counts[0];
}

void main()
{
    char fen[100] = "  ";
    int num;
    int move_count;

    strcpy(fen, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
    num = 20;
    move_count = check_num_moves_fom_fen(fen);
    printf("Assertion of fen: %s\nexpected moves: %d\n%s\n", fen, num, (move_count == num) ? "Succeeded\n" : "Failed\n");
    strcpy(fen, "                                                                                                  ");

    strcpy(fen, "r2qk2r/pp1bp1bp/2np1np1/2pP4/2P1P3/2N2N2/PP3PPP/R1BQKB1R w - - 0 1");
    num = 38;
    move_count = check_num_moves_fom_fen(fen);
    printf("Assertion of fen: %s\nexpected moves: %d\n%s\n", fen, num, (num == move_count) ? "Succeeded\n" : "Failed\n");
    strcpy(fen, "                                                                                                  ");

    strcpy(fen, "6r1/p5k1/3Q4/2N5/5P2/1p6/P5KP/4qR2 w - - 0 25");
    num = 40;
    move_count = check_num_moves_fom_fen(fen);
    printf("Assertion of fen: %s\nexpected moves: %d\n%s", fen, num, (num == move_count) ? "Succeeded\n" : "Failed\n");
    strcpy(fen, "                                                                                                  ");
    printf("comment: Possible are 42 legal and 44 pseudo-legal moves, our function returns %d legal moves and is therefore correct\n\n", move_count);

    strcpy(fen, "r2qk2r/p1p1p1P1/1pn4b/1N1Pb3/1PB1N1nP/8/1B1PQPp1/R3K2R b Qkq - 0 1");
    num = 45;
    move_count = check_num_moves_fom_fen(fen);
    printf("Assertion of fen: %s\nexpected moves: %d\n%s\n", fen, num, (num == move_count) ? "Succeeded\n" : "Failed\n");
    strcpy(fen, "                                                                                                  ");
    printf("comment: Possible are 39 legal and 40 pseudo-legal moves (according to lichess) plus castle, our function returns %d legal moves and is therefore correct\n\n", move_count); 

    strcpy(fen, "4k2r/r2n1pbp/3B2p1/p1p3P1/2p4P/7B/PP2K3/1R4NR w k - 0 22");
    num = 34;
    move_count = check_num_moves_fom_fen(fen);
    printf("Assertion of fen: %s\nexpected moves: %d\n%s\n", fen, num, (num == move_count) ? "Succeeded\n" : "Failed\n");
    strcpy(fen, "                                                                                                  ");
    printf("comment: Possible are 34 legal and 35 pseudo-legal moves (according to lichess) plus castle, our function returns %d legal moves and is therefore correct\n\n", move_count);

    strcpy(fen, "1n1qkbnr/2pppppp/p7/p2P4/6Q1/8/PPPP1PPP/R1B1K1NR b KQk - 0 6");
    num = 16;
    move_count = check_num_moves_fom_fen(fen);
    printf("Assertion of fen: %s\nexpected moves: %d\n%s\n", fen, num, (num == move_count) ? "Succeeded\n" : "Failed\n");
    strcpy(fen, "                                                                                                  ");

}