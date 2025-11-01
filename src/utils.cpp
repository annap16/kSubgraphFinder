#include "../headers/utils.h"
#include "../headers/graph.h"
#include "../headers/graphGenerator.h"
#include "../headers/graphMappingIterator.h"

#include <cmath>
#include <map>

int verticesNeededForCopies(int numCopies, int subgraphSize, int graphSize)
{
    long double logCurrentCombination = std::log(subgraphSize + 1);
    long double logNumCopies = std::log(numCopies);
    int additionalVertices = 2;

    while (logCurrentCombination < logNumCopies)
    {
        logCurrentCombination += std::log(subgraphSize + additionalVertices);
        additionalVertices++;
    }

    if (additionalVertices + subgraphSize < graphSize)
        return 0;
    return additionalVertices + subgraphSize - graphSize;
}

void findCopy(int numCopies, Graph *G, Graph *H, GraphGenerator *GG, int currentCost, int minCost)
{
    if (numCopies == 0)
    {
        *RESULT = G->copy();
        return;
    }

    Graph *S = GG->next();
    GraphGenerator *copyGG = GG->createNew(S);
    GraphMappingIterator *graphMappingIterator = new GraphMappingIterator(H, S);

    while (graphMappingIterator->hasNext())
    {
        Graph *copyG = G->createNew();
        int *verticiesMapping = graphMappingIterator->next();

        currentCost = addMissingEdgesAndCalculateCost(copyG, H, S, verticiesMapping, currentCost, minCost);
        if (currentCost < minCost)
        {
            findCopy(numCopies - 1, copyG, H, copyGG, currentCost, minCost);
        }

        delete copyG;
        delete[] verticiesMapping;
    }

    delete graphMappingIterator;
    delete copyGG;

    findCopy(numCopies, G, H, copyGG, currentCost, minCost);
}

int addMissingEdgesAndCalculateCost(Graph *G, Graph *H, Graph *S, int *vertexMapping, int currentCost, int minCost)
{
    int numVertices = H->size();

    for (int i = 0; i < numVertices; i++)
    {
        std::map<int, int> patternEdges = H->getEdgesWithMultiplicity(i);

        for (const auto &[neighbor, multiplicity] : patternEdges)
        {
            int mappedFrom = vertexMapping[i];
            int mappedTo = vertexMapping[neighbor];

            int currentMultiplicity = S->edgeCount(mappedFrom, mappedTo);

            int edgesToAdd = multiplicity - currentMultiplicity;
            if (edgesToAdd <= 0)
                continue;

            if (currentCost + edgesToAdd >= minCost)
            {
                return minCost;
            }

            G->addEdge(mappedFrom, mappedTo, edgesToAdd);
            currentCost += edgesToAdd;
        }
    }

    return currentCost;
}
