#include "move_util.h"

// Files
#define a_file 0x8080808080808080ull
#define b_file 0x4040404040404040ull
#define c_file 0x2020202020202020ull
#define d_file 0x1010101010101010ull
#define e_file 0x0808080808080808ull
#define f_file 0x0404040404040404ull
#define g_file 0x0202020202020202ull
#define h_file 0x0101010101010101ull
// rows
#define row_1 0x00000000000000ffull
#define row_2 0x000000000000ff00ull
#define row_3 0x0000000000ff0000ull
#define row_4 0x00000000ff000000ull
#define row_5 0x000000ff00000000ull
#define row_6 0x0000ff0000000000ull
#define row_7 0x00ff000000000000ull
#define row_8 0xff00000000000000ull 
// diagonals left corner
#define diag_l_1 0x0000000000000080ull
#define diag_l_2 0x0000000000008040ull
#define diag_l_3 0x0000000000804020ull
#define diag_l_4 0x0000000080402010ull
#define diag_l_5 0x0000008040201008ull
#define diag_l_6 0x0000804020100804ull
#define diag_l_7 0x0080402010080402ull
#define diag_l_8 0x8040201008040201ull
#define diag_l_9 0x4020100804020100ull
#define diag_l_10 0x2010080402010000ull
#define diag_l_11 0x1008040201000000ull
#define diag_l_12 0x0804020100000000ull
#define diag_l_13 0x0402010000000000ull
#define diag_l_14 0x0201000000000000ull
#define diag_l_15 0x0100000000000000ull
// diagonals right corner
#define diag_r_1 0x0000000000000001ull
#define diag_r_2 0x0000000000000102ull
#define diag_r_3 0x0000000000010204ull
#define diag_r_4 0x0000000001020408ull
#define diag_r_5 0x0000000102040810ull
#define diag_r_6 0x0000010204081020ull
#define diag_r_7 0x0001020408102040ull
#define diag_r_8 0x0102040810204080ull
#define diag_r_9 0x0204081020408000ull
#define diag_r_10 0x0408102040800000ull
#define diag_r_11 0x0810204080000000ull
#define diag_r_12 0x1020408000000000ull
#define diag_r_13 0x2040800000000000ull
#define diag_r_14 0x4080000000000000ull
#define diag_r_15 0x8000000000000000ull

const uint64_t files[8] = {h_file, g_file, f_file, e_file, d_file, c_file, b_file, a_file};
const uint64_t rows[8] = {row_1, row_2, row_3, row_4, row_5, row_6, row_7, row_8};
const uint64_t diag_l[15] = {diag_l_1, diag_l_2, diag_l_3, diag_l_4, diag_l_5, diag_l_6, diag_l_7, diag_l_8, diag_l_9, diag_l_10, diag_l_11, 
                            diag_l_12, diag_l_13, diag_l_14, diag_l_15};
const uint64_t diag_r[15] = {diag_r_1, diag_r_2, diag_r_3, diag_r_4, diag_r_5, diag_r_6, diag_r_7, diag_r_8, diag_r_9, diag_r_10, diag_r_11, 
                            diag_r_12, diag_r_13, diag_r_14, diag_r_15};
const int position = 32;

void init_knight_moves(field knight_moves[]){
    uint64_t pos[64];
    for(int i = 0; i < 63; i++)
    {
        pos[i] = 1UL << i;
        if((pos[i] & a_file) > 0){
            knight_moves[i] = pos[i] << 6 ^ pos[i] >> 10 ^ pos[i] << 15 ^ pos[i] >> 17;
        }
        else if((pos[i] & b_file) > 0){
            knight_moves[i] = pos[i] << 6 ^ pos[i] >> 10 ^ pos[i] << 15 ^ pos[i] >> 15 ^ pos[i] << 17 ^ pos[i] >> 17;
        }
        else if((pos[i] & g_file) > 0){
            knight_moves[i] = pos[i] >> 6 ^ pos[i] << 10 ^ pos[i] >> 15 ^ pos[i] << 15 ^ pos[i] << 17 ^ pos[i] >> 17;
        }
        else if((pos[i] & h_file) > 0){
            knight_moves[i] = pos[i] >> 6 ^ pos[i] << 10 ^ pos[i] >> 15 ^ pos[i] << 17;
        }
        else{
            knight_moves[i] = pos[i] >> 6 ^ pos[i] << 6 ^ pos[i] << 10 ^ pos[i] >> 10 ^ pos[i] >> 15 ^ pos[i] << 15 ^ pos[i] << 17 ^ pos[i] >> 17;
        }
    }
    //printf("knight moves at %d\n", position);
    //print_board(knight_moves[position]);
}

void init_rook_moves(field rook_moves[]){
    int row = 0;
    int file = 0;
    for(int i = 0; i < 63; i++)
    {
        if(i > 1 & i % 8 == 0){
            file %= 8;
            row += 1;
        }
        rook_moves[i] = rows[row] ^ files[file];
        file += 1;
    }
    //printf("rook_moves at %d\n", position);
    //print_board(rook_moves[position]);
}

