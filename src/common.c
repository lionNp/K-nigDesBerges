#include "common.h"

bool turn = true;
bool castle_left[2] = {true, true};
bool castle_right[2] = {true, true};

field files[8] = {h_file, g_file, f_file, e_file, d_file, c_file, b_file, a_file};
field rows[8] = {row_1, row_2, row_3, row_4, row_5, row_6, row_7, row_8};
field diag_l[15] = {diag_l_1, diag_l_2, diag_l_3, diag_l_4, diag_l_5, diag_l_6, diag_l_7, diag_l_8, diag_l_9, diag_l_10, diag_l_11, 
                            diag_l_12, diag_l_13, diag_l_14, diag_l_15};
field diag_r[15] = {diag_r_1, diag_r_2, diag_r_3, diag_r_4, diag_r_5, diag_r_6, diag_r_7, diag_r_8, diag_r_9, diag_r_10, diag_r_11, 
                            diag_r_12, diag_r_13, diag_r_14, diag_r_15};

// move masks
field knight_moves[64] = {0x20400ull, 0x50800ull, 0xa1100ull, 0x142200ull, 0x284400ull, 0x508800ull, 0xa01000ull, 0x402000ull, 0x2040004ull, 
                        0x5080008ull, 0xa110011ull, 0x14220022ull, 0x28440044ull, 0x50880088ull, 0xa0100010ull, 0x40200020ull, 0x204000402ull, 0x508000805ull, 
                        0xa1100110aull, 0x1422002214ull, 0x2844004428ull, 0x5088008850ull, 0xa0100010a0ull, 0x4020002040ull, 0x20400040200ull, 0x50800080500ull, 
                        0xa1100110a00ull, 0x142200221400ull, 0x284400442800ull, 0x508800885000ull, 0xa0100010a000ull, 0x402000204000ull, 0x2040004020000ull, 0x5080008050000ull, 
                        0xa1100110a0000ull, 0x14220022140000ull, 0x28440044280000ull, 0x50880088500000ull, 0xa0100010a00000ull, 0x40200020400000ull, 0x204000402000000ull, 
                        0x508000805000000ull, 0xa1100110a000000ull, 0x1422002214000000ull, 0x2844004428000000ull, 0x5088008850000000ull, 0xa0100010a0000000ull, 0x4020002040000000ull, 
                        0x400040200000000ull, 0x800080500000000ull, 0x1100110a00000000ull, 0x2200221400000000ull, 0x4400442800000000ull, 0x8800885000000000ull, 0x100010a000000000ull, 
                        0x2000204000000000ull, 0x4020000000000ull, 0x8050000000000ull, 0x110a0000000000ull, 0x22140000000000ull, 0x44280000000000ull, 0x88500000000000ull, 0x10a00000000000ull, 
                        0x7f52569a4660ull};
field king_moves[64] = {0x302ull, 0x705ull, 0xe0aull, 0x1c14ull, 0x3828ull, 0x7050ull, 0xe0a0ull, 0xc040ull, 0x30203ull, 0x70507ull, 0xe0a0eull, 0x1c141cull, 0x382838ull, 0x705070ull, 
                        0xe0a0e0ull, 0xc040c0ull, 0x3020300ull, 0x7050700ull, 0xe0a0e00ull, 0x1c141c00ull, 0x38283800ull, 0x70507000ull, 0xe0a0e000ull, 0xc040c000ull, 0x302030000ull, 0x705070000ull, 0xe0a0e0000ull, 
                        0x1c141c0000ull, 0x3828380000ull, 0x7050700000ull, 0xe0a0e00000ull, 0xc040c00000ull, 0x30203000000ull, 0x70507000000ull, 0xe0a0e000000ull, 0x1c141c000000ull, 0x382838000000ull, 0x705070000000ull, 
                        0xe0a0e0000000ull, 0xc040c0000000ull, 0x3020300000000ull, 0x7050700000000ull, 0xe0a0e00000000ull, 0x1c141c00000000ull, 0x38283800000000ull, 0x70507000000000ull, 0xe0a0e000000000ull, 0xc040c000000000ull, 
                        0x302030000000000ull, 0x705070000000000ull, 0xe0a0e0000000000ull, 0x1c141c0000000000ull, 0x3828380000000000ull, 0x7050700000000000ull, 0xe0a0e00000000000ull, 0xc040c00000000000ull, 0x203000000000000ull, 
                        0x507000000000000ull, 0xa0e000000000000ull, 0x141c000000000000ull, 0x2838000000000000ull, 0x5070000000000000ull, 0xa0e0000000000000ull};


