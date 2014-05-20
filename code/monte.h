
typedef enum {LONG=0, NEUTRAL, SHORT} action_t;

typedef struct trade_s {
    char date[DATE_SIZE];
    action_t action;
    double raw_gain;
    double actual_gain;
} trade_s;

typedef struct trade_s *trade_p;

typedef struct earnings_s {
    char ticker[TICKER_SIZE];
    trade_p trades;
    unsigned int size;
} earnings_s;

typedef struct earnings_s earnings_p;

double run_monte_carlo(int interactions, stock_p s);
