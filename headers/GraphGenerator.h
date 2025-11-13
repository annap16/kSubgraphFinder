#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "Graph.h"
#include <vector>
#include <optional>
#include <cstdint>

class GraphGenerator {
private:
    Graph& G;
    int k;
    uint64_t current;
    bool finished;

    static uint64_t next_combination(uint64_t x);

public:
    GraphGenerator(Graph& graph, int k = -1);
    std::optional<std::vector<int>> next();
};

#endif 
