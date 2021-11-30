all: shell_silent

shell_silent: shell_silent.o
	g++ shell_silent.o -o shell_silent

shell_silent.o: shell_silent.cpp
	g++ shell_silent.cpp -c

shell: shell.o
	g++ shell.o -o shell

shell.o: shell.cpp
	g++ shell.cpp -c

clean:
	rm *.o shell shell_silent