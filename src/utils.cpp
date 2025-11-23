#include "../headers/utils.h"
#include "../headers/graph.h"
#include "../headers/graphGenerator.h"
#include "../headers/graphMappingIterator.h"
#include "../headers/multiEdge.h"
#include "../headers/graphAugmentationResult.h"
#include "../headers/foundCopy.h"

#include <cmath>
#include <map>
#include <cstdint>
#include <optional>
#include <vector>

void findCopy(int numCopies, Graph &G, Graph &H, GraphGenerator &GG, int currentCost, int &minCost, GraphAugmentationResult &result, std::vector<FoundCopy> currentCopies)
{
    if (numCopies == 0)
    {
        result.graphAugmentation = G.copy();
        result.cost = currentCost;
        result.foundCopies = currentCopies;
        minCost = currentCost;
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

            auto [addedEdges, addedEdgesCost] = addMissingEdgesAndCalculateCost(G, H, *verticesMapping, currentCost, minCost);
            currentCost += addedEdgesCost;

            if (currentCost < minCost)
            {
                currentCopies.push_back(FoundCopy(*verticesMapping, addedEdgesCost));
                findCopy(numCopies - 1, G, H, copyGG, currentCost, minCost, result, currentCopies);
                currentCopies.pop_back();
            }

            G.deleteEdges(addedEdges);
            currentCost -= addedEdgesCost;
        }
    }
}

std::tuple<std::vector<MultiEdge>, int> addMissingEdgesAndCalculateCost(Graph &G, Graph &H, const std::vector<int> &vertexMapping, int currentCost, int &minCost)
{
    int numVertices = H.size();
    int addedEdgesCost = 0;

    std::vector<MultiEdge> addedEdges;

    for (int i = 0; i < numVertices; i++)
    {
        std::vector<MultiEdge> patternEdges = H.getMultiEdges(i);

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
                return std::make_tuple(addedEdges, addedEdgesCost);
            }

            G.addEdge(mappedFrom, mappedTo, edgesToAdd);
            addedEdges.push_back(MultiEdge(mappedFrom, mappedTo, edgesToAdd));
            addedEdgesCost += edgesToAdd;
        }
    }

    return std::make_tuple(addedEdges, addedEdgesCost);
}