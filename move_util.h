#include <string.h>
#include <stdbool.h>

typedef uint64_t field;
#define figuren_anz 8

#define white true
#define black false

field check_for_chess(field own_pieces, field enemy_pieces, field position, bool color, field all_fields[]);
field find_legal_pawn_moves(field own_pieces, field enemy_pieces, field position, bool color);
field find_legal_rook_moves(field own_pieces, field enemy_pieces, field position);
field find_legal_diag_moves(field own_pieces, field enemy_pieces, field position);