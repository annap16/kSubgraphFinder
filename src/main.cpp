#include <iostream>

#include "../headers/graphGenerator.h"
#include "../headers/graph.h"
#include "../headers/utils.h"

Graph *RESULT = nullptr;
int cost = -1;

int main()
{
    // TODO: Parse input and get graph G, H and k from the input file
    Graph *G = new Graph();
    Graph *H = new Graph();
    int numCopies = 2;
    RESULT = new Graph();

    findCopies(*G, *H, numCopies);

    // TODO: Save/Display RESULT and clean memory
    delete RESULT;
    delete G;
    delete H;

    return 0;
}
