#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "../../headers/graphGenerator.h"
#include "../../headers/graph.h"
#include "../../headers/utils.h"
#include "../../headers/graphAugmentationResult.h"
#include "../../headers/inputParser.h"
#include "../../headers/resultWriter.h"

namespace fs = std::filesystem;

// Run a single test
bool runTest(const fs::path &inputFile, const fs::path &expectedFile, const fs::path &outputFile)
{
    std::cout << "[TEST] Running: " << inputFile.filename() << std::endl;

    ParsedData data = parseInputFile(inputFile.string());
    if (!data.G || !data.H)
    {
        std::cerr << "  [Error] Failed to load input data.\n";
        return false;
    }

    Graph G = *data.G;
    Graph H = *data.H;
    int numCopies = data.numCopies;

    GraphAugmentationResult result;
    GraphGenerator GG(G, H.size());
    int minCost = INT32_MAX;

    findCopy(numCopies, G, H, GG, 0, minCost, result);

    if (!ResultWriter::saveToFile(outputFile.string(), result, numCopies))
    {
        std::cerr << "  [Error] Cannot save result.\n";
        return false;
    }

    // Compare with expected output
    std::ifstream outFile(outputFile);
    std::ifstream expFile(expectedFile);
    if (!outFile || !expFile)
    {
        std::cerr << "  [Error] Cannot open output/expected files.\n";
        return false;
    }

    std::string outLine, expLine;
    int lineNum = 0;
    while (std::getline(outFile, outLine) && std::getline(expFile, expLine))
    {
        lineNum++;
        if (outLine != expLine)
        {
            std::cerr << "  [FAIL] Line " << lineNum << " differs.\n";
            return false;
        }
    }

    if ((std::getline(outFile, outLine) && !outFile.eof()) ||
        (std::getline(expFile, expLine) && !expFile.eof()))
    {
        std::cerr << "  [FAIL] File lengths differ.\n";
        return false;
    }

    std::cout << "  [PASS]\n";
    return true;
}

int main()
{
    fs::path inputFolder = "test/input_data";
    fs::path expectedFolder = "test/exact_algorithm/expected_results";
    fs::path outputFolder = "test/exact_algorithm/output_results";

    fs::create_directories(outputFolder);

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
        if (runTest(inputFile, expectedFile, outputFile))
        {
            passed++;
        }
    }

    std::cout << "\nTest summary: " << passed << " / " << total << " passed.\n";
    return 0;
}
