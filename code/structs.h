typedef char *string;

#define DATE_SIZE 12
#define TICKER_SIZE 6

typedef struct day_entry {
    char date[DATE_SIZE];
    float open;
    float high;
    float low;
    float close;
    float adj_close;
} day_entry;

typedef struct stock {
    char ticker[TICKER_SIZE];
    string stock_name;
    day_entry *entries;
    unsigned int size;
} stock;

typedef struct moving_average {
    char date[DATE_SIZE];
    double value;
} moving_average;

typedef struct stock_moving_average {
    char ticker[TICKER_SIZE];
    unsigned int period;
    moving_average *ma;
    unsigned int size;

} stock_moving_average;

typedef day_entry *day_entry_p;
typedef stock *stock_p;
typedef moving_average *moving_average_p;
typedef stock_moving_average *stock_moving_average_p;

void string_to_entry(day_entry_p new_entry, string line);

stock_p file_to_stock(string filename);

void destroy_s(stock_p s);
void destroy_sma(stock_moving_average_p sma);
