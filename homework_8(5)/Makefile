all: pipe_ipc

pipe_ipc: pipe_ipc.o 
	g++ pipe_ipc.o -o pipe_ipc

pipe_ipc.o: pipe_ipc.cpp
	g++ pipe_ipc.cpp -c

clean:
	rm *.o pipe_ipc