#pragma once

#include "../../headers/graph.h"

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class TestRunner
{
    static bool checkIsomorphicCopy(const Graph &GAugmented, const Graph &H, const std::vector<int> &map);
    static bool checkDistinctCopies(const std::vector<std::vector<int>> &copies);
    static int countAddedEdges(const Graph &GOriginal, const Graph &GAugmented);

public:
    static bool runTest(const fs::path &inputFile,
                        const fs::path &expectedFile,
                        const fs::path &outputFile);
};
