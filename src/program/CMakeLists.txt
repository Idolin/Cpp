cmake_minimum_required(VERSION 3.2)

add_executable(icmp EXCLUDE_FROM_ALL icmp.cpp)
target_link_libraries(icmp icmp_diff_lib pam_writer_lib cstr_lib debug_lib -lpthread -lMagick++-7.Q16HDRI -lMagickWand-7.Q16HDRI -lMagickCore-7.Q16HDRI)
target_compile_options(icmp PRIVATE -O2 -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16)
target_include_directories(icmp PRIVATE /usr/include/ImageMagick-7 /usr/include/ImageMagick-7/Magick++)

add_custom_command(TARGET icmp
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:icmp> $ENV{HOME}/Documents/Programming/C++/functions/icmp)

add_executable(icmp_diff EXCLUDE_FROM_ALL icmp_diff.cpp)
target_link_libraries(icmp_diff icmp_diff_lib pam_writer_lib cstr_lib debug_lib -lMagick++-7.Q16HDRI -lMagickWand-7.Q16HDRI -lMagickCore-7.Q16HDRI)
add_custom_command(TARGET icmp_diff
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:icmp_diff> $ENV{HOME}/Documents/Programming/C++/functions/icmp_diff)
        
add_executable(pictures EXCLUDE_FROM_ALL pictures.cpp)
target_link_libraries(pictures pam_writer_lib cstr_lib debug_lib rand_lib)

add_executable(pic_dvt EXCLUDE_FROM_ALL pic_deviation.cpp)
target_link_libraries(pic_dvt -lMagick++-7.Q16HDRI -lMagickWand-7.Q16HDRI -lMagickCore-7.Q16HDRI)
target_compile_options(pic_dvt PRIVATE -O2 -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16)
target_include_directories(pic_dvt PRIVATE /usr/include/ImageMagick-7 /usr/include/ImageMagick-7/Magick++)

add_custom_command(TARGET pic_dvt
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pic_dvt> $ENV{HOME}/Documents/Programming/C++/functions/pic_dvt)
