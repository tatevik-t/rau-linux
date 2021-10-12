all: main

main: main.o
	g++ main.o -o main

main.o: main.cpp complex.o
	g++ main.cpp -c

complex.o: complex.cpp complex.h
	g++ complex.cpp -c

clean:
	rm *.o main