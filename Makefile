CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iheaders -g

SRC = src/main.cpp \
	src/utils.cpp \
	src/graph.cpp \
    src/graphGenerator.cpp \
    src/inputParser.cpp \
	src/graphMappingIterator.cpp \
	src/findDenseSubgraph.cpp \
	src/approximation.cpp \
	src/resultWriter.cpp

OUT = main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
