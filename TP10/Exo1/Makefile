CFLAGS=-Wall -Wextra -g

TARGETS=interrupt

all: $(TARGETS)

interrupt: interruptionSigint.o
	gcc $^ -o $@ $(LIBS)

interruptionSigint.o: interruptionSigint.c


clear:
	rm *.o $(TARGETS)
