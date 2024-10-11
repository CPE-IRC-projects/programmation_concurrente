CFLAGS=-Wall -Wextra -g

TARGETS=init premier second clean

all: $(TARGETS)

init: init.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

premier: premier.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

second: second.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

clean: clean.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

dijkstra.o: ../dijkstra.h ../dijkstra.c
init.o: ../dijkstra.h init.c
premier.o: ../dijkstra.h premier.c
second.o: ../dijkstra.h second.c
clean.o: ../dijkstra.h clean.c


clear:
	rm *.o $(TARGETS)
