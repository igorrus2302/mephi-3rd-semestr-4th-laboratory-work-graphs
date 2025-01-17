#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <set>

#include "undirected_graph.h"
#include "dynamic_array.h"



template <typename TValue>
void SaveGraphToDot(const UndirectedGraph<TValue>& graph, const std::string& filename)
{
    std::ofstream dotFile(filename);

    if (!dotFile) {
        std::cerr << "Error opening file for writing\n";
        return;
    }

    dotFile << "graph G {\n";

    for (int i = 0; i < graph.GetVertexCount(); ++i)
    {
        TValue vertex = graph.GetVertex(i);
        dotFile << "  \"" << vertex << "\";\n";

        auto adjacentVertices = graph.GetAdjacentVertices(vertex);

        for (int j = 0; j < adjacentVertices.GetLength(); ++j)
        {
            TValue adjacentVertex = adjacentVertices[j].vertex;
            int weight = adjacentVertices[j].weight;

            if (vertex < adjacentVertex)
            {
                dotFile << "  \"" << vertex << "\" -- \"" << adjacentVertex << "\" [label=\"" << weight << "\"];\n";
            }
        }
    }

    dotFile << "}\n";

    dotFile.close();
    std::cout << "Graph has been successfully saved to " << filename << " in DOT format.\n";
}

template <typename TValue>
void SaveColoredGraphToDot(const UndirectedGraph<TValue>& graph, const DynamicArray<int>& colors, const std::string& filename)
{
    std::ofstream dotFile(filename);

    if (!dotFile)
    {
        std::cerr << "Error opening file for writing\n";
        return;
    }

    dotFile << "graph G {\n";

    const std::vector<std::string> colorPalette ={
            "red", "green", "blue", "yellow", "cyan", "magenta", "orange", "pink", "purple", "brown",
            "lime", "teal", "navy", "olive", "maroon", "gray", "black", "gold", "silver", "beige"
    };

    for (int i = 0; i < graph.GetVertexCount(); ++i)
    {
        TValue vertex = graph.GetVertex(i);
        int colorIndex = colors[i];
        std::string vertexColor = (colorIndex >= 0 && colorIndex < colorPalette.size()) ? colorPalette[colorIndex] : "white";
        dotFile << "  \"" << vertex << "\" [style=filled, fillcolor=\"" << vertexColor << "\"]\n";
    }

    for (int i = 0; i < graph.GetVertexCount(); ++i)
    {
        TValue vertex = graph.GetVertex(i);
        auto adjacentVertices = graph.GetAdjacentVertices(vertex);

        for (int j = 0; j < adjacentVertices.GetLength(); ++j)
        {
            TValue adjacentVertex = adjacentVertices[j].vertex;
            int weight = adjacentVertices[j].weight;

            if (vertex < adjacentVertex)
            {
                dotFile << "  \"" << vertex << "\" -- \"" << adjacentVertex << "\" [label=\"" << weight << "\"]\n";
            }
        }
    }

    dotFile << "}\n";

    dotFile.close();
    std::cout << "Colored graph has been successfully saved to " << filename << " in DOT format.\n";
}

template <typename TValue>
void SaveGraphWithMSTToDot(const UndirectedGraph<TValue>& graph, const DynamicArray<Edge>& mst, const std::string& filename)
{
    std::ofstream dotFile(filename);

    if (!dotFile)
    {
        std::cerr << "Error opening file for writing\n";
        return;
    }

    dotFile << "graph G {\n";

    std::set<std::pair<TValue, TValue>> mstEdges;
    for (int i = 0; i < mst.GetLength(); ++i)
    {
        TValue u = mst[i].vertex;
        int weight = mst[i].weight;
        mstEdges.insert({std::min(u, weight), std::max(u, weight)});
    }

    for (int i = 0; i < graph.GetVertexCount(); ++i)
    {
        TValue vertex = graph.GetVertex(i);
        bool isInMST = false;

        for (int k = 0; k < mst.GetLength(); ++k)
        {
            if (mst[k].vertex == vertex || mst[k].weight == vertex)
            {
                isInMST = true;
                break;
            }
        }

        if (isInMST)
        {
            dotFile << "  \"" << vertex << "\" [style=filled, fillcolor=lightblue];\n";
        }
        else
        {
            dotFile << "  \"" << vertex << "\";\n";
        }
    }

    for (int i = 0; i < graph.GetVertexCount(); ++i)
    {
        TValue vertex = graph.GetVertex(i);
        auto adjacentVertices = graph.GetAdjacentVertices(vertex);

        for (int j = 0; j < adjacentVertices.GetLength(); ++j)
        {
            TValue adjacentVertex = adjacentVertices[j].vertex;
            int weight = adjacentVertices[j].weight;

            if (vertex < adjacentVertex) // Уникальные рёбра
            {
                bool isMSTEdge = mstEdges.find({vertex, adjacentVertex}) != mstEdges.end();
                if (isMSTEdge)
                {
                    dotFile << "  \"" << vertex << "\" -- \"" << adjacentVertex << "\" [label=\"" << weight << "\", color=red, penwidth=2.0];\n";
                }
                else
                {
                    dotFile << "  \"" << vertex << "\" -- \"" << adjacentVertex << "\" [label=\"" << weight << "\"];\n";
                }
            }
        }
    }

    dotFile << "}\n";
    dotFile.close();

    std::cout << "Graph with MST and highlighted vertices has been successfully saved to " << filename << " in DOT format.\n";
}

void ShowGraph(const std::string& dotFilename, const std::string& outputFilename);