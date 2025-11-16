#pragma once

#include <optional>
#include <vector>

class Graph;

class GraphMappingIterator
{
    Graph &H;
    std::vector<int> &verticesSubset;
    std::vector<int> cur;
    std::vector<int> dir;
    int n;
    bool finished = false;
    bool first = true;

    int largestMobileIndex() const;
    std::vector<int> curToVerticesSubset() const;

public:
    GraphMappingIterator(Graph &H, std::vector<int> &verticesSubset)
        : H(H), verticesSubset(verticesSubset)
    {
        n = H.size();
        cur = std::vector<int>(n);
        dir = std::vector<int>(n);
        for (int i = 0; i < n; i++)
        {
            cur[i] = i;
            dir[i] = -1;
        }
    }

    ~GraphMappingIterator() {};
    std::optional<std::vector<int>> next() {};
};
