CFLAGS=-Wall -Wextra -g

TARGETS=interrupt

all: $(TARGETS)

interrupt: interruptfils.o
	gcc $^ -o $@ $(LIBS)

interruptfils.o: interruptfils.c


clear:
	rm *.o $(TARGETS)
