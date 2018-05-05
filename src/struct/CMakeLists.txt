cmake_minimum_required(VERSION 3.2)

set(SOURCE_LIB graph.cpp)
add_library(graph_lib STATIC "${SOURCE_LIB}")

add_dependencies(graph_lib heap_tgt vector_tgt)

set(SOURCE_TGT_DSU dsu.hpp)
add_custom_target(dsu_tgt
        DEPENDS "${SOURCE_TGT_DSU}")

add_test(dsu_test dsu.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/dsu.test.cpp)

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)