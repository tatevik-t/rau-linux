all: main

main: main.o parallelmatrix.o
	g++ main.o parallelmatrix.o -pthread -o main

main.o: main.cpp 
	g++ -c -pthread main.cpp

parallelmatrix.o: parallelmatrix.cpp parallelmatrix.h
	g++ -c -pthread parallelmatrix.cpp

clean:
	rm *.o main