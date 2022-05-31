all: httpserver.o

httpserver.o: httpserver.hpp httpserver.cpp request.hpp response.hpp
	g++ -c httpserver.hpp httpserver.cpp request.hpp response.hpp

clean:
	rm *.o *.hpp.gch