#pragma once

// this is only used as a return type in getMultiEdges – method of the Graph class
struct MultiEdge
{
    int from;
    int to;
    int multiplicity;

    MultiEdge(int f, int t, int m) : from(f), to(t), multiplicity(m) {}
};