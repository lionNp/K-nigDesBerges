#include "main.h"
#include <math.h>
#include "common.h"
#include "bit_boards_util.h"
#include "move_util.h"
#include "evaluation.h"
#include "stopwatch_utils.h"
#include "move_generator.h"
#include "move_executer.h"
#include <stdlib.h>
#include "hashset.h"
#include "alphabeta.h"
#include <time.h>

void print_all_learing_weights();
void use_learing_weights();
void clear_bitfields();
void step_back_learing_weights();
void set_learning_weight(float* weight_to_adjust);
void get_fitting();
void set_all_learing_weights();

//globals for training
float eval;
float stock_eval;
float fitting = 0.0f;
float tmp_fitting;
float learning_rate = 0.01f;

float learning_material_modify = 20;
float learning_position_modify = 1;
float learning_contol_modify = 2;
float learning_pawns_modify = 1;
float learning_king_safety_modify = 1;

float bef_learning_material_modify = 20;
float bef_learning_position_modify = 1;
float bef_learning_contol_modify = 2;
float bef_learning_pawns_modify = 1;
float bef_learning_king_safety_modify = 1;

//std weight list for reference
/*
float material_modify = 20;
float position_modify = 1;
float contol_modify = 2;
float pawns_modify = 1;
float king_safety_modify = 1;
*/

void main()
{
    //config for random number laster, rand() returns random integer
    srand(time(NULL));

    char fen1[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    float stockfish_eval1 = 1.0f;

    //set inital baseline
    clear_bitfields();
    import_gamestring(bitfields, game_string);

    eval = evaluation();
    stock_eval = stockfish_eval1;

    //hereafter in (float) tmp_fitting the new temp fitting is calculated
    get_fitting();
    printf("inital fitting: %f\n", tmp_fitting);
    printf("with inital weights:\n");
    print_all_learing_weights();

    fitting = tmp_fitting;

    //while not converged
    while(true)
    {
        //for fen-eval-pair
        for(int i=0; i<1; i++)
        {
            //get board
            clear_bitfields();
            import_gamestring(bitfields, game_string);

            //randomise tmp learing weights
            set_all_learing_weights();
            print_all_learing_weights();

            //get own and extern evaluation
            eval = evaluation();
            stock_eval = stockfish_eval1;

            //returns fitting into tmp_fitting
            get_fitting();
            printf("new fitting: %f\n", tmp_fitting);


            if(fitting < tmp_fitting)
            {
                printf("    --fitting used\n");
                use_learing_weights();
                fitting = tmp_fitting;
            }
            else
            {
                printf("    --fitting not used\n");
                step_back_learing_weights();
            }


            //if fitting is better, adjust weights and keep running
        }

        break;
    }

}

void set_all_learing_weights()
{
    set_learning_weight(&learning_material_modify);
    set_learning_weight(&learning_position_modify);
    set_learning_weight(&learning_contol_modify);
    set_learning_weight(&learning_pawns_modify);
    set_learning_weight(&learning_king_safety_modify);
}

void print_all_learing_weights()
{
    printf("learning_material_modify: %f\n", learning_material_modify);
    printf("learning_position_modify: %f\n", learning_position_modify);
    printf("learning_contol_modify: %f\n", learning_contol_modify);
    printf("learning_pawns_modify: %f\n", learning_pawns_modify);
    printf("learning_king_safety_modify: %f\n\n", learning_king_safety_modify);
}

void use_learing_weights()
{
    bef_learning_material_modify = learning_material_modify;
    bef_learning_position_modify = learning_position_modify;
    bef_learning_contol_modify = learning_contol_modify;
    bef_learning_pawns_modify = learning_pawns_modify;
    bef_learning_king_safety_modify = learning_king_safety_modify;
}

void step_back_learing_weights()
{
    learning_material_modify = bef_learning_material_modify;
    learning_position_modify = bef_learning_position_modify;
    learning_contol_modify = bef_learning_contol_modify;
    learning_pawns_modify = bef_learning_pawns_modify;
    learning_king_safety_modify = bef_learning_king_safety_modify;
}

void set_learning_weight(float* weight_to_adjust)
{
    int range = 100;

    float random_factor = (float) (rand() % range + 1) * learning_rate;

    if(rand()%2 == 0)
        *weight_to_adjust = *weight_to_adjust + random_factor;
    else
        *weight_to_adjust = *weight_to_adjust - random_factor;
}

void clear_bitfields()
{
    for(int i=0; i<8; i++)
    {
        bitfields[i] = 0UL;
    }
}

void get_fitting()
{
    //fitting as percentage to run regression on
    tmp_fitting = stock_eval / eval;
    if(eval<stock_eval)
        tmp_fitting = eval/stock_eval;
}