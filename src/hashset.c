#include "hashset.h"

hashset* root = NULL;
int max_duplicates = 1;

void hashset_add()
{
    hashset* present = hashset_contains();
    if(present != NULL)
    {
        present->duplicates++;
        if(present->duplicates > max_duplicates)
            max_duplicates = present->duplicates;
        return;
    }

    hashset* entry = malloc(sizeof(hashset));

    for(int i = 0; i < figure_count; i++)
        entry->boards[i] = bitfields[i];

    entry->duplicates = 1;

    HASH_ADD(hh, root, boards, sizeof(field) * 8, entry);
}

hashset* hashset_contains()
{
    hashset* entry;
    HASH_FIND(hh, root, &bitfields, sizeof(field) * 8, entry);

    return entry;
}

void hashset_clear()
{
    
    /*
        struct s_hashset *current_user, *tmp;

    HASH_ITER(hh, root, current_user, tmp) {
        HASH_DEL(root, current_user); 
        free(current_user);            
    }
    */

    HASH_CLEAR(hh, root);

    max_duplicates = 1;
}

int hashset_duplicates()
{
    return max_duplicates;
}