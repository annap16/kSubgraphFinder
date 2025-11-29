#pragma once

#include "graph.h"
#include "multiEdge.h"
#include "graphAugmentationResult.h"
#include <vector>
#include <tuple>

GraphAugmentationResult findCopiesApproximation(Graph &GOriginal, Graph &H, int numCopies);

std::vector<int> findDenseSubgraph(Graph &G, int m, const std::vector<bool> &deleted_vertices);
std::tuple<std::vector<int>, std::vector<MultiEdge>> findMatch(Graph &G, Graph &H, const std::vector<int> &denseSubgraph, const std::vector<bool> &deleted_vertices);
void pickAndRemoveVertex(Graph &G, const std::vector<int> &match, std::vector<bool> &deleted_vertices);