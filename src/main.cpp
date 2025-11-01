#include <iostream>

#include "../headers/graphGenerator.h"
#include "../headers/graph.h"
#include "../headers/utils.h"

Graph *RESULT = nullptr;

int main()
{
    // TODO: Parse input and get graph G, H and k from the input file
    Graph *G = new Graph();
    Graph *H = new Graph();
    int numCopies = 2;
    int subgraphSize = H->size();
    int graphSize = G->size();
    RESULT = new Graph();

    GraphGenerator *GG = new GraphGenerator(G);

    int additionalVerticies = verticesNeededForCopies(numCopies, subgraphSize, graphSize);
    if (additionalVerticies != 0)
    {
        G->addIsolatedVertices(additionalVerticies);
    }

    findCopy(numCopies, G, H, GG, 0, INT32_MAX);

    // TODO: Save/Display RESULT and clean memory
    delete RESULT;
    delete G;
    delete H;

    return 0;
}
