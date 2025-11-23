#pragma once

#include <vector>

#include "multiEdge.h"
#include "foundCopy.h"

class Graph;
class GraphGenerator;
class GraphAugmentationResult;

void findCopy(int numCopies, Graph &G, Graph &H, GraphGenerator &GG, int currentCost, int &minCost, GraphAugmentationResult &Result, std::vector<FoundCopy> currentCopies = {});
std::tuple<std::vector<MultiEdge>, int> addMissingEdgesAndCalculateCost(Graph &G, Graph &H, const std::vector<int> &vertexMapping, int currentCost, int &minCost);