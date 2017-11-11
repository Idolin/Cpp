cmake_minimum_required(VERSION 3.2)
project(debug)

add_custom_target(debug_depends ALL DEPENDS ../string term_lib)

set(SOURCE_LIB tests_run.cpp tests_abstract.cpp def_debug.cpp)
add_library(debug_lib STATIC "${SOURCE_LIB}" test_def.h def_debug.h)
add_dependencies(debug_lib debug_depends)

target_link_libraries(debug_lib str_lib term_lib)