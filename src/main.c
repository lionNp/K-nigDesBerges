#include <sys/time.h>

#include "main.h"
#include <math.h>
#include "bit_boards_util.h"
#include "move_util.h"

int main(){
    // initialize move_masks
    uint64_t knight_moves[64];
    uint64_t king_moves[64];
    init_knight_moves(knight_moves);
    init_king_moves(king_moves);

    //read situation string
    field bitfield_fig[figure_count];
    char feld_string[] = game_string;
    
    import_string(bitfield_fig, feld_string);

    //print_all_boards(bitfield_fig);
    print_board(bitfield_fig[black]);
    struct timeval stop, start;

    // measure performance starting here
    gettimeofday(&start, NULL);

    // all moves
    field legal_moves[32];
    int x = 0;
    int bit = 0;
    for(int piece=king; piece<=pawn; piece++){
        field pieces = bitfield_fig[black] & bitfield_fig[piece];
        int num_moves = num_pieces(pieces);
        field single_piece[num_moves];
        get_single_pieces(pieces, single_piece, num_moves);
        for(int y = 0; y < num_moves; y++){
            switch(piece){
                case pawn:
                    legal_moves[x] = find_legal_pawn_moves(bitfield_fig[black], bitfield_fig[white], single_piece[y], 0);
                    break;
                case rook:
                    legal_moves[x] = find_legal_rook_moves(bitfield_fig[black], bitfield_fig[white], single_piece[y]);
                    break;
                case bishop:
                    legal_moves[x] = find_legal_diag_moves(bitfield_fig[black], bitfield_fig[white], single_piece[y]);
                    break;
                case knight:
                    bit = log2(single_piece[y]);
                    legal_moves[x] = knight_moves[bit] ^ (knight_moves[bit] & bitfield_fig[black]);
                    break;
                case queen: ;
                    field legal_moves_queen_1 = find_legal_diag_moves(bitfield_fig[black], bitfield_fig[white], single_piece[y]);
                    field legal_moves_queen_2 = find_legal_rook_moves(bitfield_fig[black], bitfield_fig[white], single_piece[y]);
                    legal_moves[x] = legal_moves_queen_1 | legal_moves_queen_2;
                    break;
                case king:
                    bit = log2(single_piece[y]);
                    legal_moves[x] = king_moves[bit] ^ (king_moves[bit] & bitfield_fig[black]); 
                    break;
                default:
                    break;
                }
        }
        x++;
    }

    gettimeofday(&stop, NULL);
    printf("all moves took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    print_moves(legal_moves);
    //printf("\ncheck from\n");
    //print_board(in_check_from);
    return 0;
}

void test_add_to_board_coords(){
    field board = (field) 0;

    printf("board:\n");
    print_board(board);
    printf("add to board at x=3, y=2\n");
    add_to_board_coords(&board, 3, 4);
    print_board(board);
}

void test_bitfield_print(){
    field bitfield = (field) 0b1000101001100001111100011010001001011111001100011001000111111000;
    print_board(bitfield);
}