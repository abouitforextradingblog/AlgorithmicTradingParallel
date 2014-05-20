#include <stdio.h>
#include "structs.h"
#include "macd.h"
#include "exponent.h"
#include "monte.h"
#include "parameter_exploration.h"
#include "c_parallel.h"
#include <time.h>
#include <omp.h>

#define startTime t_start=omp_get_wtime();
#define endTime t_end=omp_get_wtime();

inline double calc_time(clock_t start, clock_t end) {
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}

void calc_sma(stock_p s, int period, int path) {
    double time;
    double t_start, t_end;
    t_start = omp_get_wtime();
    stock_moving_average_p sma = calculate_moving_average(s, period);
    t_end = omp_get_wtime();

    time = t_end - t_start;
    printf("Time taken to run %s from %s to %s, with %d period is %lf\n", s->ticker, s->entries[s->size-1].date, s->entries[0].date, period, time);
    sort_ascending_date(sma);
    sma_to_file(path, sma, s);

    destroy_sma(sma);
}

int main(int argc, char *argv[]) {
    string path = "output";
    stock_p aapl = file_to_stock("utils/IBM.csv");
    double start, end;
    double time25, time12, timesub, time200;

    //calc_sma(aapl, 50, path);
    /*calc_sma_exp(aapl,12,path);
    calc_sma(aapl, 25, path);
    calc_sma_exp(aapl, 25, path);
    calc_sma(aapl, 200, path);
    calc_sma_exp(aapl, 200, path);*/

    int a = 200, b = 22, c = 5;

    double t_start, t_end;
    printf("Running\n");
    t_start = omp_get_wtime();
    double reward = explore_parameters_c_parallel(a, b, c, aapl);
    printf("Returned reward %lf\n", reward);
    t_end = omp_get_wtime();
    printf("Run time parallel%lf\n", t_end - t_start);
    t_start = omp_get_wtime();
    reward = explore_parameters(a, b, c, aapl);
    printf("Returned reward %lf\n", reward);
    //reward = run_monte_carlo(1000, aapl); 
    //printf("Returned reward %lf\n", reward);
    t_end = omp_get_wtime();

    printf("Run time %lf\n", t_end - t_start);
    destroy_s(aapl);

    return 0;
}
