#include "move_util.h"
#include "bit_boards_util.h"
#include "common.h"
#include <math.h>

// Generate move_masks
//
//
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

// Check if King is checked
//
//
field in_check(field position, field bitfield_figs[]){
    field check_from = (field) 0;

    //check for bishop or queen
    check_from |= (find_legal_diag_moves(bitfield_figs[turn], bitfield_figs[!turn], position) & ((bitfield_figs[bishop] | bitfield_figs[queen]) & bitfield_figs[!turn]) );

    //check for rook or queen
    check_from |= (find_legal_rook_moves(bitfield_figs[turn], bitfield_figs[!turn], position) & ((bitfield_figs[rook] | bitfield_figs[queen]) & bitfield_figs[!turn]) );

    //check for knight
    int bit = log2(position);
    check_from |= (knight_moves[bit] & bitfield_figs[!turn]) & bitfield_figs[knight];

    // WARNING: beware board orientation
    //check for p
    if(turn)
        check_from |= (((position << 9) | (position << 7)) & (bitfield_figs[pawn] & bitfield_figs[!turn]));
    else
        check_from |= (((position >> 9) | (position >> 7)) & (bitfield_figs[pawn] & bitfield_figs[!turn]));

    return check_from;
}

// get legal moves
//
//
field find_legal_pawn_moves(field own_pieces, field enemy_pieces, field position){
    field moves = (field) 0;

    int bit_num = log2(position);

    int x = bit_num % 8;
    int y = bit_num / 8;

    //printf("color: %d\n", color);
    if(turn){
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
