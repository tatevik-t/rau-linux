all: main

main: main.o ParallelScheduler.o
	g++ main.o ParallelScheduler.o -pthread -o main

main.o: main.cpp 
	g++ -c -pthread main.cpp

ParallelScheduler.o: ParallelScheduler.cpp ParallelScheduler.h
	g++ -c -pthread ParallelScheduler.cpp

clean:
	rm *.o main