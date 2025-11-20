#pragma once

#include <map>
#include <vector>
#include "multiEdge.h"


class Graph
{
private:
    // adjacency[u] - map of (v -> multiplicity), where (u,v) is an edge
    std::map<int, std::map<int, int>> adjacency;
    int vertexCount = 0;

public:
    Graph() {};
    ~Graph() {};

    void addEdge(int u, int v, int multiplicity);
    std::vector<MultiEdge> getMultiEdges(int u) const;
    int edgeCount(int u, int v) const;
    int size() const;
    Graph copy();
    void addIsolatedVertices(int count);
    void deleteEdges(std::vector<MultiEdge>){};
};