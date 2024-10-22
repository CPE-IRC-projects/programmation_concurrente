CFLAGS=-Wall -Wextra -g -pthread

TARGETS=prodconso

all: $(TARGETS)

prodconso: prodconso.o
	gcc $^ -o $@ $(LIBS)

prodconso.o: prodconso.c

clear:
	rm *.o $(TARGETS)