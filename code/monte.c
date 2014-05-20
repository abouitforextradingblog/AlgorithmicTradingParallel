//#include <boost/random/uniform_int_distribution.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "monte.h"

double run_monte_carlo( int iterations, stock_p s) {
    //boost::random::uniform_int_distribution<int> six(0,2);
    //boost::random::mt19937 rng;
    double final_reward = 0;

    //earninngs = (earnings_p) emalloc(sizeof(earnings_s) * iterations);

#pragma omp parallel for shared(final_reward)
    for( int i=0; i<iterations; i++) {
        //Thread initialization
        double x;
        struct drand48_data randBuffer;
        double first_point;
        double reward;
        action_t d;

        srand48_r(time(NULL), &randBuffer);
        reward = 0;
        first_point = 0;
        
        for( int j=0; j < s->size; j++) {
            //Generate random number
            drand48_r(&randBuffer, &x);
            d = x <= 0.33 ? LONG : x <= 0.66 ? NEUTRAL : SHORT;

            if(d == LONG) {
                if(first_point == 0)
                    first_point = s->entries[j].open;
                reward += ((s->entries[j].close - s->entries[j].open) );
            }
            else if(d == SHORT) {
                if(first_point == 0)
                    first_point = s->entries[j].close;
                reward += ((s->entries[j].open - s->entries[j].close));
            }
        }
        final_reward += (reward / first_point);
    }

    final_reward /= iterations;

    return final_reward*100;
}
