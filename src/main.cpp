#include <iostream>

#include "../headers/graphGenerator.h"
#include "../headers/graph.h"
#include "../headers/utils.h"
#include "../headers/graphAugmentationResult.h"

int main()
{
    // TODO: Parse input and get graph G, H and k from the input file
    Graph G, H;
    int numCopies = 2;

    GraphAugmentationResult result;
    GraphGenerator GG(G, H.size());

    findCopy(numCopies, G, H, GG, 0, INT32_MAX, result);

    // TODO: Save/Display RESULT and clean memory

    return 0;
}
