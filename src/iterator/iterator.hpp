#pragma once

#include "./input_iterator.hpp"
#include "./output_iterator.hpp"
#include "./forward_iterator.hpp"
#include "./bidirectional_iterator.hpp"
#include "./random_access_iterator.hpp"

/*
 * Adapters list:
 *  - output_iterator<Impl>
 *  - input_iterator<Impl, [OPTIONAL] value_type, reference_t, pointer_t, difference_type>
 *  - forward_iterator<Impl, [OPTIONAL] value_type, reference_t, pointer_t, difference_type>
 *  - bidirectional_iterator<Impl, [OPTIONAL] value_type, reference_t, pointer_t, difference_type>
 *  - random_access_iterator<Impl, [OPTIONAL] value_type, reference_t, pointer_t, difference_type>
 *
 * These iterator adapters can be used to make iterator classes from any struct which meets following requirements:
 *  It can't have operator++() and increment() method at the same time.
 *  It can't have operator--() and decrement() method at the same time.
 *  It can't have operator+=(integral_type) and increment(integral_type) method at the same time.
 *  It can't have operator-=(integral_type) and decrement(integral_type) method at the same time.
 *  For output iterator:
 *      Requires:
 *          - It must provide operator*() const
 *          - It must provide either operator++() or increment() method or
 *              getIndex() & setIndex(integral_type) methods
 *      Optionally:
 *          - It can provide operator++(int)
 *
 *  For input iterator:
 *      Requires:
 *          - It must provide either operator==(const It&) const or non-member function operator==(const It&, const It&) or getIndex() method
 *          - It must provide operator*() const -> convertible to reference
 *          - It must provide either operator++() or increment() method or
 *              getIndex() const & setIndex(integral_type) methods
 *      Optionally:
 *          - It can provide operator!=(const It&) const or non-member function operator!=(const It&, const It&)
 *          - It can provide operator->() const -> convertible to pointer
 *          - It can provide operator++(int)
 *
 *  For forward iterator:
 *      Same as for input iterator, but with stricter restrictions for post-increment operator return type
 *
 *  For bidirectional iterator:
 *      Forward iterator requirements, and
 *      Requires:
 *          - It must provide either operator--() or decrement() method or
 *              getIndex() const & setIndex(integral_type) methods
 *      Optionally:
 *          - It can provide operator--(int)
 *
 *  For random access iterator:
 *      Bidirectional iterator requirements, and
 *      Requires:
 *          - It must provide either operator+=(integral_type) or increment(integral_type) or
 *              getIndex() const & setIndex(integral_type) methods
 *          - It must provide either operator-=(integral_type) or decrement(integral_type) or
 *              getIndex() const & setIndex(integral_type) methods
 *          - It must provide either operator-(const It&) or
 *              getIndex() const & setIndex(integral_type) methods
 *          - It must provide either member operator<(const It&) const or non-member function operator<(const It&, const It&) const or getIndex() const method
 *      Optionally:
 *          - It can provide non-member function operator+(const It&, integral_type) or member operator+(integral_type) const
 *          - It can provide non-member function operator+(integral_type, const It&)
 *          - It can provide non-member function operator-(const It&, integral_type) or member operator-(integral_type) const
 *          - It can provide operator[integral_type] const
 *          - It can provide operator>(const It&) const
 *          - It can provide operator>=(const It&) const
 *          - It can provide operator<=(const It&) const
 *
 *  Iterator adapter types deduction (if type is not explicitly provided):
 *      value_type:
 *       - if exists Impl::value_type typedef then value_type = Impl::value_type, otherwise
 *       - value_type = dereferenced return type of Impl::operator*() without const qualifier
 *
 *      reference_t:
 *       - if it's output iterator adapter, then reference_t = void, otherwise
 *       - if value_type was explicitly provided:
 *          - if it's input iterator adapter:
 *              - if exists Impl::value_type and Impl::reference typedefs and
 *                  Impl::reference without cv qualifiers != Impl::value_type&, then reference_t = Impl::reference, otherwise
 *              - reference_t = const value_type&, otherwise
 *          - if exists Impl::reference typedef and it's reference to const (const T&),
 *              then reference_t = const value_type&, otherwise
 *          - if return type of Impl::operator*() is reference to const (const T&),
 *              then reference_t = const value_type&, otherwise
 *          - reference_t = value_type&, otherwise
 *       - if exists Impl::reference typedef then reference_t = Impl::reference, otherwise
 *       - if it's input iterator adapter, then
 *          reference_t = return type of Impl::operator*() with const qualifier for referred type, otherwise
 *       - reference_t = return type of Impl::operator*()
 *
 *      pointer_t:
 *       - if it's output iterator adapter, then pointer_t = void, otherwise
 *       - if reference_t was explicitly provided:
 *          - if not exists Impl::operator->(), then pointer_t = T*, where T = dereferenced reference_t, otherwise
 *          - if Impl::operator->() return type without pointer and cv qualifiers is same, as
 *              dereferenced return type of Impl::operator*() without cv qualifiers, then
 *              pointer_t = T*, where T = dereferenced reference_t, otherwise
 *       - if value_type was explicitly provided:
 *          - if not exists Impl::operator->():
 *              - if it's input iterator adapter, then pointer_t = const value_type*, otherwise
 *              - pointer_t = value_type*, otherwise
 *          - if exists Impl::value_type typedef and Impl::operator->() return type without pointer and cv qualifiers == Impl::value_type:
 *              - if Impl::operator->() return type without pointer has volatile qualifier:
 *                 - if it's input iterator adapter, then pointer_t = const volatile value_type*, otherwise
 *                 - if Impl::operator->() return type without pointer has const qualifier,
 *                  then pointer_t = const volatile value_type*, otherwise
 *                 - pointer_t = volatile value_type*, otherwise
 *              - if Impl::operator->() return type without pointer has const qualifier,
 *                  then pointer_t = const value_type*, otherwise
 *              - pointer_t = value_type*, otherwise
 *          - pointer_t = Impl::operator->() return type, otherwise
 *       - if exists Impl::pointer typedef, then pointer_t = Impl::pointer, otherwise
 *       - if exists Impl::operator->():
 *          - if it's input iterator adapter, then
 *              pointer_t = return type of Impl::operator->() with const qualifier for referred type, otherwise
 *          - pointer_t = return type of Impl::operator->(), otherwise
 *       - pointer_t = T*, where T = dereferenced reference_t
 *
 *      difference_type:
 *       - if it's output iterator adapter, then difference_type = void, otherwise
 *       - if exists Impl::difference_type typedef, then difference_type = Impl::difference_type, otherwise,
 *       - if exists Impl::operator-(const Impl&) const or operator-(const Impl&, const Impl&) which return type is integral, then
 *              difference_type = return type of 'Impl - Impl' expression, otherwise
 *       - difference_type = std::ptrdiff_t
 */
