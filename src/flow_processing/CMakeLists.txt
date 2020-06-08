cmake_minimum_required(VERSION 3.2)

set(SOURCE_TGT_WELFORD welford.hpp)
add_custom_target(welford_tgt DEPENDS "${SOURCE_TGT_SORT}")
add_test(welford_test welford.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/welford.test.cpp)

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
