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

std::tuple<std::vector<int>, std::vector<MultiEdge>> findMatch(Graph &G, Graph &H, const std::vector<int> &denseSubgraph)
{
    int n = G.size();
    int m = H.size();
    int remainingUnmatched = m;

    // match[i] - number of a vertex from G mapped to i-th vertex from H
    std::vector<int> match(m, -1);

    std::vector<MultiEdge> missingEdges;

    std::vector<bool> used(n, false);

    // For quick check if a vertex is in dense subgraph
    std::unordered_set<int> denseSet(denseSubgraph.begin(), denseSubgraph.end());

    for(int h = 0; h < m; ++h) {
        double bestScore = -std::numeric_limits<double>::infinity();
        std::vector<int> candidates;

        for(int g = 0; g < n; ++g) {
            if(used[g]) continue;

            // Penalty for having a small degree (does that make sense?)
            int degH = H.getMultiEdges(h).size();
            int degG = G.getMultiEdges(g).size();
            int lackingDeg = (degH > degG) ? (degH - degG) : 0;

            // Missing edges to already mapped vertices
            int missingCount = 0;
            for(const MultiEdge &e : H.getMultiEdges(h)) {
                int h2 = e.to;
                int g2 = match[h2];
                if(g2 < 0) continue; // h2 is not mapped yet

                int edgesH = e.multiplicity;
                int edgesG = G.edgeCount(g, g2);

                if(edgesG < edgesH) {
                    missingCount += (edgesH - edgesG);
                }
            }

            int isInDenseSubgraph = denseSet.count(g) > 0;

            // We can adjust it like (-2 point for every edge missing and + is_in_dense_subraph * (number of unmatched vertices) / 3) if you want
            double score = -static_cast<double>(lackingDeg) //?
                           -static_cast<double>(missingCount)
                           + static_cast<double>(isInDenseSubgraph * remainingUnmatched);

            if(score > bestScore) {
                bestScore = score;
                candidates.clear();
                candidates.push_back(g);
            } else if(score == bestScore) { // stalemate
                candidates.push_back(g);
            }
        }

        int chosenG = candidates[0];

        // Save the match
        match[h] = chosenG;
        used[chosenG] = true;
        --remainingUnmatched;

        // Add missing edges to already mapped vertices
        for (const MultiEdge &e : H.getMultiEdges(h))
        {
            int h2 = e.to;
            int g2 = match[h2];
            if (g2 < 0) continue;

            int edgesH = e.multiplicity;
            int edgesG = G.edgeCount(chosenG, g2);

            if (edgesH > edgesG) {
                missingEdges.push_back(
                        MultiEdge(chosenG, g2, edgesH - edgesG)
                );
            }
        }
    }

    return {match, missingEdges};
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