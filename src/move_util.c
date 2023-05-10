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
        rook_moves[i] = ranks[row] ^ files[file];
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
        queen_moves[i] = ranks[row] ^ files[file] ^ diag_r[file + row] ^ diag_l[7 + row - file];
        file += 1;
    }

    //printf("queen_moves at %d\n", position);
    //print_board(queen_moves[position]);
}

// Check if King is checked
//
//
field in_check(field position){
    field check_from = (field) 0;
    
    //check for bishop or queen
    check_from |= (find_legal_diag_moves(bitfields[is_player_white], bitfields[!is_player_white], position) & ((bitfields[bishop] | bitfields[queen]) & bitfields[!is_player_white]));

    //check for rook or queen
    check_from |= (find_legal_rook_moves(bitfields[is_player_white], bitfields[!is_player_white], position) & ((bitfields[rook] | bitfields[queen]) & bitfields[!is_player_white]));

    //check for knight
    int bit = log2(position);
    check_from |= (knight_moves[bit] & bitfields[!is_player_white]) & bitfields[knight];

    // WARNING: beware board orientation
    //check for p
    if(is_player_white){
        if(position & h_file)
            check_from |= ((position << 9) & (bitfields[pawn] & bitfields[!is_player_white]));
        else if(position & a_file)
            check_from |= ((position << 7) & (bitfields[pawn] & bitfields[!is_player_white]));
        else{
            check_from |= ((position << 7) & (bitfields[pawn] & bitfields[!is_player_white]));
            check_from |= ((position << 9) & (bitfields[pawn] & bitfields[!is_player_white]));
        }     
    }
    else{
        if(position & h_file)
            check_from |= ((position >> 7) & (bitfields[pawn] & bitfields[!is_player_white]));
        else if(position & a_file)
            check_from |= ((position >> 9) & (bitfields[pawn] & bitfields[!is_player_white]));
        else{
            check_from |= ((position >> 7) & (bitfields[pawn] & bitfields[!is_player_white]));
            
            check_from |= ((position >> 9) & (bitfields[pawn] & bitfields[!is_player_white]));
        }
    }
    //print_board(check_from ^ position);
    return check_from;
}

// Check if King is checked
//
//
field pinned_piece_check(field position){
    field pinned = 0UL;
    int bit = log2(position);
    //check for bishop or queen
    pinned |= find_bishop_pins(bitfields[is_player_white], bitfields[!is_player_white], position);

    //check for rook or queen
    pinned |= find_rook_pins(bitfields[is_player_white], bitfields[!is_player_white], position);

    return pinned;
}

field find_bishop_pins(field own_pieces, field enemy_pieces, field position){
    field moves = (field) 0;
    field moves_old = (field) 0;
    int bit_num = log2(position);

    //printf("bitnum: %d\n", bit_num);

    int x = bit_num % 8;
    int y = bit_num / 8;

    //printf("x = %d\ny = %d\n", x, y);
    int pin = 0;
    int own_count = 0;
    //check up left
    int max_steps = 7-x;
    if(7-y < 7-x) max_steps = 7-y;
    //printf("max_steps: %d\n", max_steps);
    for(int i=1; i<=max_steps; i++){
        moves |= position << (9*i);
        if((position << (9*i)) & own_pieces)
            own_count++;
        if(((position << (9*i)) & (enemy_pieces & (bitfields[bishop] | bitfields[queen])))) {
            pin = 1;
            
            break;
        }
    }
    if(!pin || own_count != 1)
        moves = moves_old;
    else{
        pin = 0;
        moves_old = moves;
    }
    own_count = 0;    
    //check up right
    max_steps = x;
    if(7-y < x) max_steps = 7-y;
    for(int i=1; i<=max_steps; i++){
        moves |= position << (7*i);
        if((position << (7*i)) & own_pieces)
            own_count++;
        if(((position << (7*i)) & (enemy_pieces & (bitfields[bishop] | bitfields[queen])))){
            pin = 1;
            break;
        } 
    }
    if(!pin || own_count != 1)
        moves = moves_old;
    else{
        pin = 0;
        moves_old = moves;
    }
    own_count = 0;
    //check down left
    max_steps = 7-x;
    if(y < 7-x) max_steps = y;
    for(int i=1; i<=max_steps; i++){
        moves |= position >> (7*i);
        if((position >> (7*i)) & own_pieces)
            own_count++;
        if(((position >> (7*i)) & (enemy_pieces & (bitfields[bishop] | bitfields[queen])))){
            pin = 1;
            break;
        }  
    }
    if(!pin || own_count != 1)
        moves = moves_old;
    else{
        pin = 0;
        moves_old = moves;
    }
    own_count = 0;
    //check down right
    max_steps = x;
    if(y < x) max_steps = y;
    for(int i=1; i<=max_steps; i++){
        moves |= position >> (9*i);
        if((position >> (9*i)) & own_pieces)
            own_count++;
        if(((position >> (9*i)) & (enemy_pieces & (bitfields[bishop] | bitfields[queen])))){
            pin = 1;
            break;
        }
    }

    if(!pin || own_count != 1)
        moves = moves_old;
    else{
        pin = 0;
        moves_old = moves;
    }
    return moves;
}


