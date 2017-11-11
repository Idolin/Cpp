cmake_minimum_required(VERSION 3.2)
project(bigint)

add_test(bigint_test big_integer.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/big_integer.test.cpp)
set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)

#if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
#    message("debug mode")
#    add_executable(BIGINT_TEST big_integer.test.cpp)
#endif(${CMAKE_BUILD_TYPE} MATCHES "Debug")

set(SOURCE_LIB big_integer.cpp big_integer.test.cpp)
add_library(bigint_lib STATIC ${SOURCE_LIB})