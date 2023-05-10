#include "move_generator.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include <math.h>

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
    field rooks = 0UL;
    //castle check
    if(is_player_white){
        if(castle_left[is_player_white]){
            rooks = bitfields[is_player_white] & castle_white_left_check;
            if(rooks == castle_white_left){
                // add castle left move
                legal_moves[0] |= 0x0000000000000020ull;
                counts[0]++;
            }
        }
        if(castle_right[is_player_white]){
            rooks = bitfields[is_player_white] & castle_white_right_check;
            if(rooks == castle_white_right){
                // add castle right move
                legal_moves[0] |= 0x0000000000000002ull;
                counts[0]++;
            }
        }
    }
    else{
        if(castle_left[is_player_white]){
            rooks = bitfields[is_player_white] & castle_black_left_check;
            if(rooks == castle_black_left){
                // add castle left move
                legal_moves[0] |= 0x2000000000000000ull;
                counts[0]++;
            }
        }
        if(castle_right[is_player_white]){
            rooks = bitfields[is_player_white] & castle_black_right_check;
            if(rooks == castle_black_right){
                // add castle right move
                legal_moves[0] |= 0x0200000000000000ull;
                counts[0]++;
            }
        }
    }
}

field find_pseudo_moves_for_piece(field piece_pos, int piece_type)
{
    switch(piece_type)
    {
        case pawn:
            return find_legal_pawn_moves(bitfields[is_player_white], bitfields[!is_player_white], piece_pos);

        case rook:
            return find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], piece_pos);

        case bishop:
            return find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], piece_pos);

        case knight:
            bit_pos = log2(piece_pos);
            return knight_moves[bit_pos] ^ (knight_moves[bit_pos] & bitfields[is_player_white]);

        case queen: ;
            field legal_moves_queen_1 = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], piece_pos);
            field legal_moves_queen_2 = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], piece_pos);
            return legal_moves_queen_1 | legal_moves_queen_2;
            
        case king:
            bit_pos = log2(piece_pos);
            field moves = king_moves[bit_pos] ^ (king_moves[bit_pos] & bitfields[is_player_white]); 

            //castle check
            if(is_player_white)
            {
                if(castle_left[is_player_white] && 
                    (bitfields[is_player_white] & castle_white_left_check) == castle_white_left)
                {
                    moves |= 0x0000000000000020ull;
                }
                if(castle_right[is_player_white] &&
                    (bitfields[is_player_white] & castle_white_right_check) == castle_white_right)
                {
                    moves |= 0x0000000000000002ull;
                }
            }
            else
            {
                if(castle_left[is_player_white] && 
                    (bitfields[is_player_white] & castle_black_left_check) == castle_black_left)
                {
                    moves |= 0x2000000000000000ull;
                }
                if(castle_right[is_player_white] && 
                    (bitfields[is_player_white] & castle_black_right_check) == castle_black_right)
                {
                    moves |= 0x0200000000000000ull;
                }
            }

            return moves;

        default:
            return (field) 0;
    }
}

typedef struct s_piece_info
{
    int piece;
    field piece_pos;
    field pseudo_moves;
    int num_moves;
} piece_move_info;

int generate_pseudo_moves(piece_move_info* results, int* total_piece_count)
{
    int total_move_count = 0;
    int res_index = 0;

    for(int piece_type = king; piece_type <= pawn; piece_type++)
    {
        field pieces = bitfields[is_player_white] & bitfields[piece_type];
        int piece_count = get_piece_count(pieces);

        *total_move_count += piece_count;

        field[] single_piece_boards;
        get_single_piece_boards(pieces, single_piece_boards, piece_count);

        for(int i = 0; i < piece_count; i++)
        {
            field current_piece = single_piece_boards[i];
            field pseudo_moves = find_pseudo_moves_for_piece(current_piece, piece_type);
            int num_moves = get_piece_count(pseudo_moves);
            
            results[res_index].piece = piece_type;
            results[res_index].piece_pos = current_piece;
            results[res_index].pseudo_moves = pseudo_moves;
            results[res_index].num_moves = num_moves;

            total_move_count += num_moves;
            res_index++;
        }
    }

    return total_move_count;
}