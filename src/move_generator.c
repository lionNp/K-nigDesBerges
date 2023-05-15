#include "move_generator.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include <math.h>

int generate_moves(field moves_from[], field moves_to[], int piece_idx[])
{
    field king_position = bitfields[is_player_white] & bitfields[king];
    field king_pinned = pinned_piece_check(king_position);
    field pin_l_diag = 0UL;
    field pin_r_diag = 0UL;
    field pin_hori = 0UL;
    field pin_vert = 0UL;
    field attacked_squares[16] = {0UL};
    int piece_array[16];
    field legal_moves[16];
    field legal_moves_piece[16];
    field danger = 0UL;
    int bit_pos = 0;
    int x = 0;
    generate_attacked_squares(attacked_squares);

    for(int i = 0; i < 16; i++){
        danger |= attacked_squares[i];
    }
    for(int i = 0; i < 16; i++){
        if(king_position & diag_l[i])
            pin_l_diag |= diag_l[i] & king_pinned;
        if(king_position & diag_r[i])
            pin_r_diag |= diag_r[i] & king_pinned;
    }
    for(int i = 0; i < 8; i++){
        if(king_position & ranks[i])
            pin_hori |= ranks[i] & king_pinned;
        if(king_position & files[i])
            pin_vert |= files[i] & king_pinned;
    }
    int bit = log2(king_position);
    if(((bitfields[king] & bitfields[is_player_white]) & danger)){
        field checked_from = in_check(king_position);
        int checkers = get_piece_count(checked_from & bitfields[!is_player_white]);
        if(checkers > 1){
            legal_moves[x] = king_moves[bit] ^ (king_moves[bit] & bitfields[is_player_white]); 
            legal_moves[x] |= castle_move();
            legal_moves[x] ^= legal_moves[x] & danger;
        }
        else{
            for(int current_piece = king; current_piece <= pawn; current_piece++)
            {
                field player_piece_board = bitfields[is_player_white] & bitfields[current_piece];
                int piece_count = get_piece_count(player_piece_board);
                field single_piece_boards[piece_count];
                get_single_piece_boards(player_piece_board, single_piece_boards, piece_count);
                for(int i = 0; i < piece_count; i++)
                {
                    switch(current_piece)
                    {
                        case pawn:
                            legal_moves[x] = find_legal_pawn_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                            break;

                        case rook:
                            legal_moves[x] = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                            break;

                        case bishop:
                            legal_moves[x] = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                            break;

                        case knight:
                            bit_pos = log2(single_piece_boards[i]);
                            legal_moves[x] = knight_moves[bit_pos] ^ (knight_moves[bit_pos] & bitfields[is_player_white]);
                            break;

                        case queen: ;
                            field legal_moves_queen_1 = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                            field legal_moves_queen_2 = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                            legal_moves[x] = legal_moves_queen_1 | legal_moves_queen_2;
                            break;
                            
                        case king:
                            bit_pos = log2(single_piece_boards[i]);
                            legal_moves[x] = king_moves[bit_pos] ^ (king_moves[bit_pos] & bitfields[is_player_white]); 
                            legal_moves[x] |= castle_move();
                            legal_moves[x] ^= legal_moves[x] & danger;
                            break;
                    }
                    if(current_piece != king)
                        legal_moves[x] &= checked_from;
                    if(king_pinned){
                        for(int x = 0; x < 4; x++){
                            if(legal_moves_piece[x] & pin_l_diag)
                                legal_moves[x] &= pin_l_diag;
                            else if(legal_moves_piece[x] & pin_r_diag)
                                legal_moves[x] &= pin_r_diag;
                            else if(legal_moves_piece[x] & pin_hori)
                                legal_moves[x] &= pin_hori;
                            else if(legal_moves_piece[x] & pin_vert)
                                legal_moves[x] &= pin_vert;
                        }
                    }
                    if(legal_moves[x]){
                        piece_array[x] = current_piece;
                        legal_moves_piece[x] = single_piece_boards[i];
                        x++;
                    }
                }
            }
        }
    }else{
        for(int current_piece = king; current_piece <= pawn; current_piece++)
        {
            field player_piece_board = bitfields[is_player_white] & bitfields[current_piece];
            int piece_count = get_piece_count(player_piece_board);
            field single_piece_boards[piece_count];
            get_single_piece_boards(player_piece_board, single_piece_boards, piece_count);
            for(int i = 0; i < piece_count; i++)
            {
                switch(current_piece)
                {
                    case pawn:
                        legal_moves[x] = find_legal_pawn_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                        break;

                    case rook:
                        legal_moves[x] = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                        break;

                    case bishop:
                        legal_moves[x] = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                        break;

                    case knight:
                        bit_pos = log2(single_piece_boards[i]);
                        legal_moves[x] = knight_moves[bit_pos] ^ (knight_moves[bit_pos] & bitfields[is_player_white]);
                        break;

                    case queen: ;
                        field legal_moves_queen_1 = find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                        field legal_moves_queen_2 = find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], single_piece_boards[i]);
                        legal_moves[x] = legal_moves_queen_1 | legal_moves_queen_2;
                        break;
                        
                    case king:
                        bit_pos = log2(single_piece_boards[i]);
                        legal_moves[x] = king_moves[bit_pos] ^ (king_moves[bit_pos] & bitfields[is_player_white]); 
                        legal_moves[x] |= castle_move();
                        legal_moves[x] ^= legal_moves[x] & danger;
                        break;
                }
                if(king_pinned){
                    for(int x = 0; x < 4; x++){
                        if(legal_moves_piece[x] & pin_l_diag)
                            legal_moves[x] &= pin_l_diag;
                        else if(legal_moves_piece[x] & pin_r_diag)
                            legal_moves[x] &= pin_r_diag;
                        else if(legal_moves_piece[x] & pin_hori)
                            legal_moves[x] &= pin_hori;
                        else if(legal_moves_piece[x] & pin_vert)
                            legal_moves[x] &= pin_vert;
                    }
                }
                if(legal_moves[x]){
                    piece_array[x] = current_piece;
                    legal_moves_piece[x] = single_piece_boards[i];
                    x++;
                }
            }
        }
    }
    
    int move_count = 0;
    for(int i = 0; i < x; i++){
        int current_mc = get_piece_count(legal_moves[i]);
        field single_moves[current_mc];
        get_single_piece_boards(legal_moves[i], single_moves, current_mc);
        for(int k = 0; k < current_mc; k++)
        {
            piece_idx[move_count] = piece_array[i];
            moves_from[move_count] = legal_moves_piece[i];
            moves_to[move_count] = single_moves[k];
            move_count++;
        }
    }
    return move_count;
}

