#include "move_generator.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include <math.h>

//legal_moves[i] are all moves for piece "legal_moves_piece[i]". piece_array contains all player pieces. counts[0] is the total move count, count[1] is the total piece count for player
void generate_moves(field legal_moves[], field legal_moves_piece[], int piece_array[], int counts[])
{
    int bit_pos = 0;
    int x = 0;

    for(int current_piece = king; current_piece <= pawn; current_piece++)
    {
        field player_piece_board = bitfields[is_player_white] & bitfields[current_piece];
        int piece_count = get_piece_count(player_piece_board);

        field single_piece_boards[piece_count];
        get_single_piece_boards(player_piece_board, single_piece_boards, piece_count);

        for(int i = 0; i < piece_count; i++)
        {
            counts[1]++;
            piece_array[x] = current_piece;
            legal_moves_piece[x] = single_piece_boards[i];

            switch(current_piece)
            {
                case pawn:
                    legal_moves[x] = find_legal_pawn_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;

                case rook:
                    legal_moves[x] = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;

                case bishop:
                    legal_moves[x] = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;

                case knight:
                    bit_pos = log2(single_piece_boards[i]);
                    legal_moves[x] = knight_moves[bit_pos] ^ (knight_moves[bit_pos] & bitfields[is_player_white]);
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;

                case queen: ;
                    field legal_moves_queen_1 = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                    field legal_moves_queen_2 = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                    legal_moves[x] = legal_moves_queen_1 | legal_moves_queen_2;
                    counts[0] += get_piece_count(legal_moves[x]);
                    break;
                    
                case king:
                    bit_pos = log2(single_piece_boards[i]);
                    legal_moves[x] = king_moves[bit_pos] ^ (king_moves[bit_pos] & bitfields[is_player_white]); 
                    counts[0] += get_piece_count(legal_moves[x]);
            }
            x++;
        }
    }
}
    