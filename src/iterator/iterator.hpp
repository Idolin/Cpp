#pragma once

#include "./input_iterator.hpp"
#include "./output_iterator.hpp"
#include "./forward_iterator.hpp"
#include "./bidirectional_iterator.hpp"
#include "./random_access_iterator.hpp"

/*
 * These iterator adapters can be used to make iterator classes from any struct which meets following requirements:
 *  For output iterator:
 *      Requires:
 *          - It must provide operator*()
 *          - It must provide either operator++() or increment() method or
 *              getIndex() & setIndex(integral_type) methods
 *      Optionally:
 *          - It can provide operator++(int)
 *
 *  For input iterator:
 *      Requires:
 *          - It must provide either operator==(const It&) or getIndex() method
 *          - It must provide operator*()
 *          - It must provide either operator++() or increment() method or
 *              getIndex() & setIndex(integral_type) methods
 *      Optionally:
 *          - It can provide operator!=(const It&)
 *          - It can provide operator->()
 *          - It can provide operator++(int)
 *
 *  For forward iterator:
 *      Same as for input iterator, but with stricter restrictions for post-increment operator return type
 *
 *  For bidirectional iterator:
 *      Forward iterator requirements, and
 *      Requires:
 *          - It must provide either operator--() or decrement() method or
 *              getIndex() & setIndex(integral_type) methods
 *      Optionally:
 *          - It can provide operator--(int)
 *
 *  For random access iterator:
 *      Bidirectional iterator requirements, and
 *      Requires:
 *          - It must provide either operator+=(integral_type) or increment(integral_type) or
 *              getIndex() & setIndex(integral_type) methods
 *          - It must provide either operator-=(integral_type) or decrement(integral_type) or
 *              getIndex() & setIndex(integral_type) methods
 *          - It must provide either operator-=(const It&) or
 *              getIndex() & setIndex(integral_type) methods
 *          - It must provide either operator<(const It&) or getIndex() method
 *      Optionally:
 *          - It can provide non-member function operator+(const It&, integral_type)
 *          - It can provide non-member function operator+(integral_type, const It&)
 *          - It can provide non-member function operator-(const It&, integral_type)
 *          - It can provide operator[integral_type]
 *          - It can provide operator>()
 *          - It can provide operator>=()
 *          - It can provide operator<=()
 *
 * If struct Impl meets above requirements, iterator struct can be made like this:
 *  For output iterator:
 *      using It = output_iterator<Impl>
 *  For other iterators:
 *      using It = $iterator_type$<Impl, value_type, reference, pointer, difference_type>,
 *      Where:
 *          $iterator_type$ is input_iterator, forward_iterator, bidirectional_iterator or random_access_iterator
 *          value_type, reference, pointer and difference_type are optional template parameters
 */