// pinned example: field king_pinned = pinned_piece_check(king_position);
field filter_pin_moves(field pinned, field piece, field moves, field position){
    field pin_l_diag = 0UL;
    field pin_r_diag = 0UL;
    field pin_hori = 0UL;
    field pin_vert = 0UL;
    for(int i = 0; i < 16; i++){
        if(position & diag_l[i])
            pin_l_diag |= diag_l[i] & pinned;
        if(position & diag_r[i])
            pin_r_diag |= diag_r[i] & pinned;
    }
    for(int i = 0; i < 8; i++){
        if(position & ranks[i])
            pin_hori |= ranks[i] & pinned;
        if(position & files[i])
            pin_vert |= files[i] & pinned;
    }
    for(int x = 0; x < 4; x++){
        if(piece & pin_l_diag)
            moves &= pin_l_diag;
        else if(piece & pin_r_diag)
            moves &= pin_r_diag;
        else if(piece & pin_hori)
            moves &= pin_hori;
        else if(piece & pin_vert)
            moves &= pin_vert;
    }
    return moves;
}

field castle_move(){
    //castle check
    field move = 0UL;
    field rooks = 0UL;
    if(is_player_white){
        if(castle_left[is_player_white]){
            rooks = bitfields[is_player_white] & castle_white_left_check;
            if(rooks == castle_white_left)
                move |= 0x0000000000000020ull;
        }
        if(castle_right[is_player_white]){
            rooks = bitfields[is_player_white] & castle_white_right_check;
            if(rooks == castle_white_right)
                move |= 0x0000000000000002ull;
        }
    }
    else{
        if(castle_left[is_player_white]){
            rooks = bitfields[is_player_white] & castle_black_left_check;
            if(rooks == castle_black_left)
                move |= 0x2000000000000000ull;
        }
        if(castle_right[is_player_white]){
            rooks = bitfields[is_player_white] & castle_black_right_check;
            if(rooks == castle_black_right)
                move |= 0x0200000000000000ull;
        }
    }
    return move;
}

void generate_attacked_squares(field attacked_squares[])
{
    int bit_pos = 0;
    int x = 0;
    for(int current_piece = king; current_piece <= pawn; current_piece++)
    {
        field opponent_piece_board = bitfields[!is_player_white] & bitfields[current_piece];
        int piece_count = get_piece_count(opponent_piece_board);
        field single_piece_boards[piece_count];
        get_single_piece_boards(opponent_piece_board, single_piece_boards, piece_count);
        field own_king = bitfields[king] & bitfields[is_player_white];
        for(int i = 0; i < piece_count; i++)
        {
            switch(current_piece)
            {
                case pawn:
                    attacked_squares[x] = find_legal_pawn_attacks(bitfields[!is_player_white], bitfields[is_player_white] ^ own_king, single_piece_boards[i]);
                    break;

                case rook:
                    attacked_squares[x] = find_legal_rook_attacks(bitfields[!is_player_white], bitfields[is_player_white] ^ own_king, single_piece_boards[i]);
                    break;

                case bishop:
                    attacked_squares[x] = find_legal_diag_attacks(bitfields[!is_player_white], bitfields[is_player_white] ^ own_king, single_piece_boards[i]);
                    break;

                case knight:
                    bit_pos = log2(single_piece_boards[i]);
                    attacked_squares[x] = knight_moves[bit_pos] ^ (knight_moves[bit_pos] & bitfields[is_player_white]);
                    break;

                case queen: ;
                    field legal_moves_queen_1 = find_legal_diag_attacks(bitfields[!is_player_white], bitfields[is_player_white] ^ own_king, single_piece_boards[i]);
                    field legal_moves_queen_2 = find_legal_rook_attacks(bitfields[!is_player_white], bitfields[is_player_white] ^ own_king, single_piece_boards[i]);
                    attacked_squares[x] = legal_moves_queen_1 | legal_moves_queen_2;
                    break;
                    
                case king:
                    bit_pos = log2(single_piece_boards[i]);
                    attacked_squares[x] = king_moves[bit_pos] ^ (king_moves[bit_pos] & bitfields[is_player_white]); 
            }
            x++;
        }
    }
}
    