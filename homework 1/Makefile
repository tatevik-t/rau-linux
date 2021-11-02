all: main

main: main.o complex.o
	g++ main.o complex.o -o main

main.o: main.cpp complex.h
	g++ main.cpp -c

complex.o: complex.cpp complex.h
	g++ complex.cpp -c

clean:
	rm *.o main