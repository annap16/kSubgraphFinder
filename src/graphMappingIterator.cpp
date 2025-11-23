#include "../headers/graphMappingIterator.h"

/**
 * Returns vector, on which i-th possition is a vertex from verties subset
 * assigned to i-th vertex of graph H.
 */
std::optional<std::vector<int>> GraphMappingIterator::next()
{
    if (finished)
        return {};

    if (first)
    {
        first = false;
        return curToVerticesSubset();
    }

    int lm = largestMobileIndex();
    if (lm == -1)
    {
        finished = true;
        return std::nullopt;
    }
    int moved_val = cur[lm];
    int swap_with = lm + dir[lm];
    std::swap(cur[lm], cur[swap_with]);
    std::swap(dir[lm], dir[swap_with]);

    for (int i = 0; i < n; ++i)
    {
        if (cur[i] > moved_val)
            dir[i] = -dir[i];
    }

    return curToVerticesSubset();
}

int GraphMappingIterator::largestMobileIndex() const
{
    int idx = -1;
    int best_val = -1;
    for (int i = 0; i < n; ++i)
    {
        int d = dir[i];
        int ni = i + d;
        if (ni < 0 || ni >= n)
            continue;
        if (cur[i] > cur[ni] && cur[i] > best_val)
        {
            best_val = cur[i];
            idx = i;
        }
    }
    return idx;
}

std::vector<int> GraphMappingIterator::curToVerticesSubset() const
{
    std::vector<int> result(n);
    for (int i = 0; i < n; i++)
    {
        result[i] = verticesSubset[cur[i]];
    }
    return result;
}