#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "structs.h"
#include "exponent.h"

stock_moving_average_p calculate_moving_average_exponentiated( stock_p s, int period ) {
    double sum = 0;
    int i = 0, j = 0;
    unsigned int num_points = s->size - period;
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif

    if( num_points <= 0 ) {
        return NULL;
    }
    stock_moving_average_p result = (stock_moving_average_p) emalloc(sizeof(stock_moving_average));
    result->period = period;
    result->ma = (moving_average_p) emalloc(sizeof(moving_average) * num_points);
    strcpy(result->ticker, s->ticker);

    for( i = 0; i < period && i < s->size; i++) {
    	double exp_factor=-exp(i+1);
        sum += s->entries[i].close*exp_factor;
    }
    strcpy(result->ma[j].date, s->entries[i].date);
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
    printf("Date[%d]=%s\n", i, s->entries[i-period].date);
#endif

    result->ma[j++].value = sum / period;

    for( ; i < s->size - 1; i++, j++ ) {
        strcpy(result->ma[j].date, s->entries[i+1].date);
        double sum=0;
        for(int k=0;k<period;k++){
        	double exp_factor=-exp(k+1);
        	sum += s->entries[j+k].close*exp_factor;
        }

        //sum = sum - s->entries[i-period].close + s->entries[i+1].close;
        result->ma[j].value = (float)sum / (float)period;
    }
    result->size = j;
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
    return result;
}


