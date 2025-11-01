#pragma once

class Graph;
class GraphGenerator;

extern Graph *RESULT;

int verticesNeededForCopies(int numCopies, int subgraphSize, int graphSize);
void findCopy(int numCopies, Graph *G, Graph *H, GraphGenerator *GG, int currentCost, int minCost);
int addMissingEdgesAndCalculateCost(Graph *G, Graph *H, Graph *S, int *vertexMapping, int currentCost, int minCost);
