cmake_minimum_required(VERSION 3.2)

set(VECTOR_SOURCE_TGT vector.hpp)
add_custom_target(vector_tgt
        DEPENDS "${VECTOR_SOURCE_TGT}")

add_test(vector_test vector.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/vector.test.cpp)


set(LINKED_LIST_SOURCE_TGT linkedlist.hpp)
add_custom_target(linkedlist_tgt
        DEPENDS "${LINKED_LIST_SOURCE_TGT}")

add_test(linkedlist_test linkedlist.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/linkedlist.test.cpp)


set(QUEUE_SOURCE_TGT queue.hpp)
add_custom_target(queue_tgt
        DEPENDS "${QUEUE_SOURCE_TGT}")

add_test(queue_test queue.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/queue.test.cpp)


add_library(bit_array_lib STATIC bit_array.cpp)

add_test(bit_array_test bit_array.test.cpp)
list(APPEND TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/bit_array.test.cpp)


set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
