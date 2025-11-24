#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <iostream>
#include "../headers/graph.h"

std::vector<int> findDenseSubgraph(Graph &G, int m, const std::vector<bool> &deleted_vertices)
{
    Graph H = G.copy();

    std::vector<int> vertices;
    for (int u = 0; u < H.size(); ++u)
        if (!deleted_vertices[u])
            vertices.push_back(u);

    while ((int)vertices.size() > m)
    {
        int minDegree = std::numeric_limits<int>::max();
        int toRemove = -1;

        for (int u : vertices)
        {
            int degree = 0;

            auto edgesOut = H.getMultiEdges(u);
            for (auto &e : edgesOut)
                degree += e.multiplicity;

            for (int v : vertices)
            {
                if (v == u)
                    continue;
                auto edgesIn = H.getMultiEdges(v);
                for (auto &e : edgesIn)
                {
                    if (e.to == u)
                        degree += e.multiplicity;
                }
            }

            if (degree < minDegree)
            {
                minDegree = degree;
                toRemove = u;
            }
        }

        if (toRemove == -1)
        {
            toRemove = vertices[rand() % vertices.size()];
        }

        std::vector<MultiEdge> outEdges = H.getMultiEdges(toRemove);
        H.deleteEdges(outEdges);

        std::vector<MultiEdge> inEdges;
        for (int v : vertices)
        {
            if (v == toRemove)
                continue;
            auto edges = H.getMultiEdges(v);
            for (auto &e : edges)
            {
                if (e.to == toRemove)
                    inEdges.push_back(e);
            }
        }
        H.deleteEdges(inEdges);

        vertices.erase(std::remove(vertices.begin(), vertices.end(), toRemove), vertices.end());
    }

    return vertices;
}
