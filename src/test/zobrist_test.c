#include "common.h"
#include "zobrist_hash.h"

void main ()
{
    uint64_t hash = 1234567890;
    float score = 1.0f;

    put_score(hash, score);

    printf("Score: %f\n", get_score(hash));
}