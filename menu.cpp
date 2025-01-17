#include "menu.h"
#include "undirected_graph.h"
#include "dynamic_array.h"
#include "graph_creator.h"
#include "print_distances.h"
#include "print_colors.h"
#include "show_graph.h"
#include "functional_tests.h"

#include <iostream>



int Functions()
{
    std::cout << "  Menu:\n\n";
    std::cout << "0. Exit\n";
    std::cout << "1. Add vertex\n";
    std::cout << "2. Add edge\n";
    std::cout << "3. Delete vertex\n";
    std::cout << "4. Delete edge\n";
    std::cout << "5. Generate random graph\n";
    std::cout << "6. Run Diijksta algorithm\n";
    std::cout << "7. Paint graph\n";
    std::cout << "8. Search the skeleton of the graph\n";
    std::cout << "9. Show graph\n";

    std::cout << "\n";
    std::cout << "Input number of function:\n";

    int function;

    std::cin >> function;

    return function;
}

void Menu()
{
    bool isOpen = true;

    UndirectedGraph<int> graph;

    RunFunctionalTests();

    while (isOpen)
    {
        int function = Functions();

        switch (function)
        {
            default:
                std::cout << "Wrong number of function input\n\n";
                continue;

            case(0):

                exit(0);

            case(1):
            {
                int vertex;

                std::cout << "Input vertex:\n";
                std::cin >> vertex;

                graph.AddVertex(vertex);

                std::cout << "\n";
                break;
            }

            case (2):
            {
                int firstVertex;
                int secondVertex;
                int edgeWeight;

                std::cout << "Input first vertex:\n";
                std::cin >> firstVertex;

                std::cout << "Input second vertex:\n";
                std::cin >> secondVertex;

                std::cout << "Input weight of the edge:\n";
                std::cin >> edgeWeight;

                graph.AddEdge(firstVertex, secondVertex, edgeWeight);

                std::cout << "\n";
                break;
            }
            case (3):
            {
                int vertex;

                std::cout << "Input vertex to delete:\n";
                std::cin >> vertex;

                graph.RemoveVertex(vertex);

                std::cout << "\n";
                break;
            }
            case (4):
            {
                int firstVertex;
                int secondVertex;

                std::cout << "Input first vertex:\n";
                std::cin >> firstVertex;

                std::cout << "Input second vertex:\n";
                std::cin >> secondVertex;

                graph.RemoveEdge(firstVertex, secondVertex);

                std::cout << "\n";
                break;
            }
            case (5):
            {
                int vertexCount;
                int edgeCount;
                int minWeight;
                int maxWeight;

                std::cout << "Input number of vertexes:\n";
                std::cin >> vertexCount;

                std::cout << "Input number of edges:\n";
                std::cin >> edgeCount;

                std::cout << "Input minimal weight of edge:\n";
                std::cin >> minWeight;

                std::cout << "Input maximum weight of edge:\n";
                std::cin >> maxWeight;

                graph = std::move(GenerateGraph(vertexCount, edgeCount, minWeight, maxWeight));

                std::cout << "\n";
                break;
            }
            case (6):
            {
                int starterVertex;

                std::cout << "Input number of starter vertex\n";
                std::cin >> starterVertex;

                DynamicArray<int> distances = graph.DiijkstaAlgorithm(starterVertex);

                PrintGraphDistances(graph, distances, std::cout);

                std::cout << "\n";
                break;
            }
            case (7):
            {
                DynamicArray<int> colors = graph.ColorGraph();

                PrintGraphColor(graph, colors, std::cout);

                std::string dotFileName = "colored_graph.dot";

                SaveColoredGraphToDot(graph, colors, dotFileName);
                ShowGraph("colored_graph.dot", "colored_graph.png");

                std::cout << "\n";
                break;
            }
            case (8):
            {
                DynamicArray<Edge> mst = graph.FindMinimumSpanningTreeKruskal();

                std::cout << "Minimum Spanning Tree:" << std::endl;
                for (int i = 0; i < mst.GetLength(); i++)
                {
                    std::cout << "Edge to vertex: " << mst[i].vertex << " with weight: " << mst[i].weight << std::endl;
                }

                std::string dotFileName = "skeleton_of_the_graph.dot";

                SaveGraphWithMSTToDot(graph, mst, dotFileName);
                ShowGraph("skeleton_of_the_graph.dot", "skeleton_of_the_graph.png");

                std::cout << "\n";
                break;
            }
            case (9):
            {
                std::string dotFileName = "graph.dot";

                SaveGraphToDot(graph, dotFileName);
                ShowGraph("graph.dot", "graph.png");

                std::cout << "\n";
                break;
            }
        }
    }
}


