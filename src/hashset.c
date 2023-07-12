#include "hashset.h"

hashset* root = NULL;
int duplicate_counter = 1;

void hashset_add(field board)
{
    bool contains = hashset_contains(board);
    if(contains)
    {
        duplicate_counter++;
        return;
    }

    hashset* entry = malloc(sizeof(hashset));
    entry->value = board;

    HASH_ADD(hh, root, value, sizeof(field), entry);
}

bool hashset_contains(field board)
{
    hashset* entry;
    HASH_FIND(hh, root, &board, sizeof(field), entry);

    return entry != NULL;
}

void hashset_clear()
{
    HASH_CLEAR(hh, root);
    duplicate_counter = 1;
}

int hashset_duplicates()
{
    return duplicate_counter;
}