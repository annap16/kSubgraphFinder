#pragma once

#include <vector>

struct FoundCopy
{
    std::vector<int> mapping;
    int cost;

    FoundCopy(std::vector<int> &m, int c) : mapping(m), cost(c) {}
};