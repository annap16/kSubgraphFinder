#include "headers/approximation.h"

GraphAugmentationResult findCopiesApproximation(Graph &G, Graph &H, int numCopies)
{
    GraphAugmentationResult result{
        .cost = 0,
        .foundCopies = {}};
    Graph G2 = G.copy();
    int cost = 0;

    for (int i = 0; i < numCopies; i++)
    {
        // find a subgraph of G with high degrees by removing vertices with lowest degrees
        std::vector<int> denseSubgraph = findDenseSubgraph(G2, H.size());

        // greedy find a match using greedy approach and score
        // (eg. -2 point for every edge missing and + is_in_dense_subraph * (number of unmatched vertices) / 3)
        auto [match, edges_to_add] = findMatch(G2, H, denseSubgraph);

        for (auto &e : edges_to_add)
        {
            G2.addEdge(e.from, e.to, e.multiplicity);
            result.cost += e.multiplicity;
        }

        // opcjonalne pełzanie tu
        result.foundCopies.push_back(FoundCopy(denseSubgraph, match, edges_to_add.size()));

        // removes vertex with minimal degree in graph G to ensure that the match will stay unique
        pickAndRemoveVertex(G2, match);
    }

    result.graphAugmentation = G2;

    return result;
}
// removes vertex form match vector with minimal degree in graph G to ensure that the match will stay unique
void pickAndRemoveVertex(Graph &G, const std::vector<int> &match)
{
    int minDegree = -1;
    int vertexToRemove = -1;
    for (int v : match)
    {
        int degree = 0;
        auto edgesOutV = G.getMultiEdges(v);
        for (auto &e : edgesOutV)
            degree += e.multiplicity;

        for (int u : match)
        {
            if (u == v)
                continue;
            auto edgesOutU = G.getMultiEdges(u);
            for (auto &e : edgesOutU)
            {
                if (e.to == v)
                    degree += e.multiplicity;
            }
        }

        if (degree < minDegree)
        {
            minDegree = degree;
            vertexToRemove = v;
        }
    }

    G.removeVertex(vertexToRemove);
}