cmake_minimum_required(VERSION 3.2)
project(AADS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    message("debug mode")
    enable_testing()
endif ()

add_subdirectory(src/other)
add_subdirectory(src/string)
add_subdirectory(src/bigint)
add_subdirectory(src/debug)
add_subdirectory(src/container)
add_subdirectory(src/struct)
add_subdirectory(src/struct_algo)
add_subdirectory(src/sort)
add_subdirectory(src/template)
add_subdirectory(src/tree)
#add_subdirectory(src/program)
add_subdirectory(src/parallelism)
add_subdirectory(src/algo)
add_subdirectory(src/window)
add_subdirectory(src/flow_processing)

set(MAIN_FILE ./src/main.cpp)
set(SOURCE_FILES "${MAIN_FILE}")
add_executable(AADS "${SOURCE_FILES}" "${TEST_FILES}")

target_link_libraries(AADS debug_lib bigint_lib graph_lib rand_lib str_lib term_lib parallel_lib bit_array_lib)

add_executable(only_main "${MAIN_FILE}")

add_custom_target(valgrind
        COMMAND valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:AADS>
        DEPENDS AADS
        COMMENT "Checking by valgrind")

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    add_custom_target(assembler_O2
            WORKING_DIRECTORY "/home/cynder/trash"
            COMMAND ${CMAKE_CXX_COMPILER} -O2 -fverbose-asm -S "${CMAKE_SOURCE_DIR}/${MAIN_FILE}" -o main.asm
            COMMAND cat main.asm
            COMMENT "getting assembler listing of main.cpp")
endif ()
