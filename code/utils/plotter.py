'''import datetime
import random
import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 1:
    print "Usage: python plotter.py <sma file name>"
    exit(-1)

infile = open(sys.argv[1], "r")
infile.readline()

x = []
y = []
x2 = []

for line in infile.readlines():
    splitter = line.rstrip().split(",")
    x.append(

# make up some data
x = [datetime.datetime.now() + datetime.timedelta(hours=i) for i in range(12)]
y = [i+random.gauss(0,1) for i,_ in enumerate(x)]

# plot
plt.plot(x,y)
# beautify the x-labels
plt.gcf().autofmt_xdate()

plt.show()'''

import matplotlib.pyplot as plt
from matplotlib.dates import YearLocator, MonthLocator, DateFormatter
from datetime import datetime
import sys

if len(sys.argv) < 1:
    print "Usage: python plotter.py <sma file name>"
    exit(-1)

infile = open(sys.argv[1], "r")
infile.readline()

years    = YearLocator()   # every year
months   = MonthLocator()  # every month
yearsFmt = DateFormatter('%Y')

dates = []
opens = []
smas = []
for line in infile.readlines():
    splitter = line.rstrip().split(",")
    dates.append(datetime.strptime(splitter[0], '%Y-%m-%d'))
    opens.append(splitter[1])
    smas.append(splitter[2])

fig, ax = plt.subplots()
ax.plot_date(dates, opens, '-')
ax.plot_date(dates, smas, '-')

# format the ticks
ax.xaxis.set_major_locator(years)
ax.xaxis.set_major_formatter(yearsFmt)
ax.xaxis.set_minor_locator(months)
ax.autoscale_view()

# format the coords message box
def price(x): return '$%1.2f'%x
ax.fmt_xdata = DateFormatter('%Y-%m-%d')
ax.fmt_ydata = price
ax.grid(True)

fig.autofmt_xdate()
plt.show()

