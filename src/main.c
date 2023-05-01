#include <sys/time.h>

#include "main.h"
#include <math.h>
#include "common.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include "evaluation.h"

int main(){
    // initialize move_masks
    uint64_t knight_moves[64];
    uint64_t king_moves[64];
    uint64_t rook_moves[64];
    uint64_t queen_moves[64];
    uint64_t bishop_moves[64];
    init_knight_moves(knight_moves);
    init_king_moves(king_moves);
    init_bishop_moves(bishop_moves);
    init_queen_moves(queen_moves);
    init_rook_moves(rook_moves);
    

    //read situation string
    field bitfield_fig[figure_count];
    char feld_string[] = game_string;
    
    import_string(bitfield_fig, feld_string);

    //print_all_boards(bitfield_fig);
    print_board(bitfield_fig[black]);
    struct timeval stop, start;

    // measure performance starting here
    gettimeofday(&start, NULL);

    //switch sides here
    turn -= turn;
    // all moves
    field legal_moves[16];
    field legal_moves_piece[16];
    int piece_array[16];
    char wpiece[16] = "";
    int x = 0;
    int bit = 0;
    int piece_count = 0;
    int move_count = 0;
    for(int piece=king; piece<=pawn; piece++){
        field pieces = bitfield_fig[black] & bitfield_fig[piece];
        int num_moves = num_pieces(pieces);
        field single_piece[num_moves];
        get_single_pieces(pieces, single_piece, num_moves);
        for(int y = 0; y < num_moves; y++){
            piece_count++;
            piece_array[x] = piece;
            legal_moves_piece[x] = single_piece[y];
            switch(piece){
                case pawn:
                    wpiece[x] = 'p';
                    legal_moves[x] = find_legal_pawn_moves(bitfield_fig[black], bitfield_fig[white], single_piece[y], 0);
                    move_count += num_pieces(legal_moves[x]);
                    break;
                case rook:
                    wpiece[x] = 'r';
                    legal_moves[x] = find_legal_rook_moves(bitfield_fig[black], bitfield_fig[white], single_piece[y]);
                    move_count += num_pieces(legal_moves[x]);
                    break;
                case bishop:
                    wpiece[x] = 'b';
                    legal_moves[x] = find_legal_diag_moves(bitfield_fig[black], bitfield_fig[white], single_piece[y]);
                    move_count += num_pieces(legal_moves[x]);
                    break;
                case knight:
                    wpiece[x] = 'n';
                    bit = log2(single_piece[y]);
                    legal_moves[x] = knight_moves[bit] ^ (knight_moves[bit] & bitfield_fig[black]);
                    move_count += num_pieces(legal_moves[x]);
                    break;
                case queen:
                    wpiece[x] = 'q';
                    field legal_moves_queen_1 = find_legal_diag_moves(bitfield_fig[turn], bitfield_fig[!turn], single_piece[y]);
                    field legal_moves_queen_2 = find_legal_rook_moves(bitfield_fig[turn], bitfield_fig[!turn], single_piece[y]);
                    legal_moves[x] = legal_moves_queen_1 | legal_moves_queen_2;
                    move_count += num_pieces(legal_moves[x]);
                    break;
                case king:
                    wpiece[x] = 'k';
                    bit = log2(single_piece[y]);
                    legal_moves[x] = king_moves[bit] ^ (king_moves[bit] & bitfield_fig[turn]); 
                    move_count += num_pieces(legal_moves[x]);
            }
        x++;
        }
    }
    field moves[piece_count*move_count];
    int rating[move_count];
    int count = 0;
    for(int i = 0; i < piece_count; i++){
        int num_moves = num_pieces(legal_moves[i]);
        field single_move[num_moves];
        get_single_pieces(legal_moves[i], single_move, num_moves);
        for(int k = 0; k < num_moves; k++){
            rating[count] = evaluation(bitfield_fig, single_move[k], legal_moves_piece[i], piece_array[i]);
            moves[count] = legal_moves_piece[i];
            count++;
            moves[count] = single_move[k];
            count++;
        }
    }
    gettimeofday(&stop, NULL);
    printf("all moves took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    for(int i = 0; i < piece_count*move_count; i++){
        printf("%d ", rating[i]);
    }
    printf("\n");


    /*
    for move:
        make move
        evaluate
        unmake move
    */

    //for(int i = 0; i < count; i++){
    //    print_board(moves[i]);
    //}
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
