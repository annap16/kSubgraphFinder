#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "Graph.h"
#include <vector>
#include <optional>

class GraphGenerator {
private:
    Graph& G;
    int k;
    bool finished;

    std::vector<int> comb;

    bool next_combination(std::vector<int>& c, int n, int k);

public:
    GraphGenerator(Graph& graph, int k = -1);
    std::optional<std::vector<int>> next();
};

#endif
