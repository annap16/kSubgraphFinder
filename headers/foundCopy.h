#pragma once

#include <vector>

struct FoundCopy
{
    std::vector<int> subsetG;
    std::vector<int> mapping;
    int cost;

    FoundCopy(std::vector<int> &s, std::vector<int> &m, int c) : subsetG(s), mapping(m), cost(c) {}
};