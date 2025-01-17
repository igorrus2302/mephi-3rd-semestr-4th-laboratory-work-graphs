#pragma once

#include "hash_table.h"
#include "dynamic_array.h"

#include <optional>
#include <queue>
#include <unordered_map>
#include <limits>
#include <vector>
#include <algorithm>
#include <functional>



class Edge {
public:

    int vertex;
    int weight;

    Edge(int v = 0, int w = 0) : vertex(v), weight(w) {}

    bool operator==(const Edge& other) const
    {
        return vertex == other.vertex && weight == other.weight;
    }

    bool operator!=(const Edge& other) const
    {
        return !(*this == other);
    }
};


template <typename TKey>
class UndirectedGraph {
private:

    int vertexCount;
    DynamicArray<TKey> vertexes;
    HashTable<TKey, DynamicArray<Edge>> adjacencyList;

public:

    UndirectedGraph(int vertexCount = 0)
    {
        if (vertexCount < 0)
            vertexCount = 0;

        this->vertexCount = vertexCount;
        adjacencyList = HashTable<TKey, DynamicArray<Edge>>(vertexCount);
        vertexes = DynamicArray<TKey>(0);
    }

    void AddEdge(TKey vertex1, TKey vertex2, int weight)
    {
        if (adjacencyList.GetValue(vertex1) == std::nullopt || adjacencyList.GetValue(vertex2) == std::nullopt)
            return;

        for (int i = 0; i < adjacencyList.GetValue(vertex1).value().GetLength(); i++)
            if (adjacencyList.GetValue(vertex1).value()[i].vertex == vertex2)
                return;

        DynamicArray<Edge> array1 = adjacencyList.GetValue(vertex1).value();
        array1.Append(Edge(vertex2, weight));
        DynamicArray<Edge> array2 = adjacencyList.GetValue(vertex2).value();
        array2.Append(Edge(vertex1, weight));
        adjacencyList.Add(vertex1, array1);
        adjacencyList.Add(vertex2, array2);
    }

    void AddVertex(TKey vertex)
    {
        for (int i = 0; i < vertexes.GetLength(); i++)
            if (vertexes[i] == vertex)
                return;

        DynamicArray<Edge> newAdjacencyList;

        adjacencyList.Add(vertex, newAdjacencyList);
        vertexes.Append(vertex);
        vertexCount++;
    }

    int GetVertexCount() const
    {
        return vertexCount;
    }

    TKey GetVertex(int index) const
    {
        if (index < 0 || index > vertexCount)
            return TKey();

        return vertexes[index];
    }

    DynamicArray<Edge> GetAdjacentVertices(TKey vertex) const
    {
        auto result = adjacencyList.GetValue(vertex);

        if (result.has_value())
            return result.value();
        else
            return DynamicArray<Edge>();
    }

    bool AreConnected(TKey vertex1, TKey vertex2) const
    {
        if (adjacencyList.GetValue(vertex1) == std::nullopt || adjacencyList.GetValue(vertex2) == std::nullopt)
            return false;

        DynamicArray<Edge> adjacentVertices = GetAdjacentVertices(vertex1);

        for (int i = 0; i < adjacentVertices.GetLength(); i++)
            if (adjacentVertices[i].vertex == vertex2)
                return true;

        return false;
    }

    void RemoveEdge(TKey vertex1, TKey vertex2)
    {
        if (adjacencyList.GetValue(vertex1) == std::nullopt || adjacencyList.GetValue(vertex2) == std::nullopt)
            return;

        DynamicArray<Edge> array1 = adjacencyList.GetValue(vertex1).value();
        for (int i = 0; i < array1.GetLength(); i++)
        {
            if (array1[i].vertex == vertex2)
            {
                array1.Remove(i);
                break;
            }
        }

        adjacencyList.Add(vertex1, array1);

        DynamicArray<Edge> array2 = adjacencyList.GetValue(vertex2).value();

        for (int i = 0; i < array2.GetLength(); i++)
        {
            if (array2[i].vertex == vertex1)
            {
                array2.Remove(i);
                break;
            }
        }

        adjacencyList.Add(vertex2, array2);
    }

    void RemoveVertex(TKey vertex)
    {
        if (adjacencyList.GetValue(vertex) == std::nullopt)
            return;

        DynamicArray<Edge> adjacentEdges = GetAdjacentVertices(vertex);

        for (int i = 0; i < vertexes.GetLength(); i++)
        {
            if (vertexes[i] == vertex)
            {
                vertexes.Remove(i);
                break;
            }
        }

        for (int i = 0; i < adjacentEdges.GetLength(); i++)
        {
            TKey adjacentVertex = adjacentEdges[i].vertex;
            RemoveEdge(vertex, adjacentVertex);
        }

        adjacencyList.Remove(vertex);
        vertexCount--;
    }

