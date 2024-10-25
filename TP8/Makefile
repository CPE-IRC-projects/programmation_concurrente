CFLAGS = -Wall -Wextra -ggdb3 -pthread -O3 -march=native
LDADD = -pthread -lm

all: pipeline-graphic

pipeline-graphic: pipeline-graphic.o gpu.o
	gcc $^ -o$@ $(LDADD)
gpu.o pipeline-graphic.o: gpu.h

.PHONY: clean
clean:
	@rm -f *.o pipeline-graphic
