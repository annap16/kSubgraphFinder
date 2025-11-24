#pragma once

#include "graph.h"
#include "foundCopy.h"

struct GraphAugmentationResult
{
    bool solutionFound;
    Graph graphAugmentation;
    int cost;
    std::vector<FoundCopy> foundCopies;
};
