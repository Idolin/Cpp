cmake_minimum_required(VERSION 3.2)

set(SOURCE_LIB_GRAPH graph.cpp)
add_library(graph_lib STATIC "${SOURCE_LIB_GRAPH}")

add_dependencies(graph_lib heap_tgt vector_tgt)

set(SOURCE_TGT_DSU dsu.hpp)
add_custom_target(dsu_tgt
        DEPENDS "${SOURCE_TGT_DSU}")

add_test(dsu_test dsu.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/dsu.test.cpp)


add_library(bit_array_lib STATIC bit_array.cpp)

add_test(bit_array_test bit_array.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/bit_array.test.cpp)


set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
