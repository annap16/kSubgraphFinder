#pragma once

class Graph;

class GraphGenerator
{
    Graph *G;

public:
    GraphGenerator() {};
    GraphGenerator(Graph *G)
    {
        this->G = G;
    }
    ~GraphGenerator() {};
    Graph *next() {};
    GraphGenerator *createNew(Graph *S) {};
};