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
float learning_rate = 0.0003f;

long total_fittings_tested = 0;

//minimum fitting that all sets need to be achieve similtaniously before termination
float all_fitting_precision = 0.98f;

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


#define i_max 3

void main()
{
    //config for random number laster, rand() returns random integer
    srand(time(NULL));

    char strings[i_max][100] =   {
                            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq",
                            "rnbqk2r/pp2bppp/2p2n2/4N3/2B1P3/8/PPP1QPPP/RNB1K2R w KQkq",
                            "8/1k6/1r3rp1/8/4R2P/2K5/3R4/8"
                        };
    float stockfish_eval[i_max] = {-6.0f, 50.0f, 8.0f};
    //initial fittings (our weights: -5.000000, 63.000000, 6.000000)

    import_gamestring(bitfields, strings[0]);
    float ev1 = evaluation();
    import_gamestring(bitfields, strings[1]);
    float ev2 = evaluation();
    import_gamestring(bitfields, strings[2]);
    float ev3 = evaluation();
    printf("fittings: %f, %f, %f\n", ev1, ev2, ev3);

    //set inital baseline
    float initial_sum_fittings = 0.0f;
    for(int i=0; i<i_max; i++)
    {
        clear_bitfields();
        import_gamestring(bitfields, strings[i]);

        eval = evaluation();
        stock_eval = stockfish_eval[i];

        get_fitting();
        initial_sum_fittings += tmp_fitting;
    }

    fitting = initial_sum_fittings / i_max;

    //hereafter in (float) tmp_fitting the new temp fitting is calculated
    printf("inital fitting: %f\n", fitting);
    printf("with inital weights:\n");
    print_all_learing_weights();

    
    //while not converged
    while(true)
    {
        //reset fitting
        float sum_fittings = 0.0f;

        //for fen-eval-pair
        for(int i=0; i<i_max; i++)
        {
            if(total_fittings_tested % 1000000 == 0)
            {
                printf("%.1fM total fittings tested\n", (float) total_fittings_tested / 1000000);
            }

            //randomise tmp learing weights
            set_all_learing_weights();
            //print_all_learing_weights();

            //get board
            clear_bitfields();
            import_gamestring(bitfields, strings[i]);

            //get own and extern evaluation
            eval = evaluation();
            stock_eval = stockfish_eval[i];

            //returns fitting into tmp_fitting
            get_fitting();

            sum_fittings += tmp_fitting;
        }
        tmp_fitting = sum_fittings / i_max;


        if(fitting < tmp_fitting)
        {
            use_learing_weights();
            fitting = tmp_fitting;

            printf("new fitting %f after %ld fittings tested\n", fitting, total_fittings_tested);
        }
        else
        {
            step_back_learing_weights();
        }

        if(tmp_fitting > all_fitting_precision)
        {
            fitting = tmp_fitting;
            break;
        }


        //terminate after 5m fittings tested
        if(total_fittings_tested > 5000000)
        {
            printf("\nover 5M fittings tested -> termination\n");
            break;
        }
    }

    import_gamestring(bitfields, strings[0]);
    ev1 = evaluation();
    import_gamestring(bitfields, strings[1]);
    ev2 = evaluation();
    import_gamestring(bitfields, strings[2]);
    ev3 = evaluation();
    printf("\nfinal fittings: %f, %f, %f\n\n", ev1, ev2, ev3);

    printf("final weights:\n");
    print_all_learing_weights();

}

void set_all_learing_weights()
{
    set_learning_weight(&material_modify);
    set_learning_weight(&position_modify);
    set_learning_weight(&contol_modify);
    set_learning_weight(&pawns_modify);
    set_learning_weight(&king_safety_modify);

    total_fittings_tested++;
}

void print_all_learing_weights()
{
    printf("material_modify: %f\n", material_modify);
    printf("position_modify: %f\n", position_modify);
    printf("contol_modify: %f\n", contol_modify);
    printf("pawns_modify: %f\n", pawns_modify);
    printf("king_safety_modify: %f\n\n", king_safety_modify);
}

void use_learing_weights()
{
    bef_learning_material_modify = material_modify;
    bef_learning_position_modify = position_modify;
    bef_learning_contol_modify = contol_modify;
    bef_learning_pawns_modify = pawns_modify;
    bef_learning_king_safety_modify = king_safety_modify;
}

void step_back_learing_weights()
{
    material_modify = bef_learning_material_modify;
    position_modify = bef_learning_position_modify;
    contol_modify = bef_learning_contol_modify;
    pawns_modify = bef_learning_pawns_modify;
    king_safety_modify = bef_learning_king_safety_modify;
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
    if((stock_eval < 0.0f && eval > 0.0f) || (stock_eval > 0.0f && eval < 0.0f))
    {
        tmp_fitting = 0.0f;
        return;
    }

    //positive
    if(eval > 0.0f && stock_eval > 0.0f)
    {
        tmp_fitting = stock_eval / eval;
        if(eval<stock_eval)
            tmp_fitting = eval/stock_eval;
    }
    //negative
    else if(eval < 0.0f && stock_eval < 0.0f)
    {
        tmp_fitting = stock_eval / eval;
        if(eval > stock_eval)
            tmp_fitting = eval/stock_eval;
    }
    //if one fitting is 0 and cant be used for division
    else{
        tmp_fitting = 0.0f;
    }

}