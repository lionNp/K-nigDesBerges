#include "move_generator.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include <math.h>

void generate_moves(field legal_moves[], field legal_moves_piece[], int piece_array[], int counts[]){
    int bit = 0;
    int x = 0;
    for(int current_piece = king; current_piece<=pawn; current_piece++)
    {
        field player_piece_board = bitfields[is_player_white] & bitfields[current_piece];
        int num_moves = get_piece_count(player_piece_board);

        field single_piece_boards[num_moves];
        get_single_piece_boards(player_piece_board, single_piece_boards, num_moves);

        for(int y = 0; y < num_moves; y++)
        {
            counts[1]++;
            piece_array[x] = current_piece;
            legal_moves_piece[x] = single_piece_boards[y];

            switch(current_piece)
            {
                case pawn:
                    legal_moves[x] = find_legal_pawn_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;

                case rook:
                    legal_moves[x] = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;

                case bishop:
                    legal_moves[x] = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;

                case knight:
                    bit = log2(single_piece_boards[y]);
                    legal_moves[x] = knight_moves[bit] ^ (knight_moves[bit] & bitfields[is_player_white]);
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;

                case queen: ;
                    field legal_moves_queen_1 = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                    field legal_moves_queen_2 = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[y]);
                    legal_moves[x] = legal_moves_queen_1 | legal_moves_queen_2;
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;
                    
                case king:
                    bit = log2(single_piece_boards[y]);
                    legal_moves[x] = king_moves[bit] ^ (king_moves[bit] & bitfields[is_player_white]); 
                    counts[0] += get_piece_count(legal_moves[x]);
            }
            x++;
        }
    }
}
    