
void print_sma( stock_moving_average_p sma, int start, int num, string debug);
stock_moving_average_p calculate_moving_avg_from_sma( stock_moving_average_p sma, int period );
stock_moving_average_p calculate_moving_average( stock_p s, int period );

void sma_to_file( string path, stock_moving_average_p sma, stock_p s);

stock_moving_average_p subtract_sma(stock_moving_average_p sma1, stock_moving_average_p sma2);

void sort_ascending_date(stock_moving_average_p sma);
