#include "headers/approximation.h"

std::tuple<int, std::vector<std::vector<int>>> findCopiesApproximation(Graph &G, Graph &H, int numCopies)
{
    Graph G2 = G.copy();
    int cost = 0;
    std::vector<std::vector<int>> result(numCopies);

    for (int i = 0; i < numCopies; i++)
    {
        // find a connected subgraph of G with high degrees by removing vertices with lowest
        // degrees that are not cutting vertices
        std::vector<int> denseSubgraph = findDenseSubgraph(G2);

        // greedy find a match using greedy approach and score
        // (eg. -2 point for every edge missing and + is_in_dense_subraph * (number of unmatched vertices) / 3)
        auto [match, edges_to_add] = findMatch(G2, H, denseSubgraph);

        for (auto &e : edges_to_add)
        {
            G2.addEdge(e.from, e.to, e.multiplicity);
            cost += e.multiplicity;
        }

        // opcjonalne pełzanie tu
        result[i] = match;

        pickAndRemoveVertex(G2, match);
    }

    return {cost, result};
}