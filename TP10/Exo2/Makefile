CFLAGS=-Wall -Wextra -g

TARGETS=interrupt

all: $(TARGETS)

interrupt: dontinterruptionSigint.o
	gcc $^ -o $@ $(LIBS)

dontinterruptionSigint.o: dontinterruptionSigint.c


clear:
	rm *.o $(TARGETS)