field find_rook_pins(field own_pieces, field enemy_pieces, field position){
    field moves = (field) 0;
    field moves_old = 0UL;

    int bit_num = log2(position);

    int x = bit_num % 8;
    int y = bit_num / 8;

    // printf("coords: %d, %d\n", x, y);
    int pin = 0;
    int own_count = 0;
    //check up
    for(int i = 1; i < 8-y; i++){
        moves |= position << (8*i);
        if((position << (8*i)) & own_pieces)
            own_count++;
        if(((position << (8*i)) & (enemy_pieces & (bitfields[rook] | bitfields[queen])))){
            pin = 1;
            break;
        }  
    }

    if(!pin || own_count != 1)
        moves = moves_old;
    else{
        pin = 0;
        moves_old = moves;
    }
    own_count = 0;
    //check down
    for(int i = 1; i <= y; i++){
        moves |= position >> (8*i);
        if((position >> (8*i)) & own_pieces)
            own_count++;
        if(((position >> (8*i)) & (enemy_pieces & (bitfields[rook] | bitfields[queen])))){
            pin = 1;
            break;
        }  
    }

    if(!pin || own_count != 1)
        moves = moves_old;
    else{
        pin = 0;
        moves_old = moves;
    }
    own_count = 0;
    //check left
    for(int i = 1; i < 8-x; i++){
        moves |= position << (i);
        if((position << (i)) & own_pieces)
            own_count++;
        if(((position << (i)) & enemy_pieces) != (field) 0){
            pin = 1;
            break;
        }
    }

    if(!pin || own_count != 1)
        moves = moves_old;
    else{
        pin = 0;
        moves_old = moves;
    }
    own_count = 0;
    //check right
    for(int i=1; i<=x; i++){
        moves |= position >> (i);
        if((position >> (i)) & own_pieces)
            own_count++;
        if(((position >> (i)) & (enemy_pieces & (bitfields[rook] | bitfields[queen])))){
            pin = 1;
            break;
        } 
    }

    if(!pin || own_count != 1)
        moves = moves_old;
    else{
        pin = 0;
        moves_old = moves;
    }
    return moves;
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
    if(is_player_white){
        //might be smarter and faster, might be shit (^ is bitwise xor)

        //one forward
        moves |= (((position << 8) & (own_pieces | enemy_pieces)) ^ (position << 8) );

        //two forward
        if((position & rank_2) && !(((position << 16) ^ (position << 8)) & (own_pieces | enemy_pieces)))
            moves |= (position << 16);      

        if(position & h_file)
            moves |= ((position << 9) & enemy_pieces);
        else if(position & a_file)
            moves |= ((position << 7) & enemy_pieces);
        else{
            moves |= ((position << 7) & enemy_pieces);
            moves |= ((position << 9) & enemy_pieces);
        }       
    }
    else{
        //one forward
        moves |= (((position >> 8) & (own_pieces | enemy_pieces)) ^ (position >> 8));

        //two forward
        if((position & rank_7) && !(((position >> 16) ^ (position >> 8)) & (own_pieces | enemy_pieces)))
            moves |= (position >> 16);
              
        /*if(y == 6 && ( ((position >> 8) & (own_pieces | enemy_pieces)) == (field) 0) ){
            moves = moves | (((position >> 16) & (own_pieces | enemy_pieces)) ^ (position >> 16));
        }*/
        if(position & h_file)
            moves |= ((position >> 7) & enemy_pieces);
        else if(position & a_file)
            moves |= ((position >> 9) & enemy_pieces);
        else {
            moves |= ((position >> 7) & enemy_pieces);
            moves |= ((position >> 9) & enemy_pieces);
        }
    }
    return moves;
}

field find_legal_pawn_attacks(field own_pieces, field enemy_pieces, field position){
    field moves = (field) 0;

    int bit_num = log2(position);

    int x = bit_num % 8;
    int y = bit_num / 8;

    if(!is_player_white){
        if(position & h_file)
            moves |= ((position << 9));
        else if(position & a_file)
            moves |= ((position << 7));
        else{
            moves |= ((position << 7));
            moves |= ((position << 9));
        }       
    }
    else{
        if(position & h_file)
            moves |= ((position >> 7));
        else if(position & a_file)
            moves |= ((position >> 9));
        else {
            moves |= ((position >> 7));
            moves |= ((position >> 9));
        }
    }
    return moves;
}

field find_legal_rook_moves(field own_pieces, field enemy_pieces, field position){
    field moves = (field) 0;

    int bit_num = log2(position);

    int x = bit_num % 8;
    int y = bit_num / 8;

    // printf("coords: %d, %d\n", x, y);

    //check up
    for(int i = 1; i < 8-y; i++){
         if(((position << (8*i)) & own_pieces) == (field) 0){
            moves = moves | position << (8*i);
            //printf("added: %d, %d\n", x, i+y);
            if(((position << (8*i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    //check down
    for(int i = 1; i <= y; i++){
         if(((position >> (8*i)) & own_pieces) == (field) 0){
            moves = moves | position >> (8*i);
            //printf("added: %d, %d\n", x, y-i);
            if(((position >> (8*i)) & enemy_pieces) != (field) 0) break;
        }
        else break;
    }

    //check left
    for(int i = 1; i < 8-x; i++){
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

    int bit_num = log2(position);

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
