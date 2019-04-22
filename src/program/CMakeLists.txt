cmake_minimum_required(VERSION 3.2)

include_directories(/usr/include/ImageMagick-7 /usr/include/ImageMagick-7/Magick++)
add_compile_options(-O2 -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16)

add_executable(icmp icmp.cpp)
target_link_libraries(icmp icmp_diff_lib str_lib -lpthread -lMagick++-7.Q16HDRI -lMagickWand-7.Q16HDRI -lMagickCore-7.Q16HDRI)
add_custom_command(TARGET icmp
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:icmp> $ENV{HOME}/Documents/Programming/C++/functions/icmp)

add_executable(icmp_diff icmp_diff.cpp)
target_link_libraries(icmp_diff icmp_diff_lib str_lib -lMagick++-7.Q16HDRI -lMagickWand-7.Q16HDRI -lMagickCore-7.Q16HDRI)
add_custom_command(TARGET icmp_diff
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:icmp_diff> $ENV{HOME}/Documents/Programming/C++/functions/icmp_diff)