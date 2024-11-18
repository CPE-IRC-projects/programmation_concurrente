CFLAGS=-Wall -Wextra -g
OBJS=prod.o conso.o dijkstra.o launcher.o
all: conso prod launcher

prod: prod.o dijkstra.o
conso: conso.o dijkstra.o
launcher: launcher.o dijkstra.o

conso.o: conso.c dijkstra.h commun.h
prod.o: prod.c dijkstra.h commun.h
launcher.o: launcher.c dijkstra.h commun.h
dijkstra.o: dijkstra.c dijkstra.h

.PHONY: clean
clean:
	rm $(OBJS) conso prod launcher

