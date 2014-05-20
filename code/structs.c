#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "common.h"

void string_to_entry(day_entry_p new_entry, string line) {
    char temp[100];
    int count = 0;
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif

    for( string field = strtok_r(line, ",", temp); field;
            field = strtok_r(NULL, ",", temp), count++ ) {
        switch(count) {
            case 0:
                strcpy( new_entry->date, field );
                break;
            case 1:
                new_entry->open = strtof(field, NULL);
                break;
            case 2:
                new_entry->high = strtof(field, NULL);
                break;
            case 3:
                new_entry->low = strtof(field, NULL);
                break;
            case 4:
                new_entry->close = strtof(field, NULL);
                break;
            case 6:
                new_entry->adj_close = strtof(field, NULL);
                break;
            default:
                break;
        }

    }
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
}

string get_ticker_name(string f) {
    string temp, ticker_name;
    string filename = emalloc(sizeof(char) * strlen(f) );
    strcpy(filename, f); 
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif

    temp = strtok(filename, "/");
    ticker_name = strtok(NULL, "/");

    if( ticker_name ) {
        temp = ticker_name;
    }

    ticker_name = strtok(temp, ".");

    if( !ticker_name )
        return "";
    else
        return ticker_name;

#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
}

stock_p file_to_stock(string filename) {
    FILE * fp;
    string line = NULL;
    int line_count;
    size_t len = 0;
    ssize_t read;
    stock_p new_stock = (stock_p) emalloc(sizeof(stock));
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif

    fp = fopen(filename, "r");
    if (fp == NULL)
        return NULL;

    line_count = -1; // To offset the header line
    while ( getline(&line, &len, fp) != -1 )
        if( len > 2 )
            line_count++;
    fclose(fp);

    new_stock->entries = (day_entry_p) emalloc(line_count * sizeof(day_entry));
    strcpy(&new_stock->ticker, get_ticker_name(filename));
    new_stock->size = 0;

    fp = fopen(filename, "r");
    
    //Ignore first line
    getline(&line, &len, fp);
    while ((read = getline(&line, &len, fp)) != -1)
        if(len > 2)
            string_to_entry( &new_stock->entries[new_stock->size++], line );
        //new_stock->size++;
        //printf("%s", line);

    if (line)
        free(line);

    fclose(fp);

    return new_stock;
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
}


void destroy_s(stock_p s) {
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
    free(s->entries);
    free(s);
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
}

void destroy_sma(stock_moving_average_p sma) {
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
    free(sma->ma);
    free(sma);
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
}
