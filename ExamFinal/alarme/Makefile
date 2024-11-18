CFLAGS=-Wall -Wextra -g

TARGETS=alarme

all: $(TARGETS)

alarme: alarme.o
	gcc $^ -o $@ $(LIBS)

alarme.o: alarme.c


clean:
	rm *.o $(TARGETS)
