//
// Created by Miłosz Wysocki on 12/11/2025.
//

#pragma once
#include "graph.h"
#include <string>
#include <fstream>
#include <stdexcept>
#include <tuple>

struct ParsedData
{
    Graph *G;
    Graph *H;
    int numCopies;
};

ParsedData parseInputFile(const std::string &filename);