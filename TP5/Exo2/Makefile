CFLAGS=-Wall -Wextra -g

TARGETS=init premier second clean troisieme

all: $(TARGETS)

init: init.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

premier: premier.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

second: second.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

clean: clean.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

troisieme: troisieme.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

dijkstra.o: ../dijkstra.h ../dijkstra.c
init.o: ../dijkstra.h init.c
premier.o: ../dijkstra.h premier.c
second.o: ../dijkstra.h second.c
clean.o: ../dijkstra.h clean.c
troisieme.o: ../dijkstra.h troisieme.c


clear:
	rm *.o $(TARGETS)
