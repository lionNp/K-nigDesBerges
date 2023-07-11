#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>
#include "common.h"
#include "uthash.h"

typedef struct s_hashset
{
    field value;
    UT_hash_handle hh;
    int duplicates;
} hashset;

extern int max_duplicates;

void hashset_add(field board);
bool hashset_contains(field board);
void hashset_clear();
int hashset_duplicates();

#endif