#pragma once

class Graph;

class GraphMappingIterator
{
    Graph *H;
    Graph *S;

public:
    GraphMappingIterator() {};
    GraphMappingIterator(Graph *H, Graph *S)
    {
        this->H = H;
        this->S = S;
    }
    ~GraphMappingIterator() {};
    bool hasNext() {};
    int *next() {};
};