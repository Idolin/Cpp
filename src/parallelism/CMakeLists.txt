cmake_minimum_required(VERSION 3.2)

set(SOURCE_TGT_SPINLOCK spinlock.hpp queued_spinlock.hpp)
add_custom_target(spinlock_tgt
        DEPENDS "${SOURCE_TGT_AVL}")

add_library(parallel_lib STATIC just_main.cpp)
target_link_libraries(parallel_lib -pthread)