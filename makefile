all: engine.hpp main.cpp
	g++ -Wall -O1 engine.hpp main.cpp

clean:
	rm a.out
	rm *.gch