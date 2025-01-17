#include "show_graph.h"

#include <iostream>



void ShowGraph(const std::string& dotFilename, const std::string& outputFilename)
{
    std::string command = "dot -Tpng " + dotFilename + " -o " + outputFilename;
    system(command.c_str());
}