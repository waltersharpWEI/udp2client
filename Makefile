CC=g++
CXXFLAGS=-Wall -std=c++11

TARGET=mpnext
SRCS = ./src/mpcb.cpp
INC = -I./include
LIBS = -L./lib -lmpcb -lpthread

all: mpnext

mpnext : build/main.o
	$(CC) $(CXXFLAGS) -o tools/mpnext build/main.o $(LIBS)

build/main.o : tools/main.cpp lib/libmpcb.a
	$(CC) $(CXXFLAGS) $(INC) -c tools/main.cpp -o build/main.o  

lib/libmpcb.a : build/Segment.o build/MPConsumer.o build/TCPServer.o build/UDPServer.o build/TCPClient.o build/UDPClient.o build/TCPClientSuper.o build/UDPClientSuper.o build/mpcb.o
	ar -crv lib/libmpcb.a build/Segment.o build/MPConsumer.o build/TCPServer.o build/UDPServer.o build/TCPClient.o build/UDPClient.o build/TCPClientSuper.o build/UDPClientSuper.o build/mpcb.o 

build/mpcb.o : src/mpcb.cpp 
	$(CC) $(CXXFLAGS) $(INC) -o build/mpcb.o -c src/mpcb.cpp

build/Segment.o: src/Segment.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/Segment.cpp -o build/Segment.o 

build/MPConsumer.o: src/MPConsumer.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/MPConsumer.cpp -o build/MPConsumer.o 	

build/TCPServer.o: src/TCPServer.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/TCPServer.cpp -o build/TCPServer.o 	
	
build/UDPServer.o: src/UDPServer.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/UDPServer.cpp -o build/UDPServer.o 	
	
build/TCPClient.o: src/TCPClient.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/TCPClient.cpp -o build/TCPClient.o
	
build/UDPClient.o: src/UDPClient.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/UDPClient.cpp -o build/UDPClient.o
	
build/TCPClientSuper.o: src/TCPClientSuper.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/TCPClientSuper.cpp -o build/TCPClientSuper.o

build/UDPClientSuper.o: src/UDPClientSuper.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/UDPClientSuper.cpp -o build/UDPClientSuper.o
	
clean:
	rm -rf tools/mpnext build/* lib/*
