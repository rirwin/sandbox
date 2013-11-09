CC=g++
CC_FLAGS=-std=c++0x


all: main

main: main.cpp ClusteringHandler.hpp Point.hpp
	$(CC) $(CC_FLAGS) $< -o $@

clean:
	-rm main
	-rm *~