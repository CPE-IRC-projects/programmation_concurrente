CFLAGS=-lm -Wall -Wextra -g 

TARGETS=fractal

all: $(TARGETS)

fractal: fractal.o
	gcc $^ -o $@ $(LIBS)

fractal.o: fractal.c

clear:
	rm *.o $(TARGETS)