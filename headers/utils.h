#pragma once

#include <vector>

class Graph;
class GraphGenerator;
class MultiEdge;

extern Graph *RESULT;
extern int cost;

int verticesNeededForCopies(int numCopies, int subgraphSize, int graphSize);
void findCopy(int numCopies, Graph &G, Graph &H, GraphGenerator &GG, int currentCost, int minCost);
std::vector<MultiEdge> addMissingEdgesAndCalculateCost(Graph &G, Graph &H, const std::vector<int> &vertexMapping, int &currentCost, int minCost);
void findCopies(Graph &G, Graph &H, int numCopies);