CFLAGS=-Wall -Wextra -g -pthread

TARGETS=mono multi

all: $(TARGETS)

mono: pi_monothread.o
	gcc $^ -o $@ $(LIBS)

multi: pi_multithread.o
	gcc $^ -o $@ $(LIBS)

pi_monothread.o: pi_monothread.c
pi_multithread.o: pi_multithread.c


clear:
	rm *.o $(TARGETS)
