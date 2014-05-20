#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "structs.h"
#include "macd.h"
#include "parameter_exploration.h"
#include "c_parallel.h"
          


double evaluate_parameter_c_parallel(stock_moving_average_p macd_ab, int c, stock_p s) {

    //printf("Macd a %s and Macd b %s\n", macd_a->ma[0].date, macd_b->ma[0].date);

    //printf("Macd a %s and Macd b %s\n", macd_a->ma[0].date, macd_b->ma[0].date);

    stock_moving_average_p macd_sl = calculate_moving_avg_from_sma(macd_ab, c);

    stock_moving_average_p macd_diff = subtract_sma(macd_ab, macd_sl);
    //printf("Macd ab %s and Macd sl %s\n", macd_ab->ma[0].date, macd_sl->ma[0].date);
    sort_ascending_date(macd_sl);
    sort_ascending_date(macd_diff);
    //printf("Macd ab %s and Macd sl %s\n", macd_ab->ma[0].date, macd_sl->ma[0].date);

    double r = get_reward(s, macd_diff);

    destroy_sma(macd_sl);
    destroy_sma(macd_diff);
    return r;
}

double explore_parameters_c_parallel(int max_a, int max_b, int max_c, stock_p s) {
    double max_reward = -10;

    for(int a = 1; a < max_a; a++) {
        stock_moving_average_p macd_a = calculate_moving_average(s, a);
        sort_ascending_date(macd_a);
#pragma omp parallel for shared(max_reward)
        for(int b = 1; b < max_b; b++) {
            stock_moving_average_p macd_b = calculate_moving_average(s, b);
            stock_moving_average_p macd_ab = subtract_sma(macd_a, macd_b);
            sort_ascending_date(macd_b);
            sort_ascending_date(macd_ab);
#pragma omp parallel for shared(max_reward)
            for(int c = 1; c < max_c; c++) {
               double reward = evaluate_parameter_c_parallel(macd_ab, c, s);
               //printf("Incorrect reward %lf != %lf\n", reward, evaluate_parameter(a, b, c, s));
               if(reward > max_reward) {
                   max_reward = reward;
               }
            }
            destroy_sma(macd_b);
            destroy_sma(macd_ab);
        }
        destroy_sma(macd_a);
    }
    return max_reward;
                
}
