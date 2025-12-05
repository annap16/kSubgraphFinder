#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sys/stat.h>
#include <sys/types.h>
#include "./test_utils/testRunner.h"

int main()
{
    std::string inputFolderExactAlgorithm = "test/exact_algorithm/input_data";
    std::string expectedFolderExactAlgorithm = "test/exact_algorithm/expected_results";
    std::string outputFolderExactAlgorithm = "test/exact_algorithm/output_results";

    std::string inputFolderApproximationAlgorithm = "test/approximation_algorithm/input_data";
    std::string expectedFolderApproximationAlgorithm = "test/approximation_algorithm/expected_results";
    std::string outputFolderApproximationAlgorithm = "test/approximation_algorithm/output_results";

    std::cout << TestRunner::COLOR_BLUE_BOLD << "\nStarted tests for Exact Algorithm:\n"
              << TestRunner::COLOR_RESET;
    auto [passedExact, totalExact] = TestRunner::runAllTests(
        inputFolderExactAlgorithm,
        expectedFolderExactAlgorithm,
        outputFolderExactAlgorithm,
        false);

    std::cout << TestRunner::COLOR_BLUE_BOLD << "\nStarted tests for Approximation Algorithm:\n"
              << TestRunner::COLOR_RESET;
    auto [passedApproximation, totalApproximation] = TestRunner::runAllTests(
        inputFolderApproximationAlgorithm,
        expectedFolderApproximationAlgorithm,
        outputFolderApproximationAlgorithm,
        true);

    std::cout << "\nResults for Exact Algorithm:\n";
    TestRunner::showTotalResults(passedExact, totalExact);

    std::cout << "\nResults for Approximation Algorithm:\n";
    TestRunner::showTotalResults(passedApproximation, totalApproximation);

    return 0;
}