void init_king_moves(field king_moves[]){
    uint64_t pos[64];   
    for(int i = 0; i < 63; i++)
    {
        pos[i] = 1UL << i;
        if((pos[i] & a_file) > 0){
            king_moves[i] = pos[i] >> 8 ^ pos[i] << 7 ^ pos[i] >> 1 ^ pos[i] << 8 ^ pos[i] >> 9;
        }
        else if((pos[i] & h_file) > 0){
            king_moves[i] = pos[i] >> 8 ^ pos[i] >> 7 ^ pos[i] << 1 ^ pos[i] << 8 ^ pos[i] << 9;
        }
        else{
            king_moves[i] = pos[i] >> 1 ^ pos[i] << 1 ^ pos[i] << 8 ^ pos[i] >> 8 ^ pos[i] >> 9 ^ pos[i] << 9 ^ pos[i] << 7 ^ pos[i] >> 7;
        }
    }
    //printf("king_moves at %d\n", position);
    //print_board(king_moves[position]);
}

void init_bishop_moves(field bishop_moves[]){
    int row = 0;
    int file = 0;
    for(int i = 0; i < 63; i++)
    {
        if(i > 1 & i % 8 == 0){
            file %= 8;
            row += 1;
        }
        bishop_moves[i] = diag_r[file + row] ^ diag_l[7 + row - file];
        file += 1;
    }
    
    //printf("bishop_moves at %d\n", position);
    //print_board(bishop_moves[position]);
}

void init_queen_moves(field queen_moves[]){
    uint64_t pos[64];
    int row = 0;
    int file = 0;
    for(int i = 0; i < 63; i++)
    {
        if(i > 1 & i % 8 == 0){
            file %= 8;
            row += 1;
        }
        queen_moves[i] = rows[row] ^ files[file] ^ diag_r[file + row] ^ diag_l[7 + row - file];
        file += 1;
    }

    //printf("queen_moves at %d\n", position);
    //print_board(queen_moves[position]);
}

field check_for_check(field own_pieces, field enemy_pieces, field position, bool color, field bitfield_figs[]){
    field chess_from = (field) 0;

    //check for b or q
    chess_from = chess_from | (find_legal_diag_moves(own_pieces, enemy_pieces, position) & ((bitfield_figs[b] | bitfield_figs[q]) & enemy_pieces) );

    //check for r or q
    chess_from = chess_from | (find_legal_rook_moves(own_pieces, enemy_pieces, position) & ((bitfield_figs[r] | bitfield_figs[q]) & enemy_pieces) );

    //check for kn
    //TODO

    // WARNING: only works if white is bottom
    //check for p
    if(color == white){
        chess_from = chess_from | ( ( (position << 9) | (position << 7) ) & (bitfield_figs[p] & bitfield_figs[bl]) );
    }
    else{
        chess_from = chess_from | ( ( (position >> 9) | (position >> 7) ) & (bitfield_figs[p] & bitfield_figs[w]) );
    }

    return chess_from;
}

field find_legal_pawn_moves(field own_pieces, field enemy_pieces, field position, bool color){
    field moves = (field) 0;

    int bit_num = 1;
    for(;bit_num <= 64; bit_num++){
        if(position >> bit_num == 0) break;
    }
    bit_num--;

    int x = bit_num % 8;
    int y = bit_num / 8;

    //printf("color: %d\n", color);
    if(color == white){
        //might be smarter and faster, might be shit (^ is bitwise xor)

        //one forward
        moves |= (((position << 8) & (own_pieces | enemy_pieces)) ^ (position << 8));

        //two forward (with magical optimisation to avoid saving and loading of registers in assembly)
        moves |= ((((position << 16) & (own_pieces | enemy_pieces)) ^ (position << 16))
                        ^ ((position >> 16) << 32))        // check ob y = 1: wenn, dann ist pos >> 16 0, also ist pos >> 16 << 32 auch 0 -> pos >> 16 != 0
                        & ((((position << 8) & (own_pieces | enemy_pieces)) ^ (position << 8)) << 8) // check ob 1 vor frei ist
        ;    

        /*
        //same as above, but with one more register loaded from memory
        moves = moves | ((((position << 16) & (own_pieces | enemy_pieces)) ^ (position << 16))
                        ^ (position & (field) 65280) << 16)       
                        & ((((position << 8) & (own_pieces | enemy_pieces)) ^ (position << 8)) << 8)
        ;  */
          

        /*if(y == 1 && ( ((position << 8) & (own_pieces | enemy_pieces)) == (field) 0) ){
            moves = moves | (((position << 16) & (own_pieces | enemy_pieces)) ^ (position << 16));
        }*/

        //capture right
        moves |= ((position << 7) & enemy_pieces);

        //capture left
        moves |= ((position << 9) & enemy_pieces);
    }
    else{
        //one forward
        moves |= (((position >> 8) & (own_pieces | enemy_pieces)) ^ (position >> 8));

        //two forward (with magical optimisation to avoid saving and loading of registers in assembly)
        moves |= ((((position >> 16) & (own_pieces | enemy_pieces)) ^ (position >> 16))
                        ^ ((position << 16) >> 32))        // check ob y = 1: wenn, dann ist pos >> 16 0, also ist pos >> 16 << 32 auch 0 -> pos >> 16 != 0
                        & ((((position >> 8) & (own_pieces | enemy_pieces)) ^ (position >> 8)) >> 8) // check ob 1 vor frei ist
        ;  
        /*if(y == 6 && ( ((position >> 8) & (own_pieces | enemy_pieces)) == (field) 0) ){
            moves = moves | (((position >> 16) & (own_pieces | enemy_pieces)) ^ (position >> 16));
        }*/
        

        //capture right
        moves |= ((position >> 7) & enemy_pieces);

        //capture left
        moves |= ((position >> 9) & enemy_pieces);
    }

    return moves;
}

