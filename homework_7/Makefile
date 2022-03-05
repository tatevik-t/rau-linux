all: producer consumer

producer: producer.o
	g++ producer.o -o producer -lrt -pthread

producer.o: producer.cpp 
	g++ -c producer.cpp

consumer: consumer.o
	g++ consumer.o -o consumer -lrt -pthread

consumer.o: consumer.cpp
	g++ -c consumer.cpp

clean:
	rm *.o producer consumer