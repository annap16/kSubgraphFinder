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
    std::cout << "[TEST] Running: " << inputFile.filename() << std::endl;

    ParsedData data = parseInputFile(inputFile.string());
    if (!data.G || !data.H)
    {
        std::cerr << "  [Error] Failed to load input data.\n";
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
        std::cerr << "  [Error] Cannot open expected result.\n";
        return false;
    }

    int expectedCost, expectedK;
    exp >> expectedCost >> expectedK;

    if (expectedCost != result.cost)
    {
        std::cerr << "  [FAIL] Wrong augmentation cost. Expected "
                  << expectedCost << " got " << result.cost << "\n";
        return false;
    }

    if (expectedK != (int)result.foundCopies.size())
    {
        std::cerr << "  [FAILED TEST SETUP] Wrong number of copies.\n";
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
            std::cerr << "  [FAIL] Copy " << i << " is not isomorphic to H.\n";
            return false;
        }
    }

    // Validate distinctness of copies
    std::vector<std::vector<int>> maps;
    for (auto &c : result.foundCopies)
        maps.push_back(c.mapping);

    if (!checkDistinctCopies(maps))
    {
        std::cerr << "  [FAIL] Copies reuse the same vertices.\n";
        return false;
    }

    // Validate edge count
    int added = countAddedEdges(GOriginal, result.graphAugmentation);
    if (added == -1)
    {
        std::cerr << " [FAIL] Unexpected behaviour - edge was deleted";
    }
    else if (added != result.cost)
    {
        std::cerr << "  [FAIL] Added edges (" << added
                  << ") do not match augmentation cost (" << result.cost << ").\n";
        return false;
    }

    std::cout << "  [PASS]\n";
    return true;
}