int white_pawn_values[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                       5, 5, 5, 5, 5, 5, 5, 5,
                       10, 5, 5, 10, 10, 5, 5, 10,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       25, 25, 25, 30, 30, 25, 25, 25,
                       50, 50, 50, 50, 50, 50, 50, 50};
int black_pawn_values[64] = {50, 50, 50, 50, 50, 50, 50, 50,
                        25, 25, 25, 30, 30, 25, 25, 25,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       10, 5, 5, 10, 10, 5, 5, 10,
                       5, 5, 5, 5, 5, 5, 5, 5,
                       0, 0, 0, 0, 0, 0, 0, 0};
int knight_values[64] = {5, 5, 5, 5, 5, 5, 5, 5,
                       5, 5, 10, 10, 10, 10, 5, 5,
                       5, 10, 10, 30, 30, 10, 10, 5,
                       5, 10, 10, 30, 30, 10, 10, 5,
                       5, 10, 10, 30, 30, 10, 10, 5,
                       5, 10, 10, 30, 30, 10, 10, 5,
                       5, 5, 10, 10, 10, 10, 5, 5,
                       5, 5, 5, 5, 5, 5, 5, 5};
int bishop_values[64] = {5, 5, 5, 5, 5, 5, 5, 5,
                       5, 5, 10, 10, 10, 10, 5, 5,
                       5, 10, 15, 15, 15, 15, 10, 5,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       5, 10, 15, 15, 15, 15, 10, 5,
                       5, 5, 10, 10, 10, 10, 5, 5,
                       5, 5, 5, 5, 5, 5, 5, 5,};
int rook_values[64] = {5, 5, 5, 5, 5, 5, 5, 5,
                       5, 5, 5, 5, 5, 5, 5, 5,
                       10, 5, 5, 10, 10, 5, 5, 10,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       15, 10, 10, 30, 30, 10, 10, 15,
                       50, 50, 50, 50, 50, 50, 50, 50};
int queen_values[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                       5, 5, 5, 5, 5, 5, 5, 5,
                       10, 5, 5, 10, 10, 5, 5, 10,
                       15, 10, 10, 15, 15, 10, 10, 15,
                       15, 10, 10, 15, 15, 10, 10, 15,
                       15, 10, 10, 15, 15, 10, 10, 15,
                       15, 10, 10, 15, 15, 10, 10, 15,
                       10, 5, 5, 10, 10, 5, 5, 10};
int white_king_values[64] = {15, 35, 35, 35, 35, 35, 35, 15,
                       15, 15, 15, 15, 15, 15, 15, 15,
                       5, 5, 5, 20, 20, 5, 5, 5,
                       5, 5, 5, 30, 30, 5, 5, 5,
                       5, 5, 5, 30, 30, 5, 5, 5,
                       5, 5, 5, 5, 5, 5, 5, 5,
                       5, 5, 5, 5, 5, 5, 5, 5,
                       0, 0, 0, 0, 0, 0, 0, 0,};
int black_king_values[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                            5, 5, 5, 5, 5, 5, 5, 5,
                            5, 5, 5, 5, 5, 5, 5, 5,
                            5, 5, 5, 30, 30, 5, 5, 5,
                            5, 5, 5, 30, 30, 5, 5, 5,
                            5, 5, 5, 20, 20, 5, 5, 5,
                            15, 15, 15, 15, 15, 15, 15, 15,
                            15, 35, 35, 35, 35, 35, 35, 15};