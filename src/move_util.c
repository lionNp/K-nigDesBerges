

field check_for_chess(field own_pieces, field enemy_pieces, field position, bool color, field bitfield_figs[]){
    field chess_from = (field) 0;

    //check for b or q
    chess_from = chess_from | (find_legal_diag_moves(own_pieces, enemy_pieces, position) & ((bitfield_figs[b] | bitfield_figs[q]) & enemy_pieces) );

    //check for r or q
    chess_from = chess_from | (find_legal_rook_moves(own_pieces, enemy_pieces, position) & ((bitfield_figs[r] | bitfield_figs[q]) & enemy_pieces) );

    //check for n
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
        moves = moves | (((position << 8) & (own_pieces | enemy_pieces)) ^ (position << 8));

        //two forward (with magical optimisation to avoid saving and loading of registers in assembly)
        moves = moves | ((((position << 16) & (own_pieces | enemy_pieces)) ^ (position << 16))
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
        moves = moves | ((position << 7) & enemy_pieces);

        //capture left
        moves = moves | ((position << 9) & enemy_pieces);
    }
    else{
        //one forward
        moves = moves | (((position >> 8) & (own_pieces | enemy_pieces)) ^ (position >> 8));

        //two forward (with magical optimisation to avoid saving and loading of registers in assembly)
        moves = moves | ((((position >> 16) & (own_pieces | enemy_pieces)) ^ (position >> 16))
                        ^ ((position << 16) >> 32))        // check ob y = 1: wenn, dann ist pos >> 16 0, also ist pos >> 16 << 32 auch 0 -> pos >> 16 != 0
                        & ((((position >> 8) & (own_pieces | enemy_pieces)) ^ (position >> 8)) >> 8) // check ob 1 vor frei ist
        ;  
        /*if(y == 6 && ( ((position >> 8) & (own_pieces | enemy_pieces)) == (field) 0) ){
            moves = moves | (((position >> 16) & (own_pieces | enemy_pieces)) ^ (position >> 16));
        }*/
        

        //capture right
        moves = moves | ((position >> 7) & enemy_pieces);

        //capture left
        moves = moves | ((position >> 9) & enemy_pieces);
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
