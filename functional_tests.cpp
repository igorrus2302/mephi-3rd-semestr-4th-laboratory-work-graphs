#include "functional_tests.h"
#include "hash_table.h"
#include "undirected_graph.h"
#include "dynamic_array.h"

#include <cassert>
#include <string>
#include <iostream>



void TestHashTable()
{

    HashTable<int, std::string> table(10);
    assert(table.GetCapacity() == 10);
    assert(table.GetCount() == 0);
    assert(table.IsEmpty());

    table.Add(1, "value1");
    table.Add(2, "value2");
    table.Add(3, "value3");

    assert(table.GetCount() == 3);
    assert(!table.IsEmpty());
    assert(table.ContainsKey(1));
    assert(table.ContainsKey(2));
    assert(table.ContainsKey(3));
    assert(!table.ContainsKey(4));

    assert(table.GetValue(1).has_value() && table.GetValue(1).value() == "value1");
    assert(table.GetValue(2).has_value() && table.GetValue(2).value() == "value2");
    assert(table.GetValue(3).has_value() && table.GetValue(3).value() == "value3");
    assert(!table.GetValue(4).has_value());

    table.Remove(2);
    assert(!table.ContainsKey(2));
    assert(table.GetCount() == 2);

    table.Add(2, "new_value2");
    assert(table.ContainsKey(2));
    assert(table.GetValue(2).has_value() && table.GetValue(2).value() == "new_value2");
    assert(table.GetCount() == 3);

    for (int i = 4; i <= 15; ++i)
    {
        table.Add(i, "value" + std::to_string(i));
    }

    assert(table.GetCapacity() >= 20);
    assert(table.GetCount() == 15);

    for (int i = 1; i <= 15; ++i)
    {
        assert(table.ContainsKey(i));
        assert(table.GetValue(i).has_value());
    }

    for (int i = 1; i <= 15; ++i)
    {
        table.Remove(i);
    }

    assert(table.IsEmpty());
    assert(table.GetCapacity() == 20);

    std::cout << "All hash table tests passed!" << std::endl;
}

void TestUndirectedGraph()
{
    UndirectedGraph<int> graph;
    assert(graph.GetVertexCount() == 0);

    graph.AddVertex(1);
    graph.AddVertex(2);
    graph.AddVertex(3);

    assert(graph.GetVertexCount() == 3);
    assert(graph.GetVertex(0) == 1);
    assert(graph.GetVertex(1) == 2);
    assert(graph.GetVertex(2) == 3);

    graph.AddEdge(1, 2, 10);
    graph.AddEdge(2, 3, 5);
    graph.AddEdge(1, 3, 15);

    assert(graph.AreConnected(1, 2));
    assert(graph.AreConnected(2, 3));
    assert(graph.AreConnected(1, 3));
    assert(!graph.AreConnected(1, 4));

    auto adj1 = graph.GetAdjacentVertices(1);
    assert(adj1.GetLength() == 2);

    graph.RemoveEdge(1, 2);
    assert(!graph.AreConnected(1, 2));

    graph.RemoveVertex(2);
    assert(graph.GetVertexCount() == 2);
    assert(!graph.AreConnected(1, 2));

    graph.AddVertex(2);
    graph.AddEdge(1, 2, 10);
    graph.AddEdge(2, 3, 5);

    auto distances = graph.DiijkstaAlgorithm(1);
    int index1 = -1, index2 = -1;

    for (int i = 0; i < graph.GetVertexCount(); ++i)
    {
        if (graph.GetVertex(i) == 2) index1 = i;
        if (graph.GetVertex(i) == 3) index2 = i;
    }

    assert(index1 != -1 && index2 != -1);
    assert(distances.GetElement(index1) == 10);
    assert(distances.GetElement(index2) == 15);

    graph.AddEdge(1, 3, 1);
    auto mst = graph.FindMinimumSpanningTreeKruskal();
    assert(mst.GetLength() == 2); // Остов должен содержать V-1 рёбер

    auto colors = graph.ColorGraph();
    assert(colors.GetLength() == graph.GetVertexCount());

    std::cout << "All undirected graph tests passed!" << std::endl;
}

void TestDynamicArray()
{
    DynamicArray<int> array(5);
    assert(array.GetLength() == 5);

    for (int i = 0; i < 5; ++i)
    {
        array.Set(i, i + 1);
    }

    assert(array.GetFirstElement() == 1);
    assert(array.GetLastElement() == 5);

    array.Append(6);
    assert(array.GetLength() == 6);
    assert(array.GetLastElement() == 6);

    array.Prepend(0);
    assert(array.GetLength() == 7);
    assert(array.GetFirstElement() == 0);

    array.InsertAt(99, 3);
    assert(array.GetLength() == 8);
    assert(array.GetElement(3) == 99);

    array.Remove(3);
    assert(array.GetLength() == 7);
    assert(array.GetElement(3) != 99);

    auto subsequence = array.GetSubsequence(2, 4);
    assert(subsequence->GetLength() == 3);
    assert(subsequence->GetFirstElement() == array.GetElement(2));
    assert(subsequence->GetLastElement() == array.GetElement(4));
    delete subsequence;

    DynamicArray<int> array2(3);
    array2.Set(0, 10);
    array2.Set(1, 20);
    array2.Set(2, 30);

    array.Union(&array2);
    assert(array.GetLength() == 10);
    assert(array.GetElement(7) == 10);
    assert(array.GetElement(8) == 20);
    assert(array.GetElement(9) == 30);

    DynamicArray<int> array3 = array;
    assert(array3 == array);

    DynamicArray<int> array4(array);
    assert(array4 == array);

    std::cout << "All dynamic array tests passed!" << std::endl;
}

void RunFunctionalTests()
{
    TestDynamicArray();
    TestHashTable();
    TestUndirectedGraph();

    std::cout << "\n";
}