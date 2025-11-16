#pragma once

#include <vector>

class MultiEdge;

class Graph
{
public:
    Graph() {};
    ~Graph() {};

    void addEdge(int u, int v, int multiplicity) {};
    std::vector<MultiEdge> getEdges(int u) {};
    int edgeCount(int u, int v) {};
    int size() {};
    Graph copy() {};
    void addIsolatedVertices(int count) {};
    void deleteEdges(std::vector<MultiEdge>){};
};