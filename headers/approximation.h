#pragma once

#include "graph.h"
#include "multiEdge.h"
#include "graphAugmentationResult.h"
#include <vector>
#include <tuple>

GraphAugmentationResult findCopiesApproximation(Graph &G, Graph &H, int numCopies);

std::vector<int> findDenseSubgraph(Graph &G, int m);

std::tuple<std::vector<int>, std::vector<MultiEdge>> findMatch(Graph &G, Graph &H, const std::vector<int> &denseSubgraph);

void pickAndRemoveVertex(Graph &G, const std::vector<int> &match);