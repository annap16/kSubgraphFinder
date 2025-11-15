#include "../headers/utils.h"
#include "../headers/graph.h"
#include "../headers/graphGenerator.h"
#include "../headers/graphMappingIterator.h"
#include "../headers/multiEdge.h"

#include <cmath>
#include <map>
#include <cstdint>
#include <optional>
#include <vector>

int verticesNeededForCopies(int numCopies, int subgraphSize, int graphSize)
{
    long double logCurrentCombination = std::log(subgraphSize + 1) + std::log(1);
    long double logNumCopies = std::log(numCopies);
    int additionalVertices = 2;

    while (logCurrentCombination < logNumCopies)
    {
        logCurrentCombination += std::log(subgraphSize + additionalVertices) + std::log(additionalVertices);
        additionalVertices++;
    }

    if (additionalVertices + subgraphSize < graphSize)
        return 0;
    return additionalVertices + subgraphSize - graphSize;
}

void findCopy(int numCopies, Graph &G, Graph &H, GraphGenerator &GG, int currentCost, int minCost)
{
    if (numCopies == 0) // base-case: found a valid copy
    {
        *RESULT = G.copy();
        cost = currentCost;
        return;
    }

    std::optional<std::vector<int>> verticesSubset;
    std::optional<std::vector<int>> verticesMapping;

    while ((verticesSubset = GG.next()))
    {
        GraphMappingIterator graphMappingIterator(H, *verticesSubset);

        while ((verticesMapping = graphMappingIterator.next()))
        {
            GraphGenerator copyGG = GG.copy();

            std::vector<MultiEdge> addedEdges = addMissingEdgesAndCalculateCost(G, H, *verticesMapping, currentCost, minCost);
            if (currentCost < minCost)
            {
                findCopy(numCopies - 1, G, H, copyGG, currentCost, minCost);
            }

            G.deleteEdges(addedEdges);
            ;
        }
    }
}

std::vector<MultiEdge> addMissingEdgesAndCalculateCost(Graph &G, Graph &H, const std::vector<int> &vertexMapping, int &currentCost, int minCost)
{
    int numVertices = H.size();

    std::vector<MultiEdge> addedEdges;

    for (int i = 0; i < numVertices; i++)
    {
        std::vector<MultiEdge> patternEdges = H.getEdges(i);

        for (MultiEdge &edge : patternEdges)
        {
            int mappedFrom = vertexMapping[edge.from];
            int mappedTo = vertexMapping[edge.to];

            int currentMultiplicity = G.edgeCount(mappedFrom, mappedTo);

            int edgesToAdd = edge.multiplicity - currentMultiplicity;
            if (edgesToAdd <= 0)
                continue;

            if (currentCost + edgesToAdd >= minCost)
            {
                currentCost = minCost;
                return addedEdges;
            }

            G.addEdge(mappedFrom, mappedTo, edgesToAdd);
            addedEdges.push_back(MultiEdge(mappedFrom, mappedTo, edgesToAdd));
            currentCost += edgesToAdd;
        }
    }

    return addedEdges;
}

void findCopies(Graph &G, Graph &H, int numCopies)
{
    int additionalVerticies = verticesNeededForCopies(numCopies, H.size(), G.size());
    if (additionalVerticies != 0)
    {
        G.addIsolatedVertices(additionalVerticies);
    }

    GraphGenerator GG(G, H.size());

    findCopy(numCopies, G, H, GG, 0, INT32_MAX);
}