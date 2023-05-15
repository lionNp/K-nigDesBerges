// #include "recursive_search.h"
// #include "move_executer.h"
// #include "move_generator.h"
// #include "bit_boards_util.h"
// #include "evaluation.h"

// //returns index 0: best_move_from, index 1 best_move_to, index 2: rating
// field* do_recursive_moves_return_rating(bool gameover, int depth, stopwatch max_time)
// {
//     // IDEA for time management
//     // 1. total time management for full best move check
//     // 2. sub max times for each branch with sub_max_time = sub_max_time.parent / 16
//     //      -> branches with fewer options can search deeper, making the proper use of check-chains possible / improving reacting to them

//     // Return values in triple:
//     // {best_move_from_position, best_move_to_position, rating}

//     //if leaf depth is reached
//     if (depth > 4)// || start_stopwatch() > max_time) //TODO: time management
//     {
//         int counts[2] = { 0, 0 };

//         //contains all player pieces
//         int piece_array[16];

//         //legal_moves[i] are all moves for piece "legal_moves_piece[i]".
//         field legal_moves[16];
//         field legal_moves_piece[16];

//         // generate all pseudo_moves
//         generate_moves(legal_moves, legal_moves_piece, piece_array);


//         int arr_size = 2 * counts[0];
//         field moves[arr_size];
//         int piece_index[arr_size];
//         float rating[arr_size];
//         int count = 0;

//         //iterate over every moveset for a piece
//         for (int i = 0; i < counts[1]; i++)
//         {
//             // get number of moves for current piece i
//             int piece_count = get_piece_count(legal_moves[i]);

//             // split moves for piece into single bitboards
//             field single_moves[piece_count];
//             get_single_piece_boards(legal_moves[i], single_moves, piece_count);

//             // foreach move the current piece could do, evaluate that move
//             for (int k = 0; k < piece_count; k++)
//             {
//                 rating[count] = evaluation(single_moves[k], legal_moves_piece[i], piece_array[i]);
//                 piece_index[count] = piece_array[i];
//                 moves[count] = legal_moves_piece[i];
//                 count++;

//                 //whats happening here ?
//                 rating[count] = 0;
//                 piece_index[count] = 0;
//                 moves[count] = single_moves[k];
//                 count++;
//             }
//         }
//         // TODO MOVE TO FIND MAX FUNCTION
//         //find first maximum rating
//         int max_rating_index = 0;
//         int total_legal_moves = counts[0];
//         for (int c = 0; c < arr_size; c++) {
//             if (rating[c] == illegal_move)
//                 total_legal_moves--;
//             else if (rating[c] > rating[max_rating_index])
//                 max_rating_index = c;
//         }

//         //HELP Jan was muss hier rein?
        
//         field ret[3] = { best_move_from, best_move_to, (field)rating[max_rating_index] }
//         return ret;
//     }

//     //counts[0]: total move count. counts[1]: total piece count (player)
//     int counts[2] = { 0, 0 };

//     //contains all player pieces
//     int piece_array[16];

//     //legal_moves[i] are all moves for piece "legal_moves_piece[i]".
//     field legal_moves[16];
//     field legal_moves_piece[16];

//     // generate all pseudo_moves
//     generate_moves(legal_moves, legal_moves_piece, piece_array, counts);

//     int moves_count;
//     field max_for_move[3] = {-1, -1, -10000};
//     field results[count[1]][3];
//     field captured_enemy_piece[8];

//     for (int i = 0; i < counts[1]; i++)
//     {
//         // if (start_stopwatch() > max_time) break;
//         //if(Zeitüberschreitung) //TODO: 

//         moves_count = get_piece_count(legal_moves[i]);

//         // split moves for piece into single bitboards
//         field single_moves[moves_count];
//         get_single_piece_boards(legal_moves[i], single_moves, moves_count);

//         field piece_moves_results[moves_count];
        
//         for (int p = 0; p < moves_count; p++) 
//         {
//             //TODO make move
//             make_move(piece_array[i], legal_moves_piece[i], single_moves[p], captured_enemy_piece);
//             piece_moves_results[p] = do_recursive_moves_return_rating(gameover, depth + 1, max_time / counts[1]);

//             //  f�r sp�ter :
//             //  if (result[i][2] > alpha / betha) break; // to break branch

//             //TODO unmake move
//             un_make_move(piece_array[i], legal_moves_piece[i], single_moves[p], captured_enemy_piece);
//         }

//         for (int p = 0; p < moves_count, p++)
//         {
//             if (piece_moves_results[p][2] > max_for_move[2])
//                 max_for_move = pieve_moves_results[p];
//         }

//     }

//     field max_move_result_pair[3] = { -1, -1, -10000 };

//     for (int i = 0; i < count[1], i++)
//     {
//         if (results[i][2] > max_move_result_pair[2])
//             max_move_result_pair = results[i];
//     }

//     return max_move_result_pair;

// }


// /*
//  * Based on Dan Bernsteins popuralized hash
//  * https://github.com/pjps/ndjbdns/blob/master/cdb_hash.c#L26
//  */

// uint32_t field_hash(const char* s)
// {
//     int len = 8; //ul
//     uint32_t h = 5381;  //prime
//     while (len--) {
//         h += (h << 5);
//         h ^= *s++;
//     }
//     return h;
// }

// uint32_t game_state_hash()
// {
//     uint32_t hash_sum = 0;
//     for (int i = 0; i < 8; i++)
//     {
//         hash_sum += field_hash((char*)bitfields[i]);
//     }
//     return hash_sum;
// }