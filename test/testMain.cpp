#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <set>

#include "./test_utils/testRunner.h"

int main()
{
    fs::path inputFolderExactAlgorithm = "test/exact_algorithm/input_data";
    fs::path expectedFolderExactAlgorithm = "test/exact_algorithm/expected_results";
    fs::path outputFolderExactAlgorithm = "test/exact_algorithm/output_results";

    fs::path inputFolderApproximationAlgorithm = "test/approximation_algorithm/input_data";
    fs::path expectedFolderApproximationAlgorithm = "test/approximation_algorithm/expected_results";
    fs::path outputFolderApproximationAlgorithm = "test/approximation_algorithm/output_results";

    fs::create_directories(outputFolderExactAlgorithm);
    fs::create_directories(outputFolderApproximationAlgorithm);

    std::cout << TestRunner::COLOR_BLUE_BOLD << "\nStarted tests for Exact Algorithm:\n"
              << TestRunner::COLOR_RESET;
    auto [passedExact, totalExact] = TestRunner::runAllTests(inputFolderExactAlgorithm, expectedFolderExactAlgorithm, outputFolderExactAlgorithm, false);

    std::cout << TestRunner::COLOR_BLUE_BOLD << "\nStarted tests for Approximation Algorithm:\n"
              << TestRunner::COLOR_RESET;
    auto [passedApproximation, totalApproximation] = TestRunner::runAllTests(inputFolderApproximationAlgorithm, expectedFolderApproximationAlgorithm, outputFolderApproximationAlgorithm, true);

    std::cout << "\nResults for Exact Algorithm:\n";
    TestRunner::showTotalResults(passedExact, totalExact);

    std::cout << "\nResults for Approximation Algorithm:\n";
    TestRunner::showTotalResults(passedApproximation, totalApproximation);

    return 0;
}