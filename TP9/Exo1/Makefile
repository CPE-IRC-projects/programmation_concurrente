CFLAGS=-Wall -Wextra -g -pthread

TARGETS=billes

all: $(TARGETS)

billes: billes.o
	gcc $^ -o $@ $(LIBS)

billes.o: billes.c


clear:
	rm *.o $(TARGETS)
