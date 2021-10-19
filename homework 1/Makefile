all: main

main: main.o
	g++ main.o complex.o -o main

main.o: main.cpp complex.h
	g++ main.cpp

complex.o: complex.cpp complex.h
	g++ complex.cpp -c

clean:
	rm *.o main