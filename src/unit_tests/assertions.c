#include "assertions.h"
#include "bit_board_utils.h"

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

bool assert_bitboards_contains_all(field collection[], int coll_size, field should_contain[], int element_size)
{
    bool result = true;

    for(int i = 0; i < element_size; i++)
    {
        bool contains = false;
        for(int j = 0; j < coll_size; j++)
        {
            if(collection[j] == should_contain[i])
            {
                contains = true;
                break;
            }
        }

        if(!contains)
        {
            printf("\nAssertion Failed :(\n");
            printf("Collection did not contain: \n");
            print_board(should_contain[i]);
            result = false;
            break;
        }
    }

    return result;
}