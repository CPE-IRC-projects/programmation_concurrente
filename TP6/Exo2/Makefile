CFLAGS=-Wall -Wextra -g

TARGETS=init clean prod conso

all: $(TARGETS)

init: init.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

clean: clean.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

prod: prod.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

conso: conso.o ../dijkstra.o
	gcc $^ -o $@ $(LIBS)

dijkstra.o: ../dijkstra.h ../dijkstra.c
init.o: ../dijkstra.h init.c
clean.o: ../dijkstra.h clean.c
prod.o: ../dijkstra.h prod.c
conso.o: ../dijkstra.h conso.c


clear:
	rm *.o $(TARGETS)
