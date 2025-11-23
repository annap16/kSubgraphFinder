#include <iostream>

#include "../headers/graphGenerator.h"
#include "../headers/graph.h"
#include "../headers/utils.h"
#include "../headers/graphAugmentationResult.h"
#include "../headers/inputParser.h"

Graph *RESULT = nullptr;
int cost = -1;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_input_file>\n";
        return 1;
    }

    ParsedData data = parseInputFile(argv[1]);
    if (!data.G || !data.H)
    {
        std::cerr << "Failed to load input data.\n";
        return 1;
    }

    Graph G = *data.G;
    Graph H = *data.H;
    int numCopies = data.numCopies;
    RESULT = new Graph();

    GraphAugmentationResult result;
    GraphGenerator GG(G, H.size());

    findCopy(numCopies, G, H, GG, 0, INT32_MAX, result);

    // TODO: Save/Display RESULT and clean memory

    return 0;
}
