cmake_minimum_required(VERSION 3.2)

add_library(term_lib STATIC term.cpp)
add_library(rand_lib STATIC rand.cpp)
add_library(counter_lib STATIC counter.cpp)
add_custom_target(hash_tgt DEPENDS hash.hpp)
