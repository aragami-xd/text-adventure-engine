CXXFLAGS := -std=c++17 -Wall
CXX := g++
OUT := a.out

debug: engine.hpp main.cpp
	${CXX} -o ${OUT} ${CXXFLAGS} -g engine.hpp main.cpp

release: engine.hpp main.cpp
	${CXX} -o ${OUT} ${CXXFLAGS} -O1 engine.hpp main.cpp

clean:
	rm ${OUT}
	rm *.gch