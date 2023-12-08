all: matriz.o p4.c
	gcc $(CFLAGS) -o p4 p4.c *.o -lm

matriz.o: matriz.c matriz.h
	gcc $(CFLAGS) -c matriz.c