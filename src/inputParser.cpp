#include "../headers/inputParser.h"
#include <iostream>
#include <sstream>

static Graph *readGraphFromStream(std::istream &in)
{
    int n;
    if (!(in >> n))
    {
        std::cerr << "[Błąd] Nie można odczytać liczby wierzchołków.\n";
        return nullptr;
    }

    Graph *g = new Graph();
    g->addIsolatedVertices(n);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int multiplicity;
            if (!(in >> multiplicity))
            {
                std::cerr << "[Błąd] Niepoprawna macierz sąsiedztwa.\n";
                delete g;
                return nullptr;
            }

            if (multiplicity > 0)
                g->addEdge(i, j, multiplicity);
        }
    }

    return g;
}

ParsedData parseInputFile(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cerr << "[Błąd] Nie można otworzyć pliku: " << filename << "\n";
        return {nullptr, nullptr, 0};
    }

    ParsedData data{};

    data.G = readGraphFromStream(in);
    if (!data.G)
        return {nullptr, nullptr, 0};

    data.H = readGraphFromStream(in);
    if (!data.H)
    {
        delete data.G;
        return {nullptr, nullptr, 0};
    }

    if (!(in >> data.numCopies))
    {
        std::cerr << "[Ostrzeżenie] Brak wartości numCopies — przyjęto 1.\n";
        data.numCopies = 1;
    }

    return data;
}
