cmake_minimum_required(VERSION 3.2)

add_library(icmp_diff_lib EXCLUDE_FROM_ALL STATIC icmp_normalize.cpp)
target_link_libraries(icmp_diff_lib -lpthread -lMagick++-7.Q16HDRI -lMagickWand-7.Q16HDRI -lMagickCore-7.Q16HDRI)
target_compile_options(icmp_diff_lib PRIVATE -O2 -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16)
target_include_directories(icmp_diff_lib PRIVATE /usr/include/ImageMagick-7 /usr/include/ImageMagick-7/Magick++)
