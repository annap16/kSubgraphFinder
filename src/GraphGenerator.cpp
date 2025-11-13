#include "GraphGenerator.h"

GraphGenerator::GraphGenerator(Graph& graph, int k)
    : G(graph), k(k == -1 ? graph.size() : k), finished(false)
{
    if (this->k > G.size() || this->k <= 0)
        finished = true;
    else
        current = (1ULL << this->k) - 1;
}

//Gosper’s algorithm (O(1)) - https://read.seas.harvard.edu/~kohler/class/cs207-s12/lec12.html
uint64_t GraphGenerator::next_combination(uint64_t x) {
    uint64_t y = x & -x;
    uint64_t c = x + y;
    return (((x ^ c) >> 2) / y) | c;
}

//generating vec of numbers from mask (O(n)) - not sure if we prefer to convert it here or returning the mask and using it in the main prog
// but probably converting here is easier to understand
std::optional<std::vector<int>> GraphGenerator::next() {
    if (finished)
        return std::nullopt;

    std::vector<int> subset;
    for (int i = 0; i < G.size(); ++i)
        if (current & (1ULL << i))
            subset.push_back(i);

    uint64_t next_mask = next_combination(current);
    if (next_mask >= (1ULL << G.size()) || next_mask == 0)
        finished = true;
    else
        current = next_mask;

    return subset;
}
