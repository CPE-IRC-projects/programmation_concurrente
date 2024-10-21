CFLAGS=-Wall -Wextra -g -pthread

TARGETS=sansbarriere barriere

all: $(TARGETS)

sansbarriere: sansbarriere.o
	gcc $^ -o $@ $(LIBS)

sansbarriere.o: sansbarriere.c

barriere: barriere.o
	gcc $^ -o $@ $(LIBS)

barriere.o: barriere.c

clear:
	rm *.o $(TARGETS)