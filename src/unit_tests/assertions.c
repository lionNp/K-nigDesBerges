#include "assertions.h"
#include "bit_boards_util.h"

bool assert_bitboard_equal(field expected, field actual)
{
    bool result = expected == actual;

    if(!result)
    {
        printf("\nAssertion Failed :(\n");
        printf("Expected:\n");
        print_board(expected);
        printf("\n");
        printf("Actual:\n");
        print_board(actual);
    }

    return result;
}