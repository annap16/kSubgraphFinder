#include <iostream>

#include "../headers/graphGenerator.h"
#include "../headers/graph.h"
#include "../headers/utils.h"
#include "../headers/graphAugmentationResult.h"
#include "../headers/inputParser.h"
#include "../headers/resultWriter.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_input_file> <path_to_output_file>\n";
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

    GraphAugmentationResult result;
    GraphGenerator GG(G, H.size());
    int minCost = INT32_MAX;

    findCopy(numCopies, G, H, GG, 0, minCost, result);

    if (!ResultWriter::saveToFile(argv[2], result, numCopies))
    {
        std::cerr << "[Error] Cannot save result.\n";
        return 1;
    }

    return 0;
}