field find_legal_rook_moves(field own_pieces, field enemy_pieces, field position){
    field moves = (field) 0;

    int bit_num = 1;
    for(;bit_num < 64; bit_num++){
        if(position >> bit_num == 0) break;
    }
    bit_num--;

    int x = bit_num % 8;
    int y = bit_num / 8;

    // printf("coords: %d, %d\n", x, y);

    //check up
    for(int i=1; i<8-y; i++){
         if(((position << (8*i)) & own_pieces) == (field) 0){
            moves = moves | position << (8*i);
            //printf("added: %d, %d\n", x, i+y);
            if(((position << (8*i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    //check down
    for(int i=1; i<y; i++){
         if(((position >> (8*i)) & own_pieces) == (field) 0){
            moves = moves | position >> (8*i);
            //printf("added: %d, %d\n", x, y-i);
            if(((position >> (8*i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    //check left
    for(int i=1; i<8-x; i++){
         if(((position << (i)) & own_pieces) == (field) 0){
            moves = moves | position << (i);
            //printf("added: %d, %d\n", x+i, y);
            if(((position << (i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    //check right
    for(int i=1; i<=x; i++){
         if(((position >> (i)) & own_pieces) == (field) 0){
            moves = moves | position >> (i);
            //printf("added: %d, %d\n", x-i, y);
            if(((position >> (i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    return moves;
}


field find_legal_diag_moves(field own_pieces, field enemy_pieces, field position){
    field moves = (field) 0;

    int bit_num = 1;
    for(;bit_num < 64; bit_num++){
        if(position >> bit_num == 0) break;
    }
    bit_num--;

    //printf("bitnum: %d\n", bit_num);

    int x = bit_num % 8;
    int y = bit_num / 8;

    //printf("x = %d\ny = %d\n", x, y);

    //check up left
    int max_steps = 7-x;
    if(7-y < 7-x) max_steps = 7-y;
    //printf("max_steps: %d\n", max_steps);
    for(int i=1; i<=max_steps; i++){
        //printf("try %d\n", i);
        //print_board(position << (9*i));
        //print_board(own_pieces);
        //add position to move list if doable
        if(((position << (9*i)) & own_pieces) == (field) 0){
            moves = moves | position << (9*i);
            //printf("added move\n");
            if(((position << (9*i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    //check up right
    max_steps = x;
    if(7-y < x) max_steps = 7-y;
    for(int i=1; i<=max_steps; i++){
        //printf("try %d\n", i);
        //print_board(position << (7*i));
        //print_board(own_pieces);
        //add position to move list if doable
        if(((position << (7*i)) & own_pieces) == (field) 0){
            moves = moves | position << (7*i);
            //printf("added move\n");
            if(((position << (7*i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    //check down left
    max_steps = 7-x;
    if(y < 7-x) max_steps = y;
    for(int i=1; i<=max_steps; i++){
        //printf("try %d\n", i);
        //print_board(position >> (7*i));
        //print_board(own_pieces);
        //add position to move list if doable
        if(((position >> (7*i)) & own_pieces) == (field) 0){
            moves = moves | position >> (7*i);
            //printf("added move\n");
            if(((position >> (7*i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    //check down right
    max_steps = x;
    if(y < x) max_steps = y;
    for(int i=1; i<=max_steps; i++){
        //printf("try %d\n", i);
        //print_board(position >> (9*i));
        //print_board(own_pieces);
        //add position to move list if doable
        if(((position >> (9*i)) & own_pieces) == (field) 0){
            moves = moves | position >> (9*i);
            //printf("added move\n");
            if(((position >> (9*i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    

    /*printf("moves:\n");
    print_board(moves);
    printf("initial piece:\n");
    print_board(position);*/
   

    return moves;
}
