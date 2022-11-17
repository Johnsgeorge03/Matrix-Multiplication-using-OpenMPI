all: main

helpers.o: helpers.c helpers.h 
	mpicc -fopenmp -Wall -g -c helpers.c

main.o: main.c helpers.h
	mpicc -fopenmp -Wall -g -c main.c

main: main.o helpers.o
	mpicc -fopenmp -Wall -g -lm -o main main.o helpers.o

clean: 
	rm main *.o
