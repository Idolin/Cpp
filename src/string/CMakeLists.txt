cmake_minimum_required(VERSION 3.2)
project(str)

set(SOURCE_LIB str.cpp)
add_library(str_lib STATIC "${SOURCE_LIB}")