#ifndef ASSERTIONS_H
#define ASSERTIONS_H

#include <stdbool.h>
#include "common.h"

bool assert_bitboard_equal(field expected, field actual);
bool assert_bitboards_contains_all(field collection[], int collection_size, field should_contain[], int element_size);

#endif