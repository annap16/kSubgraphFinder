#include "../headers/approximation.h"

GraphAugmentationResult findCopiesApproximation(Graph &G, Graph &H, int numCopies)
{
    GraphAugmentationResult result{
        .cost = 0,
        .foundCopies = {}};
    Graph G2 = G.copy();

    for (int i = 0; i < numCopies; i++)
    {
        // find a connected subgraph of G with high degrees by removing vertices with lowest
        // degrees that are not cutting vertices
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
        result.foundCopies.push_back(FoundCopy(match));

        pickAndRemoveVertex(G2, match);
    }

    result.graphAugmentation = G2;

    return result;
}