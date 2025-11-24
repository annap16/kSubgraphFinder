#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <set>

#include "./test_utils/testRunner.h"

int main()
{
    fs::path inputFolder = "test/exact_algorithm/input_data";
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
        if (TestRunner::runTest(inputFile, expectedFile, outputFile))
        {
            passed++;
        }
    }

    std::cout << "\nTest summary: " << passed << " / " << total << " passed.\n";
    return 0;
}