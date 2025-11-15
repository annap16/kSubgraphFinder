#pragma once
#include <vector>
#include <optional>

#include "graph.h"

class GraphGenerator
{
    Graph G;
    int k;

public:
    GraphGenerator(Graph G, int k) : G(G), k(k) {}
    ~GraphGenerator() = default;

    std::optional<std::vector<int>> next() {}

    GraphGenerator copy() {}
};
