CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iheaders

SRC = src/main.cpp src/utils.cpp
OUT = main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