    DynamicArray<int> ColorGraph()
    {
        DynamicArray<int> colors(vertexes.GetLength());

        for (int i = 0; i < vertexes.GetLength(); i++)
            colors.Set(i, -1);

        for (int i = 0; i < vertexes.GetLength(); i++)
        {
            TKey currentVertex = vertexes[i];
            DynamicArray<bool> availableColors(vertexes.GetLength());

            for (int j = 0; j < availableColors.GetLength(); j++)
                availableColors.Set(j, true);

            DynamicArray<Edge> adjacentEdges = GetAdjacentVertices(currentVertex);

            for (int j = 0; j < adjacentEdges.GetLength(); j++)
            {
                TKey neighbor = adjacentEdges[j].vertex;

                for (int k = 0; k < vertexes.GetLength(); k++)
                {
                    if (vertexes[k] == neighbor && colors.GetElement(k) != -1)
                    {
                        availableColors.Set(colors.GetElement(k), false);
                        break;
                    }
                }
            }

            for (int color = 0; color < availableColors.GetLength(); color++)
            {
                if (availableColors.GetElement(color))
                {
                    colors.Set(i, color);
                    break;
                }
            }
        }

        return colors;
    }

    DynamicArray<int> DiijkstaAlgorithm(TKey startVertex)
    {
        DynamicArray<int> distances(vertexes.GetLength());
        DynamicArray<bool> visited(vertexes.GetLength());
        std::unordered_map<TKey, int> vertexIndexMap;

        for (int i = 0; i < vertexes.GetLength(); i++)
        {
            distances.Set(i, std::numeric_limits<int>::max());
            visited.Set(i, false);
            vertexIndexMap[vertexes[i]] = i;
        }

        auto startIt = vertexIndexMap.find(startVertex);
        if (startIt == vertexIndexMap.end())
        {
            throw std::invalid_argument("Start vertex not found in the graph.");
        }
        int startIndex = startIt->second;
        distances.Set(startIndex, 0);

        for (int i = 0; i < vertexes.GetLength(); i++)
        {
            int minDistance = std::numeric_limits<int>::max();
            int minIndex = -1;

            for (int j = 0; j < vertexes.GetLength(); j++)
            {
                if (!visited.GetElement(j) && distances.GetElement(j) < minDistance)
                {
                    minDistance = distances.GetElement(j);
                    minIndex = j;
                }
            }

            if (minIndex == -1) break;

            visited.Set(minIndex, true);
            TKey minVertex = vertexes[minIndex];
            DynamicArray<Edge> adjacentEdges = GetAdjacentVertices(minVertex);

            for (int j = 0; j < adjacentEdges.GetLength(); j++)
            {
                TKey neighbor = adjacentEdges[j].vertex;
                int weight = adjacentEdges[j].weight;

                auto neighborIt = vertexIndexMap.find(neighbor);
                if (neighborIt != vertexIndexMap.end())
                {
                    int neighborIndex = neighborIt->second;

                    if (!visited.GetElement(neighborIndex) &&
                        distances.GetElement(minIndex) + weight < distances.GetElement(neighborIndex))
                    {
                        distances.Set(neighborIndex, distances.GetElement(minIndex) + weight);
                    }
                }
            }
        }

        return distances;
    }

    DynamicArray<Edge> FindMinimumSpanningTreeKruskal()
    {
        DynamicArray<Edge> mst;
        if (vertexes.GetLength() == 0)
            return mst;

        std::vector<std::tuple<int, int, int>> edges;

        for (int i = 0; i < vertexes.GetLength(); i++)
        {
            TKey u = vertexes[i];
            DynamicArray<Edge> adjacentEdges = GetAdjacentVertices(u);

            for (int j = 0; j < adjacentEdges.GetLength(); j++)
            {
                TKey v = adjacentEdges[j].vertex;
                int weight = adjacentEdges[j].weight;

                if (u < v)
                    edges.push_back({weight, u, v});
            }
        }

        std::sort(edges.begin(), edges.end());

        std::unordered_map<TKey, TKey> parent;
        std::unordered_map<TKey, int> rank;

        std::function<TKey(TKey)> Find = [&](TKey vertex) -> TKey {
            if (parent[vertex] != vertex)
                parent[vertex] = Find(parent[vertex]);
            return parent[vertex];
        };

        auto Union = [&](TKey root1, TKey root2) {
            if (rank[root1] < rank[root2])
                parent[root1] = root2;
            else if (rank[root1] > rank[root2])
                parent[root2] = root1;
            else
            {
                parent[root2] = root1;
                rank[root1]++;
            }
        };

        for (int i = 0; i < vertexes.GetLength(); i++)
        {
            TKey vertex = vertexes[i];
            parent[vertex] = vertex;
            rank[vertex] = 0;
        }

        for (const auto& edge : edges)
        {
            int weight = std::get<0>(edge);
            TKey u = std::get<1>(edge);
            TKey v = std::get<2>(edge);

            TKey rootU = Find(u);
            TKey rootV = Find(v);

            if (rootU != rootV)
            {
                mst.Append(Edge(v, weight));
                Union(rootU, rootV);
            }
        }

        return mst;
    }
};
