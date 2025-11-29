#include "testRunner.h"

#include <iostream>
#include <set>

#include "../../headers/graphGenerator.h"
#include "../../headers/graph.h"
#include "../../headers/utils.h"
#include "../../headers/graphAugmentationResult.h"
#include "../../headers/inputParser.h"
#include "../../headers/resultWriter.h"
#include "../../headers/approximation.h"

bool TestRunner::checkIsomorphicCopy(const Graph &GAugmented, const Graph &H, const std::vector<int> &map)
{
    int hSize = H.size();
    for (int i = 0; i < hSize; i++)
    {
        for (int j = 0; j < hSize; j++)
        {
            if (GAugmented.edgeCount(map[i], map[j]) < H.edgeCount(i, j))
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
                         const fs::path &outputFile,
                         const bool isApproximation)
{
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

    if (!isApproximation)
    {
        findCopy(data.numCopies, GOriginal, H, GG, 0, minCost, result);
    }
    else
    {
        result = findCopiesApproximation(GOriginal, H, data.numCopies);
    }

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

    if (!isApproximation)
    {
        int expectedCost;
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
    }

    int expectedK;
    exp >> expectedK;

    if (isApproximation && expectedK == -1)
    {
        if (expectedK == result.cost)
        {
            std::cout << "  " << COLOR_GREEN << "[PASS]" << COLOR_RESET << "\n";
            return true;
        }
        else
        {
            std::cerr << "  " << COLOR_RED << "[FAIL]" << COLOR_RESET
                      << "Augmentation found, but it is impossible for the given input\n";

            return false;
        }
    }

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

std::tuple<int, int> TestRunner::runAllTests(const fs::path &inputFolder,
                                             const fs::path &expectedFolder,
                                             const fs::path &outputFolder,
                                             const bool isApproximation)
{
    int total = 0;
    int passed = 0;

    for (const auto &entry : fs::directory_iterator(inputFolder))
    {
        if (!entry.is_regular_file())
            continue;

        fs::path inputFile = entry.path();
        fs::path outputFile = outputFolder / (inputFile.stem().string() + ".result");
        fs::path expectedFile = expectedFolder / (inputFile.stem().string() + ".result");

        total++;
        if (TestRunner::runTest(inputFile, expectedFile, outputFile, isApproximation))
        {
            passed++;
        }
    }

    return std::make_tuple(passed, total);
}

void TestRunner::showTotalResults(int passed, int total)
{
    if (passed == total)
    {
        std::cout << TestRunner::COLOR_GREEN << "\tTest summary: " << passed << " / " << total << " passed." << TestRunner::COLOR_RESET << "\n";
    }
    else
    {
        std::cout << TestRunner::COLOR_RED << "\tTest summary: " << passed << " / " << total << " passed." << TestRunner::COLOR_RESET << "\n";
    }
}
