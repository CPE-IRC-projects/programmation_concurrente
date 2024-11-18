CFLAGS=-Wall -Wextra -g

TARGETS=timeout

all: $(TARGETS)

timeout: timeout.o
	gcc $^ -o $@ $(LIBS)

timeout.o: timeout.c


clear:
	rm *.o $(TARGETS)
