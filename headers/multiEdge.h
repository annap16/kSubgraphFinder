#pragma once

struct MultiEdge
{
    int from;
    int to;
    int multiplicity;

    MultiEdge(int f, int t, int m) : from(f), to(t), multiplicity(m) {}
};