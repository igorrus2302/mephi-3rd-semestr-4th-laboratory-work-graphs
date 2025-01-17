cmake_minimum_required(VERSION 3.29)
project(3emestr_4laboratory)

set(CMAKE_CXX_STANDARD 20)

add_executable(3emestr_4laboratory main.cpp
        menu.cpp
        menu.h
        dynamic_array.h
        sequence.h
        hash_table.h
        idictionary.h
        unique_pointer.h
        array_sequence.h
        undirected_graph.h
        graph_creator.h
        graph_creator.cpp
        print_distances.h
        print_colors.h
        show_graph.h
        show_graph.cpp
        functional_tests.cpp
        functional_tests.h)
