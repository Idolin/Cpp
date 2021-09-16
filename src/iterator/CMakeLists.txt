cmake_minimum_required(VERSION 3.2)

set(SOURCE_TGT_ITERATOR_IMPL iterator_impl.hpp)
add_custom_target(iterator_impl_tgt)

set(SOURCE_TGT_ITERATOR iterator.hpp)
add_custom_target(iterator_tgt DEPENDS iterator_impl_tgt)

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
