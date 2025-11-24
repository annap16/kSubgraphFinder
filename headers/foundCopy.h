#pragma once

#include <vector>

struct FoundCopy
{
    std::vector<int> mapping;

    FoundCopy(std::vector<int> &m) : mapping(m) {}
};