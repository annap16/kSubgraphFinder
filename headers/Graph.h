#pragma once


#include <map>


class Graph
{
public:
    Graph() {};
    ~Graph() {};


    void addEdge(int u, int v, int multiplicity) {};
    std::map<int, int> getEdgesWithMultiplicity(int u) {};
    int edgeCount(int u, int v) {};
    int size() {};
    Graph copy() {};
    Graph *createNew() {};
    void addIsolatedVertices(int count) {};

};