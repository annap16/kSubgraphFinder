#pragma once

class Graph;

class GraphMappingIterator
{
    Graph &H;
    std::vector<int> &verticesSubset;

public:
    GraphMappingIterator(Graph &H, std::vector<int> &verticesSubset)
        : H(H), verticesSubset(verticesSubset) {}

    ~GraphMappingIterator() {};
    std::optional<std::vector<int>> next() {};
};
