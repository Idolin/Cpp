cmake_minimum_required(VERSION 3.2)
project(str)

set(SOURCE_LIB str.cpp)
add_library(str_lib STATIC "${SOURCE_LIB}")
add_dependencies(str_lib hash_tgt debug_lib)

add_test(str_test str.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/str.test.cpp)
set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
