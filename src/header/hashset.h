#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>
#include "common.h"
#include "uthash.h"

typedef struct s_hashset
{
    field boards[8];
    UT_hash_handle hh;
    int duplicates;
} hashset;

extern int max_duplicates;

void hashset_add();
hashset* hashset_contains();
void hashset_clear();
int hashset_duplicates();

#endif