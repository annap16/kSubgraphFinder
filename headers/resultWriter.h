#pragma once

#include <fstream>
#include <memory>
#include "graph.h"
#include "graphAugmentationResult.h"

class ResultWriter
{
    static void writeGraph(std::ostream &out, const Graph &g);

public:
    static bool saveToFile(const std::string filename,
                           const GraphAugmentationResult &result,
                           int k);
};
