all: shell

shell: shell.o
	g++ shell.o -o shell

shell.o: shell.cpp
	g++ shell.cpp -c

clean:
	rm *.o shell