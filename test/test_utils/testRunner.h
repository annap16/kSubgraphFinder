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
    static bool runTest(const fs::path &inputFile,
                        const fs::path &expectedFile,
                        const fs::path &outputFile,
                        const bool isApproximation);

public:
    static constexpr const char *COLOR_RESET = "\033[0m";
    static constexpr const char *COLOR_BLUE_BOLD = "\033[34;1m";
    static constexpr const char *COLOR_GREEN = "\033[32m";
    static constexpr const char *COLOR_RED = "\033[31m";

    static std::tuple<int, int> runAllTests(const fs::path &inputFolder,
                                            const fs::path &expectedFolder,
                                            const fs::path &outputFolder,
                                            const bool isApproximation);
    static void showTotalResults(int passed, int total);
};
