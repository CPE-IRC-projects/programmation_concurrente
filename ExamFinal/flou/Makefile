CFLAGS=-Wall -Wextra -g -pthread
LDLIBS=-pthread

all: flou

flou:flou.o
flou.o:flou.c

.PHONY: clean
clean:
	rm flou.o flou sortie/*
