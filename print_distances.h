#pragma once

#include <iostream>

#include "undirected_graph.h"
#include "dynamic_array.h";



template <typename TKey>
void PrintGraphDistances(const UndirectedGraph<TKey>& graph, DynamicArray<int>& distances, std::ostream& os)
{
    for (int i = 0; i < graph.GetVertexCount(); i++)
    {
        TKey vertex = graph.GetVertex(i);
        os << "Minimum distance to vertex ";
        os << vertex;
        os << ": ";

        if (distances.GetElement(i) == std::numeric_limits<int>::max())
        {
            os << "infinity\n";
        }
        else
        {
            os << distances.GetElement(i);
            os << "\n";
        }
    }
}