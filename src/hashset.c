#include "hashset.h"

hashset* root = NULL;
int max_duplicates = 1;

void hashset_add(field board)
{
    hashset* present = hashset_contains(board);
    if(present != NULL)
    {
        present->duplicates++;
        if(present->duplicates > max_duplicates)
            max_duplicates = present->duplicates;
        return;
    }

    hashset* entry = malloc(sizeof(hashset));
    entry->value = board;
    entry->duplicates = 1;

    HASH_ADD(hh, root, value, sizeof(field), entry);
}

hashset* hashset_contains(field board)
{
    hashset* entry;
    HASH_FIND(hh, root, &board, sizeof(field), entry);

    return entry;
}

void hashset_clear()
{
    struct s_hashset *current_user, *tmp;

    HASH_ITER(hh, root, current_user, tmp) {
        HASH_DEL(root, current_user);  /* delete; users advances to next */
        free(current_user);             /* optional- if you want to free  */
    }

    HASH_CLEAR(hh, root);

    max_duplicates = 1;
}

int hashset_duplicates()
{
    return max_duplicates;
}