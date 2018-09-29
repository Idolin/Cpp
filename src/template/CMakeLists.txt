cmake_minimum_required(VERSION 3.2)

set(SOURCE_TGT_COMMON_METHODS commonmethods.hpp)
add_custom_target(commonmethods_tgt DEPENDS "${SOURCE_TGT_SORT}")
add_test(commonmethods_test commonmethods.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/commonmethods.test.cpp)

set(SOURCE_TGT_VALUE_METHODS valuemethods.hpp)
add_custom_target(valuemethods_tgt DEPENDS "${SOURCE_TGT_SORT}")

set(SOURCE_TGT_ARRAY_METHODS arraymethods.hpp)
add_custom_target(arraymethods_tgt DEPENDS "${SOURCE_TGT_SORT}")

set(SOURCE_TGT_DISPLAY_METHODS displaymethods.hpp)
add_custom_target(displaymethods_tgt DEPENDS "${SOURCE_TGT_SORT}")

set(SOURCE_TGT_TYPE_METHODS typemethods.hpp)
add_custom_target(typemethods_tgt DEPENDS "${SOURCE_TGT_SORT}")

set(SOURCE_TGT_COMPILE_TIME compile_time.hpp)
add_custom_target(compile_time_tgt DEPENDS "${SOURCE_TGT_SORT}")

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
