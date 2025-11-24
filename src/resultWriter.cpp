#include "../headers/resultWriter.h"
#include <iostream>

void ResultWriter::writeGraph(std::ostream &out, const Graph &g)
{
    int n = g.size();
    out << n << "\n";

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            out << g.edgeCount(i, j);
            if (j + 1 < n)
                out << " ";
        }
        out << "\n";
    }
}

bool ResultWriter::saveToFile(const std::string filename,
                              const GraphAugmentationResult &result,
                              int k)
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        std::cerr << "[Error] Cannot write to file: " << filename << "\n";
        return false;
    }
    if (!result.solutionFound)
    {
        out << "inf\n";
        return true;
    }
    // Total augmentation cost
    out << result.cost << "\n";

    // The augmented graph
    writeGraph(out, result.graphAugmentation);

    // Number of copies k
    out << k << "\n";

    // All found copies
    for (const auto &copy : result.foundCopies)
    {

        for (size_t i = 0; i < copy.mapping.size(); ++i)
        {
            out << copy.mapping[i];
            if (i + 1 < copy.mapping.size())
                out << " ";
        }
        out << "\n";
    }

    return true;
}
