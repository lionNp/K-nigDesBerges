#include "common.h"
#include "assertions.h"
#include "bit_board_utils.h"
#include "stopwatch_utils.h"
#include <stdbool.h>

void start_unit_test(char name[])
{
    printf("\nStarting Unit Test: %s\n", name);
}

void print_test_result(bool test_result)
{
    printf("%s\n", test_result ? "All Test completed successfully !" : "Errors !");
}

bool test_set_bit_by_index()
{
    start_unit_test("set_bit_by_index");

    bool test_result = true;
    field max_bit = 1ul << 63;

    for(int i = 0; i < sizeof(field); i++)
    {
        field board = (field) 0;
        set_bit_by_index(&board, i);

        test_result &= assert_bitboard_equal(board, max_bit >> i);
    }

    print_test_result(test_result);
}

bool test_get_single_piece_boards()
{    
    start_unit_test("get_single_piece_boards");

    field bitboards[figure_count];
    import_gamesting(bitboards, "8/8/2pp1p2/8/8/8/8/8");

    int piece_count = 3;
    field all_black_pawns = bitboards[!is_player_white] & bitboards[pawn];

    field result[piece_count]; 
    get_single_piece_boards(all_black_pawns, result, piece_count);

    field e1 = (field) 0;
    set_bit_by_index(&e1, 18);

    field e2 = (field) 0;
    set_bit_by_index(&e2, 19);

    field e3 = (field) 0;
    set_bit_by_index(&e3, 21);

    field expected[] = {e3, e2, e1};

    bool test_result = assert_bitboards_contains_all(result, piece_count, expected, piece_count);
    
    print_test_result(test_result);

    return test_result;
}

int main()
{
    test_set_bit_by_index();
    test_get_single_piece_boards();
    return 0;
}