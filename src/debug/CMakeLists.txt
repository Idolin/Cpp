cmake_minimum_required(VERSION 3.2)
project(debug)

set(SOURCE_LIB tests_run.cpp tests_abstract.cpp def_debug.cpp test_def.cpp)
add_library(debug_lib STATIC "${SOURCE_LIB}")
add_dependencies(debug_lib term_lib)

target_link_libraries(debug_lib str_lib term_lib counter_lib)
