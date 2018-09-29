cmake_minimum_required(VERSION 3.2)

set(SOURCE_TGT_SORT partition.hpp sortline.hpp sortlog.hpp sortsquare.hpp)
add_custom_target(sort_tgt DEPENDS "${SOURCE_TGT_SORT}")
add_test(sort_test sort.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/sort.test.cpp)

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)