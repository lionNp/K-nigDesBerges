#ifndef _ZOBRIST_HASH_H_
#define _ZOBRIST_HASH_H_

typedef struct s_hash_entry
{
    uint64_t hash_value;
    float score;
} hash_entry;

void init_zobrist();
uint64_t calculate_hash_value();
uint64_t update_hash(uint64_t hash_value, field move_from, field move_to, int piece_type);

void put_score(uint64_t hash_value, float score);
float get_score(uint64_t hash_value);

#endif