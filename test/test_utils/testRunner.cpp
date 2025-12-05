#include "testRunner.h"

#include <iostream>
#include <set>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>

#include "../../headers/graphGenerator.h"
#include "../../headers/graph.h"
#include "../../headers/utils.h"
#include "../../headers/graphAugmentationResult.h"
#include "../../headers/inputParser.h"
#include "../../headers/resultWriter.h"
#include "../../headers/approximation.h"

static std::string getFilename(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    return (pos == std::string::npos) ? path : path.substr(pos + 1);
}

static std::string getStem(const std::string &filename)
{
    size_t pos = filename.find_last_of('.');
    return (pos == std::string::npos) ? filename : filename.substr(0, pos);
}

static bool isRegularFile(const std::string &path)
{
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
        return false;
    return S_ISREG(statbuf.st_mode);
}

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

bool TestRunner::runTest(const std::string &inputFile,
                         const std::string &expectedFile,
                         const std::string &outputFile,
                         const bool isApproximation)
{
    std::cout << COLOR_BLUE_BOLD << "[TEST]" << COLOR_RESET << " Running: "
              << getFilename(inputFile) << std::endl;

    ParsedData data = parseInputFile(inputFile);
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

    ResultWriter::saveToFile(outputFile, result, data.numCopies);

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

std::tuple<int, int> TestRunner::runAllTests(const std::string &inputFolder,
                                             const std::string &expectedFolder,
                                             const std::string &outputFolder,
                                             const bool isApproximation)
{
    int total = 0;
    int passed = 0;

    DIR *dir = opendir(inputFolder.c_str());
    if (!dir)
    {
        std::cerr << COLOR_RED << "[Error]" << COLOR_RESET
                  << " Cannot open input folder: " << inputFolder << "\n";
        return std::make_tuple(0, 0);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        std::string inputFile = inputFolder + "/" + entry->d_name;

        if (!isRegularFile(inputFile))
            continue;

        std::string stem = getStem(entry->d_name);
        std::string outputFile = outputFolder + "/" + stem + ".result";
        std::string expectedFile = expectedFolder + "/" + stem + ".result";

        total++;
        if (TestRunner::runTest(inputFile, expectedFile, outputFile, isApproximation))
        {
            passed++;
        }
    }

    closedir(dir);
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