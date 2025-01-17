#pragma once

#include <iostream>

#include "undirected_graph.h"
#include "dynamic_array.h"



template <typename TKey>
void PrintGraphColor(const UndirectedGraph<TKey>& graph, DynamicArray<int>& colors, std::ostream& os)
{
    for (int i = 0; i < graph.GetVertexCount(); i++)
    {
        TKey vertex = graph.GetVertex(i);
        os << "Vertex ";
        os << vertex;
        os << " ---> Color ";
        os << colors.GetElement(i);
        os << "\n";
    }
}