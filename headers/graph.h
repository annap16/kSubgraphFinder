#pragma once

#include <vector>

class MultiEdge;

class Graph
{
private:
    // adjacency[u] - map of (v -> multiplicity), where (u,v) is an edge
    std::map<int, std::map<int, int>> adjacency;
    int vertexCount = 0;

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