#pragma once

#include "graph.h"
#include "foundCopy.h"

struct GraphAugmentationResult
{
    Graph graphAugmentation;
    int cost = -1;
    std::vector<FoundCopy> foundCopies;
};
