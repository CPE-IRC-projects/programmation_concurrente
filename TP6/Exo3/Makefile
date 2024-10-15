CFLAGS=-Wall -Wextra -g

TARGETS=init clean min max comparateur

all: $(TARGETS)

init: init.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

clean: clean.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

min: min.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

max: max.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

comparateur: comparateur.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

dijkstra.o: ../dijkstra.h ../dijkstra.c
init.o: ../dijkstra.h init.c
clean.o: ../dijkstra.h clean.c
min.o: ../dijkstra.h min.c
max.o: ../dijkstra.h max.c
max.o: ../dijkstra.h comparateur.c


clear:
	rm *.o $(TARGETS)
