#include <iostream>
#include <cstdint>
#include <string>

#include "../headers/graphGenerator.h"
#include "../headers/graph.h"
#include "../headers/utils.h"
#include "../headers/graphAugmentationResult.h"
#include "../headers/inputParser.h"
#include "../headers/resultWriter.h"
#include "../headers/approximation.h"

enum modeType
{
    APPROXIMATION,
    EXACT
};

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << "<mode: a|d> <path_to_input_file> <path_to_output_file>\n";
        return 1;
    }
    std::string mode_str = argv[1];
    modeType mode;
    if (mode_str == "a")
        mode = APPROXIMATION;
    else if (mode_str == "d")
        mode = EXACT;
    else
    {
        std::cerr << "Invalid mode. Use 'a' for approximation or 'd' for exact.\n";
        return 1;
    }

    ParsedData data = parseInputFile(argv[2]);
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

    switch (mode)
    {
    case EXACT:
        findCopy(numCopies, G, H, GG, 0, minCost, result);
        break;

    case APPROXIMATION:
        result = findCopiesApproximation(G, H, numCopies);
        break;
    }

    if (!ResultWriter::saveToFile(argv[3], result, numCopies))
    {
        std::cerr << "[Error] Cannot save result.\n";
        return 1;
    }

    return 0;
}