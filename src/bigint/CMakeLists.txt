cmake_minimum_required(VERSION 3.2)
project(bigint)

set(SOURCE_LIB_BIGINT big_integer.cpp)
add_library(bigint_lib STATIC ${SOURCE_LIB_BIGINT})

set(SOURCE_LIB_INT128 int128.cpp)
add_library(int128_lib STATIC ${SOURCE_LIB_INT128})

add_test(int128_test int128.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/int128.test.cpp)

add_test(bigint_test big_integer.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/big_integer.test.cpp)

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
