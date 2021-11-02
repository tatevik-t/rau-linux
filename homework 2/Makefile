all: copy

copy: copy.o
	g++ copy.o -o copy

copy.o: copy.cpp
	g++ copy.cpp -c

clean:
	rm *.o copy