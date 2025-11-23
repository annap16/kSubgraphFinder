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

std::vector<MultiEdge> Graph::getMultiEdges(int u) const
{
    auto it = adjacency.find(u);
    if (it == adjacency.end())
        return {}; // this means u doesn't exist

    std::vector<MultiEdge> edges;
    for (const auto &keyval : it->second)
    {
        edges.push_back(MultiEdge{u, keyval.first, keyval.second});
    }

    return edges;
}

int Graph::edgeCount(int u, int v) const
{
    auto itU = adjacency.find(u);
    if (itU == adjacency.end())
        return 0;

    auto itV = itU->second.find(v);
    if (itV == itU->second.end())
        return 0;

    return itV->second;
}

int Graph::size() const
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

void Graph::deleteEdges(std::vector<MultiEdge>& edges) {
    for (const auto& edge : edges) {
        int u = edge.from;
        int v = edge.to;
        int m = edge.multiplicity;
        auto u_it = adjacency.find(u);
        if (u_it != adjacency.end()) {
            auto& v_map = u_it->second;
            auto v_it = v_map.find(v);
            if (v_it != v_map.end()) {
                v_it->second -= m;
                if (v_it->second <= 0) {
                    v_map.erase(v_it);
                }
            }
            if (v_map.empty()) {
                adjacency.erase(u_it);
            }
        }
    }
}

void Graph::removeVertex(int v)
{
    adjacency.erase(v);
    for (auto &pair : adjacency)
    {
        pair.second.erase(v);
    }
}