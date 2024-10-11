CFLAGS=-Wall -Wextra -g

TARGETS=init emmetteur recepteur clean

all: $(TARGETS)

init: init.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

emmetteur: emmetteur.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

recepteur: recepteur.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

clean: clean.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

dijkstra.o: ../dijkstra.h ../dijkstra.c
init.o: ../dijkstra.h init.c
emmetteur.o: ../dijkstra.h emmetteur.c
recepteur.o: ../dijkstra.h recepteur.c
clean.o: ../dijkstra.h clean.c


clear:
	rm *.o $(TARGETS)
