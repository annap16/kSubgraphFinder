#pragma once

#include "graph.h"
#include "foundCopy.h"

struct GraphAugmentationResult
{
    Graph graphAugmentation;
    int cost;
    std::vector<FoundCopy> foundCopies;
};
