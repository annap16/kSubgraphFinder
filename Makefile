CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iheaders -Itest

# main
SRC = src/main.cpp \
      src/utils.cpp \
      src/graph.cpp \
      src/graphGenerator.cpp \
      src/inputParser.cpp \
      src/graphMappingIterator.cpp \
      src/findDenseSubgraph.cpp \
      src/approximation.cpp \
      src/resultWriter.cpp

MAIN_OUT = main

# test
TEST_SRC = test/test_utils/testRunner.cpp \
		   test/testMain.cpp \
           src/utils.cpp \
           src/graph.cpp \
           src/graphGenerator.cpp \
           src/inputParser.cpp \
           src/graphMappingIterator.cpp \
           src/findDenseSubgraph.cpp \
           src/approximation.cpp \
           src/resultWriter.cpp

TEST_OUT = test/testMain

all: $(MAIN_OUT)

$(MAIN_OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(MAIN_OUT)

run: $(MAIN_OUT)
	./$(MAIN_OUT)

test_build: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_OUT)

test: test_build
	./$(TEST_OUT)

clean:
	rm -f $(MAIN_OUT) $(TEST_OUT)
