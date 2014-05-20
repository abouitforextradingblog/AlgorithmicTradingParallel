PLATFORM=linux
include Makefile.in.$(PLATFORM)

.PHONY: all

exe: algo.x
all: exe run

# =======

algo.x: main.o structs.o common.o macd.o monte.o parameter_exploration.o c_parallel.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

main.o: main.c structs.h common.h
structs.o: structs.c common.h
macd.o: macd.c structs.h common.h 
monte.o: monte.c structs.h common.h
common.o: common.c
parameter_exploration.o: parameter_exploration.c structs.h common.h
c_parallel.o: c_parallel.c parameter_exploration.h structs.h common.h
%.o: %.c
	$(CC) -c $(CFLAGS) $(OPTFLAGS) $<

# ======

run: algo.x
	./algo.x

# =======

clean:
	rm *.o
	rm *.x
