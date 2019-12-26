CC=g++
CXXFLAGS=-Wall -std=c++11

INC = -I./include
LIBS = -L./lib -lpthread

all: udp2

udp2 : build/main.o
	$(CC) $(CXXFLAGS) -o tools/mpnext build/main.o build/UDPClient.o $(LIBS)

build/main.o : build/UDPClient.o tools/main.cpp 
	$(CC) $(CXXFLAGS) $(INC) -c tools/main.cpp build/UDPClient.o -o build/main.o  
	
build/UDPClient.o: src/UDPClient.cpp
	$(CC) $(CXXFLAGS) $(INC) -c src/UDPClient.cpp -o build/UDPClient.o

clean:
	rm -rf tools/mpnext build/* lib/*
