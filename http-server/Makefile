all: main

main: main.o library
	g++ main.o http-server/httpserver.o -o main -L~/tato/ProgramFiles/boost_1_79_0 -pthread

main.o: main.cpp 
	g++ -c main.cpp 

library:  
	make -C http-server

clean:
	make -C http-server clean 
	rm *.o main