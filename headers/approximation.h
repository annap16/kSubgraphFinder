#pragma once

#include "graph.h"
#include "multiEdge.h"
#include <vector>
#include <tuple>

std::tuple<int, std::vector<std::vector<int>>> findCopiesApproximation(Graph &G, Graph &H, int numCopies);

std::vector<int> findDenseSubgraph(Graph &G, int m);
std::tuple<std::vector<int>, std::vector<MultiEdge>> findMatch(Graph &G, Graph &H, const std::vector<int> &denseSubgraph);
void pickAndRemoveVertex(Graph &G, const std::vector<int> &match);
