CFLAGS=-Wall -Wextra -g

TARGETS=pipeline

all: $(TARGETS)

pipeline: pipeline.o
	gcc $^ -o $@ $(LIBS)

pipeline.o: pipeline.c


clean:
	rm *.o $(TARGETS) sortie
