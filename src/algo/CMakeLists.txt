cmake_minimum_required(VERSION 3.2)

include_directories(/usr/include/ImageMagick-7 /usr/include/ImageMagick-7/Magick++)
add_compile_options(-O2 -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16)

add_library(icmp_diff_lib STATIC icmp_normalize.cpp)
target_link_libraries(icmp_diff_lib -lpthread -lMagick++-7.Q16HDRI -lMagickWand-7.Q16HDRI -lMagickCore-7.Q16HDRI)
