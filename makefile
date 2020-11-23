CC = gcc

all: main

main: main.o
	$(CC) -o main main.o

main.o: main.c

.PHONY: run clean

run:
	./main
clean:
	rm *.o
