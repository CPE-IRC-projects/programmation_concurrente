CFLAGS=-Wall -Wextra -g

TARGETS=init clean nrdv

all: $(TARGETS)

init: init.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

clean: clean.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

nrdv: nrdv.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

dijkstra.o: ../dijkstra.h ../dijkstra.c
init.o: ../dijkstra.h init.c
clean.o: ../dijkstra.h clean.c
nrdv.o: ../dijkstra.h nrdv.c


clear:
	rm *.o $(TARGETS)
