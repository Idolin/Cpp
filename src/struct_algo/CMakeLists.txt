cmake_minimum_required(VERSION 3.2)

set(SOURCE_TGT_BIN_SRCH binary_search.hpp)
add_custom_target(bin_srch_tgt
        DEPENDS "${SOURCE_TGT_BIN_SRCH}")

add_test(bin_srch_test binary_search.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/binary_search.test.cpp)

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)