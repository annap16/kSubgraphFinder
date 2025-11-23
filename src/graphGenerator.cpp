#include "graphGenerator.h"

// Source: https://cp-algorithms.com/combinatorics/generating_combinations.html
// Lexicographic generation of k-combinations out of n elements.

/**
 * @brief Constructs a GraphGenerator that iterates over all k-subsets of vertices.
 *
 * If k == -1, then k defaults to the size of the graph (i.e. one combination of all vertices).
 * If k is invalid (k <= 0 or k > n), the generator is marked as finished immediately.
 *
 * @param graph Reference to the graph whose vertex indices will be used.
 * @param k     Size of the subsets to generate (optional, default: -1).
 */
GraphGenerator::GraphGenerator(Graph &graph, int k)
    : G(graph), k(k == -1 ? graph.size() : k), finished(false)
{
    int n = G.size();

    // Validate k
    if (this->k > n || this->k <= 0)
    {
        finished = true;
        return;
    }

    // First combination in lexicographic order: [0, 1, 2, ..., k-1]
    comb.resize(this->k);
    for (int i = 0; i < this->k; ++i)
        comb[i] = i;
}

/**
 * @brief Computes the next k-combination in lexicographic order.
 *
 * This follows the classical "successor combination" algorithm:
 * - Find the rightmost position i such that c[i] can be increased without violating lexicographic rules.
 * - Increase c[i].
 * - Set all positions to the right of i to the minimal valid values.
 *
 * Complexity: O(k)
 *
 * @param c Vector storing the current combination.
 * @param n Number of available elements (0..n-1).
 * @param k Size of each combination.
 * @return true if the next combination exists, false if this was the last one.
 */
bool GraphGenerator::next_combination(std::vector<int> &c, int n, int k)
{
    for (int i = k - 1; i >= 0; --i)
    {
        // Can we increment c[i] while leaving room for the remaining elements?
        if (c[i] < n - (k - i))
        {
            c[i]++;

            // Fill the remaining positions with the next sequential numbers
            for (int j = i + 1; j < k; ++j)
                c[j] = c[j - 1] + 1;

            return true;
        }
    }

    // No further combination exists
    return false;
}

/**
 * @brief Returns the next k-subset of vertex indices.
 *
 * The returned subset is a lexicographically ordered vector of indices.
 * When all combinations are exhausted, the function returns std::nullopt.
 *
 * @return std::optional<std::vector<int>>
 *         - The next subset of size k
 *         - std::nullopt if no more combinations exist
 */
std::optional<std::vector<int>> GraphGenerator::next()
{
    if (finished)
        return std::nullopt;

    // Return a copy of the current combination
    std::vector<int> subset = comb;

    // Move to the next combination
    if (!next_combination(comb, G.size(), k))
        finished = true;

    return subset;
}

GraphGenerator GraphGenerator::copy()
{
    GraphGenerator gg(G, k);
    gg.finished = this->finished;
    gg.comb = this->comb;
    return gg;
}
