#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "structs.h"
#include "macd.h"
#include "parameter_exploration.h"
          

double find_price_at_date(stock_p s, unsigned int *point, string date) {
    unsigned int p = *point;
    while( p > 0 && strcmp(date, s->entries[p].date) != 0 ) {
        p--;
    }

    *point = p;

    if( p > 0)
        return s->entries[p].close;
    else
        return -1;
}

double get_reward(stock_p s, stock_moving_average_p macd_diff){
    int i = 0;
    int buy_count = 0;
    int sell_count = 0;
    unsigned int point = s->size -1;
    double reward = 0;
    double buy_point, sell_point;
    double first_buy_point=0;
    
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif

    for(int i=1; i<(macd_diff->size); i++){
	        if (macd_diff->ma[i-1].value < 0 && macd_diff->ma[i].value >= 0){
            buy_point = find_price_at_date(s, &point, &macd_diff->ma[i].date);
            if( buy_count == 0)
                first_buy_point = buy_point;
            if( buy_point == -1 ) {
                return -5;
            }
	        buy_count++;
	    }
	    else{
	    if (macd_diff->ma[i-1].value > 0 && macd_diff->ma[i].value <= 0){
                if(buy_count > 0){
			        sell_count++;

  			        /* Calculate reward here as we now have a buy-sell pair */
                    sell_point = find_price_at_date(s, &point, &macd_diff->ma[i].date);
                    if( sell_point == -1 ) {
                        return -5;
                    }
			        reward += ((sell_point - buy_point));
		        }
	        }		  
	    }        
    }

#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
    reward =(reward/first_buy_point) * 100;

    return reward;
}

double evaluate_parameter(stock_moving_average_p macd_ab, int c, stock_p s) {


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

double explore_parameters(int max_a, int max_b, int max_c, stock_p s) {
    double max_reward = -10;
    double reward = 0;
    for(int a = 0; a < max_a; a++) {
    stock_moving_average_p macd_a = calculate_moving_average(s, a);
    sort_ascending_date(macd_a);

        for(int b = 0; b < max_b; b++) {
    stock_moving_average_p macd_b = calculate_moving_average(s, b);
    sort_ascending_date(macd_b);
    stock_moving_average_p macd_ab = subtract_sma(macd_a, macd_b);
    sort_ascending_date(macd_ab);
            for(int c = 0; c < max_c; c++) {
               reward = evaluate_parameter(macd_ab, c, s);
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
