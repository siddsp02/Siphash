CC = gcc

all: siphash.o

siphash.o:
	$(CC) -c siphash.c -o siphash.o