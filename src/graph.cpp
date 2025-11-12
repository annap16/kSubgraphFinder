//
// Created by Miłosz Wysocki on 07/11/2025.
//

#include "graph.h"

void Graph::addEdge(int u, int v, int multiplicity)
{
    // should we add error handling for non-existing u,v?
    adjacency[u][v] += multiplicity;
}

std::map<int, int> Graph::getEdgesWithMultiplicity(int u)
{
    if (adjacency.find(u) == adjacency.end())
        return {}; // this means u doesn't exist

    return adjacency[u];
}

int Graph::edgeCount(int u, int v)
{
    auto itU = adjacency.find(u);
    if (itU == adjacency.end())
        return 0;

    auto itV = itU->second.find(v);
    if (itV == itU->second.end())
        return 0;

    return itV->second;
}

int Graph::size()
{
    return vertexCount;
}

Graph Graph::copy()
{
    Graph gCopy;
    gCopy.adjacency = adjacency;
    gCopy.vertexCount = vertexCount;
    return gCopy;
}

void Graph::addIsolatedVertices(int count)
{
    for (int i = 0; i < count; ++i)
    {
        int newId = vertexCount + i;
        adjacency[newId] = {};
    }
    vertexCount += count;
}
