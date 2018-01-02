cmake_minimum_required(VERSION 3.2)

set(SOURCE_TGT_HEAP heap.hpp)
add_custom_target(heap_tgt
        DEPENDS "${SOURCE_TGT_HEAP}")

set(SOURCE_TGT_AVL)
add_custom_target(avl_tgt
        DEPENDS "${SOURCE_THT_AVL}")

add_test(avl_test avl.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/avl.test.cpp)
set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)