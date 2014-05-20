#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "structs.h"
#include "macd.h"

void print_sma( stock_moving_average_p sma, int start, int num, string debug) {
    for(int i=start; i<start+num; i++)
        printf("%s Value[i] = %lf\n", debug, i, sma->ma[i].value);
    printf("%s done", debug);
}

stock_moving_average_p calculate_moving_avg_from_sma( stock_moving_average_p sma, int period) {
    double sum = 0;
    int i = 0, j = 0;
    int n = sma->size;
    unsigned int num_points = sma->size - period + 1;
    double alpha = ((double)2 / (double)(period + 1));
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif

    if( num_points <= 0 ) {
        return NULL;
    }


    stock_moving_average_p result = (stock_moving_average_p) emalloc(sizeof(stock_moving_average));
    result->period = period;
    result->ma = (moving_average_p) emalloc(sizeof(moving_average) * num_points);
    strcpy(result->ticker, sma->ticker);
    
    for( i = 0; i < period && i < n;  i++) {
        sum += sma->ma[i].value;
    }
    strcpy(result->ma[j].date, sma->ma[i-1].date);
    result->ma[j++].value = sum / (double)period;
        
    for( ; i < n; i++, j++ ) {
        strcpy(result->ma[j].date, sma->ma[i].date);
        result->ma[j].value = ((sma->ma[i].value - result->ma[j-1].value)
                              * alpha) + result->ma[j-1].value;
    }
    result->size = j;
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
    printf("Predicted size = %d\tActual Size = %d\n", num_points, j);
#endif
    return result;

}

stock_moving_average_p calculate_moving_average( stock_p s, int period ) {
    double sum = 0;
    int i = 0, j = 0;
    int n = s->size;
    unsigned int num_points = s->size - period ;
    double alpha = ((double)2 / (double)(period + 1));
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
    
    for( i = n - 1; i > (n-1-period); i--) {
        sum += s->entries[i].close;
    }
    strcpy(result->ma[j].date, s->entries[i].date);
    result->ma[j++].value = sum / (double)period;
        
    for( ; i > 0; i--, j++ ) {
        strcpy(result->ma[j].date, s->entries[i-1].date);
        result->ma[j].value = ((s->entries[i-1].close - result->ma[j-1].value)
                              * alpha) + result->ma[j-1].value;
    }
    result->size = j;
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
    printf("Predicted size = %d\tActual Size = %d\n", num_points, j);
#endif
    return result;
}


stock_moving_average_p subtract_sma( stock_moving_average_p sma1, stock_moving_average_p sma2 ) {

#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
    if( strcmp(sma1->ticker, sma2->ticker) != 0 ) {
        fprintf(stderr, "Incompatible tickers used in subtraction\n");
        return NULL;
    }
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif


    stock_moving_average_p result_sma = (stock_moving_average_p) emalloc(sizeof(stock_moving_average));
    int num_points = sma1->size<sma2->size ? sma1->size : sma2->size;

    moving_average_p ma1 = sma1->ma;
    moving_average_p ma2 = sma2->ma;


    result_sma->period = abs( sma1->period - sma2->period );
    result_sma->size = num_points;
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
    printf("%d\n", num_points);
#endif
    result_sma->ma = (moving_average_p) emalloc(sizeof(moving_average) * num_points);
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
    strcpy(result_sma->ticker, sma1->ticker);

    while( strcmp(ma1->date, ma2->date) > 0 ) {
        ma1++;
    }
    while( strcmp(ma2->date, ma1->date) > 0 ) {
        ma2++;
    }


    for( int i=0; i<num_points; i++) {
        strcpy(result_sma->ma[i].date, sma1->ma[i].date);
        result_sma->ma[i].value = sma1->ma[i].value - sma2->ma[i].value;
    }

    return result_sma;
}



void sma_to_file( string path, stock_moving_average_p sma, stock_p s) {
    char filename[100];
    int p = sma->period;
    int n = s->size;
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif

    if( sma == NULL )
        return;

    snprintf(filename, 100,"%s/%s.%d.ema", path, sma->ticker, sma->period);
    
    FILE *outfile = fopen(filename, "w");
    if( NULL == outfile ) {
        fprintf(stderr, "Unable to open file for writing\n");
        return;
    }

    if( s != NULL ) {
        fprintf(outfile, "Date,StockValue,MovingAverage\n");
        for( int i=sma->size - 1; i > 0 ; i--)
            fprintf(outfile, "%s,%f,%f\n", sma->ma[i].date, s->entries[n-1-p-i].close,
                                           sma->ma[i].value);
    } else {
        fprintf(outfile, "Date,MovingAverage\n");
        for( int i=sma->size - 1; i > 0 ; i--)
            fprintf(outfile, "%s,%f\n", sma->ma[i].date, s->entries[i+p].close);
        for( int i=0; i < sma->size; i++) 
            fprintf(outfile, "%s,%f\n", sma->ma[i].date, sma->ma[i].value);
    }
    fclose(outfile);
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
}


void sort_ascending_date( stock_moving_average_p sma ) {
    if( strcmp(sma->ma[0].date, sma->ma[1].date) > 0 ) {
        moving_average_p new_ma = (moving_average_p) emalloc( sizeof(moving_average) * sma->size);
        int i=0, j=sma->size - 1;
        for(; i<sma->size; i++, j--) {
            strcpy(new_ma[j].date, sma->ma[i].date);
            new_ma[j].value = sma->ma[i].value; 
        }
        free(sma->ma);
        sma->ma = new_ma;
    }
}
