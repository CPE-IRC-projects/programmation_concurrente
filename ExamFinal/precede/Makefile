CFLAGS=-Wall -Wextra -g
OBJS=p1.o p2.o p3.o dijkstra.o launcher.o
all: p1 p2 p3 launcher

p1: p1.o dijkstra.o
p2: p2.o dijkstra.o
p3: p3.o dijkstra.o
launcher: launcher.o dijkstra.o

p1.o: p1.c dijkstra.h commun.h
p2.o: p2.c dijkstra.h commun.h
p3.o: p3.c dijkstra.h commun.h
launcher.o: launcher.c dijkstra.h commun.h
dijkstra.o: dijkstra.c dijkstra.h

.PHONY: clean
clean:
	rm $(OBJS) p1 p2 p3 launcher

