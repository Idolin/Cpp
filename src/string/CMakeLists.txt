cmake_minimum_required(VERSION 3.2)
project(str)

set(SOURCE_LIB cstr.cpp)
add_library(cstr_lib STATIC "${SOURCE_LIB}")
add_dependencies(cstr_lib hash_tgt debug_lib)

add_test(str_test cstr.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cstr.test.cpp)
set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
