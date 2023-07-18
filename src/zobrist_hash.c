#include "common.h"
#include "bit_boards_util.h"
#include "zobrist_hash.h"
#include <stdlib.h>

#define NO_PIECE 12
#define TABLE_SIZE 240000007

field zobrist_table[8][8][13];
field masks[8][8];

hash_entry score_table[TABLE_SIZE];

void init_zobrist()
{
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            for(int p = 0; p < 12; p++)
                zobrist_table[y][x][p] = rand();
            
            masks[y][x] = 1ULL << (x + 8 * y);
        }
    }
}

int piece_at_coords(int y, int x)
{
    for (int b = 2; b < 8; b++)
    {
        if((bitfields[b] & masks[y][x]) != 0)
            return b;
    }

    return NO_PIECE;
}

uint64_t calculate_hash_value()
{
    uint64_t value = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int piece = piece_at_coords(y, x);
            if(piece != NO_PIECE)
                piece = piece - 2 + (is_player_white ? 0 : 6);
        
            value ^= zobrist_table[y][x][piece];
        }
    }
    return value;
}

uint64_t update_hash(uint64_t hash_value, field move_from, field move_to, int piece_type, field captured[])
{
    int x = 0;
    int y = 0;
    int piece = piece_type - 2 + (is_player_white ? 0 : 6);

    int captured_piece = get_captured_piece(captured);
    if(captured_piece != -1)
    {
        pos_to_coords(move_to, &x, &y);
        printf("removing piece %d at x:%d y:%d\n", captured_piece, x, y);
        hash_value ^= zobrist_table[y][x][captured_piece - 2];

        printf("after capture: %lu\n", hash_value);
    }

    pos_to_coords(move_from, &x, &y);
    hash_value ^= zobrist_table[y][x][piece];

    pos_to_coords(move_to, &x, &y);
    hash_value ^= zobrist_table[y][x][piece];

    return hash_value;
}

void put_score(uint64_t hash_value, float score)
{
    int idx = hash_value % TABLE_SIZE;

    //if(score_table[idx].score != 0)
    //    num_hash_collisions++;

    score_table[idx].hash_value = hash_value;
    score_table[idx].score = score;
}

float get_score(uint64_t hash_value)
{
    int idx = hash_value % TABLE_SIZE;
    if(score_table[idx].score != 0)
        return score_table[idx].score;
    return 0;
}