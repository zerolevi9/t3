# makefile

all: main

main: main.o fila.o
	gcc -o main main.o fila.o

main.o: main.c fila.h
	gcc -o main.o main.c -c -W -Wall -ansi -pedantic -std=gnu11

logging.o: fila.c fila.h
	gcc -o fila.o fila.c -c -W -Wall -ansi -pedantic -std=gnu11

clean:
	rm -rf *.o *~ main
