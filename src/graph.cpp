//
// Created by Miłosz Wysocki on 07/11/2025.
//

#include "graph.h"

void Graph::addEdge(int u, int v, int multiplicity)
{
    if (u >= vertexCount || v >= vertexCount)
        return;

    adjacency[u][v] += multiplicity;
}

std::vector<Edge> Graph::getMultiEdges(int u)
{
    if (adjacency.find(u) == adjacency.end())
        return {}; // this means u doesn't exist

    std::vector<Edge> edges;
    for (const std::pair<int,int> &keyval: adjacency[u])
    {
        edges.push_back(Edge{u, keyval.first, keyval.second});
    }

    return edges;
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
