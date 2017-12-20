cmake_minimum_required(VERSION 3.2)

set(SOURCE_LIB graph.cpp)
add_library(graph_lib STATIC "${SOURCE_LIB}")

add_dependencies(graph_lib heap_tgt vector_tgt)