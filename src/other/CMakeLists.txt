cmake_minimum_required(VERSION 3.2)

add_library(term_lib STATIC term.cpp)

add_library(rand_lib STATIC rand.cpp)
target_link_libraries(rand_lib debug_lib)

add_library(counter_lib STATIC counter.cpp)

add_custom_target(hash_tgt DEPENDS hash.hpp)

add_library(pam_writer_lib STATIC pam_writer.cpp)
target_link_libraries(pam_writer_lib cstr_lib)

add_test(macros_test macros.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/macros.test.cpp)

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
