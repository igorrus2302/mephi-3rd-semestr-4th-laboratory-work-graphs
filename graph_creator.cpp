#include "graph_creator.h"
#include "undirected_graph.h"

#include <cstdlib>
#include <random>
#include <algorithm>



UndirectedGraph<int> GenerateGraph(int vertexCount, int edgeCount, int minWeight, int maxWeight)
{
    UndirectedGraph<int> graph;

    for (int i = 0; i < vertexCount; i++)
        graph.AddVertex(i);

    int maxEdges = vertexCount * (vertexCount - 1) / 2;
    edgeCount = std::min(edgeCount, maxEdges);

    int edgesAdded = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> weightDis(minWeight, maxWeight);

    while (edgesAdded < edgeCount)
    {
        int vertex1 = rand() % vertexCount;
        int vertex2 = rand() % vertexCount;

        if (vertex1 == vertex2)
            continue;

        int weight = weightDis(gen);

        if (!graph.AreConnected(vertex1, vertex2))
        {
            graph.AddEdge(vertex1, vertex2, weight);
            edgesAdded++;
        }
    }

    return graph;
}