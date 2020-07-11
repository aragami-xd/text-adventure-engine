all: engine.hpp main.cpp
	g++ -std=c++17 -Wall -O1 engine.hpp main.cpp

clean:
	rm a.out
	rm *.gch