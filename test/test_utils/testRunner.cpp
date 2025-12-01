#include "testRunner.h"

#include <iostream>
#include <set>

#include "../../headers/graphGenerator.h"
#include "../../headers/graph.h"
#include "../../headers/utils.h"
#include "../../headers/graphAugmentationResult.h"
#include "../../headers/inputParser.h"
#include "../../headers/resultWriter.h"

bool TestRunner::checkIsomorphicCopy(const Graph &GAugmented, const Graph &H, const std::vector<int> &map)
{
    int hSize = H.size();
    for (int i = 0; i < hSize; i++)
    {
        for (int j = 0; j < hSize; j++)
        {
            if (GAugmented.edgeCount(map[i], map[j]) != H.edgeCount(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

bool TestRunner::checkDistinctCopies(const std::vector<std::vector<int>> &copies)
{
    std::set<std::set<int>> used;

    for (const auto &mapping : copies)
    {
        // Convert mapping into a sorted set of vertices
        std::set<int> s(mapping.begin(), mapping.end());

        // Insert the vertex set, if insertion fails (second == false), it's a duplicate
        if (!used.insert(s).second)
        {
            return false;
        }
    }
    return true;
}

int TestRunner::countAddedEdges(const Graph &GOriginal, const Graph &GAugmented)
{
    int n = GOriginal.size();
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int diff = GAugmented.edgeCount(i, j) - GOriginal.edgeCount(i, j);
            if (diff < 0)
            {
                // Unexpected behaviour - edge deleted
                return -1;
            }
            sum += diff;
        }
    }
    return sum;
}

bool TestRunner::runTest(const fs::path &inputFile,
                         const fs::path &expectedFile,
                         const fs::path &outputFile)
{
    // Color constants
    constexpr auto COLOR_RESET = "\033[0m";
    constexpr auto COLOR_BLUE_BOLD = "\033[34;1m";
    constexpr auto COLOR_GREEN = "\033[32m";
    constexpr auto COLOR_RED = "\033[31m";

    std::cout << COLOR_BLUE_BOLD << "[TEST]" << COLOR_RESET << " Running: "
              << inputFile.filename() << std::endl;

    ParsedData data = parseInputFile(inputFile.string());
    if (!data.G || !data.H)
    {
        std::cerr << "  " << COLOR_RED << "[Error]" << COLOR_RESET
                  << " Failed to load input data.\n";
        return false;
    }

    Graph GOriginal = *data.G;
    Graph H = *data.H;

    GraphAugmentationResult result;
    GraphGenerator GG(GOriginal, H.size());
    int minCost = INT32_MAX;
    findCopy(data.numCopies, GOriginal, H, GG, 0, minCost, result);

    ResultWriter::saveToFile(outputFile.string(), result, data.numCopies);

    // Load expected results
    std::ifstream exp(expectedFile);
    if (!exp)
    {
        std::cerr << "  " << COLOR_RED << "[Error]" << COLOR_RESET
                  << " Cannot open expected result.\n";
        return false;
    }

    std::string description;
    std::getline(exp, description);
    if (!description.empty())
        std::cout << " - " << description;
    std::cout << std::endl;

    int expectedCost, expectedK;
    exp >> expectedCost;

    if (expectedCost != result.cost)
    {
        std::cerr << "  " << COLOR_RED << "[FAIL]" << COLOR_RESET
                  << " Wrong augmentation cost. Expected "
                  << expectedCost << " got " << result.cost << "\n";
        return false;
    }
    if (expectedCost == -1)
    {
        std::cout << "  " << COLOR_GREEN << "[PASS]" << COLOR_RESET << "\n";
        return true;
    }

    exp >> expectedK;

    if (expectedK != (int)result.foundCopies.size())
    {
        std::cerr << "  " << COLOR_RED << "[FAILED TEST SETUP]" << COLOR_RESET
                  << " Wrong number of copies.\n";
        return false;
    }

    std::vector<std::vector<int>> expectedMappings(expectedK);
    for (int i = 0; i < expectedK; i++)
    {
        expectedMappings[i].resize(H.size());
        for (int j = 0; j < H.size(); j++)
            exp >> expectedMappings[i][j];
    }

    // Validate each copy
    for (int i = 0; i < expectedK; i++)
    {
        if (!checkIsomorphicCopy(result.graphAugmentation, H,
                                 result.foundCopies[i].mapping))
        {
            std::cerr << "  " << COLOR_RED << "[FAIL]" << COLOR_RESET
                      << " Copy " << i << " is not isomorphic to H.\n";
            return false;
        }
    }

    // Validate distinctness of copies
    std::vector<std::vector<int>> maps;
    for (auto &c : result.foundCopies)
        maps.push_back(c.mapping);

    if (!checkDistinctCopies(maps))
    {
        std::cerr << "  " << COLOR_RED << "[FAIL]" << COLOR_RESET
                  << " Copies reuse the same vertices.\n";
        return false;
    }

    // Validate edge count
    int added = countAddedEdges(GOriginal, result.graphAugmentation);
    if (added == -1)
    {
        std::cerr << "  " << COLOR_RED << "[FAIL]" << COLOR_RESET
                  << " Unexpected behaviour - edge was deleted\n";
        return false;
    }
    else if (added != result.cost)
    {
        std::cerr << "  " << COLOR_RED << "[FAIL]" << COLOR_RESET
                  << " Added edges (" << added
                  << ") do not match augmentation cost (" << result.cost << ").\n";
        return false;
    }

    std::cout << "  " << COLOR_GREEN << "[PASS]" << COLOR_RESET << "\n";
    return true;
}
