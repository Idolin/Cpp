cmake_minimum_required(VERSION 3.2)

set(SOURCE_TGT vector.hpp)
add_custom_target(vector_tgt
        DEPENDS "${SOURCE_TGT}")
add_dependencies(vector_tgt heap_tgt)