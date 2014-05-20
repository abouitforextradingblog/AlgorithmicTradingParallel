import urllib2
import urllib
import sys

url_pattern = "http://ichart.finance.yahoo.com/table.csv?s=" + \
              "%s&d=%d&e=%d&f=%d&g=d&a=%d&b=%d&c=%s&ignore=.csv"
stock_names = [sys.argv[1]]
end_date = 4
end_month = 1
end_year = 2014

start_date = 11
start_month = 12
start_year = 1980

for stock in stock_names:
    url = url_pattern % (stock, end_date, end_month, end_year, start_date,
                         start_month, start_year)
    urllib.urlretrieve (url, stock + ".csv")
