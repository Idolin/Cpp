#pragma once

#include "../template/arraymethods.hpp"
#include "../template/commonmethods.hpp"
#include "../template/typemethods.hpp"
#include "../template/type_tags.hpp"
#include "../template/t_useful.hpp"
#include "sortsquare.hpp"
#include <cstddef>
#include <limits>
#include <functional>
#include <vector>


namespace
{

    // def_getter for unsigned type T
    template<typename T>
    constexpr std::enable_if_t<std::is_unsigned<T>::value, T> def_getter(T value, unsigned)
    {
        return value;
    }

    // def_getter for signed type T
    template<typename T>
    constexpr std::enable_if_t<std::is_signed<T>::value, typename std::make_unsigned<T>::type> def_getter(T value, unsigned)
    {
        using TU = typename std::make_unsigned<T>::type;

        // as TU is unsigned type (and we assume the arithmetic is two's complement) static_cast<TU>(value) is a well-defined behaviour
        // we call def_getter with value remapped to value_unsigned following this rule:
        //  value == std::numeric_limits<T>::min -> value_unsigned = std::numeric_limits<TU>::min
        //  value == std::numeric_limits<T>::min + 1 -> value_unsigned = std::numeric_limits<TU>::min + 1
        //  ...
        //  value == std::numeric_limits<T>::max -> value_unsigned = std::numeric_limits<TU>::max
        return static_cast<TU>(value) ^ (static_cast<TU>(1) << (sizeof(TU) * 8 - 1));
    }

    constexpr unsigned ceil(unsigned a, unsigned b) noexcept
    {
        return (a + b - 1) / b;
    }

    template<typename T, typename R>
    constexpr unsigned getter_parts() noexcept
    {
        return ceil(sizeof(T), sizeof(R));
    }

    // I couldn't find a way to directly check whether compare function is NULL or not later in BucketsortImpl
    //  without it producing a warning in GCC (the address of ... will never be NULL),
    //  so instead compare_available bool will be used to check it indirectly

    // helper struct for partial specialization
    // https://stackoverflow.com/questions/22486386/partially-specializing-a-non-type-template-parameter-of-dependent-type
    template<typename T, compare_func_t<T> compare>
    struct FunctionToTypeWrapper
    {};

    template<typename T, compare_func_t<T> compare_function, typename FunctionWrapper>
    struct GetterCompare
    {
        static constexpr compare_func_t<T> compare = compare_function;
        static constexpr bool compare_available = true;

        static void call_sort(T *start, T *end)
        {
            minmaxsort<T, compare_function>(start, end);
        }
    };

    template<typename T, compare_func_t<T> compare_function>
    struct GetterCompare<T, compare_function, FunctionToTypeWrapper<T, nullptr>>
    {
        static constexpr compare_func_t<T> compare = nullptr;
        static constexpr bool compare_available = false;
    };

}


// T - input type (any type), R - return type (unsigned integral),
// getter_function - pointer to function R(def_get_by_t<T>, unsigned) which returns from highest (most matters) parts to lowest (least matters) parts
// bytes_length - amount of bytes which can be retrieved from value of type T by getter_function in the sum (by default = sizeof(T))
//  if R = uint16_t and bytes_length = 5 when getter_function will be called up to 3 times, and in the last time only last byte will be meaningful
template<typename T, typename R, R(*getter_function)(def_get_by_t<T>, unsigned),
         compare_func_t<T> compare_function = nullptr, unsigned bytes_length = sizeof(T),
         typename = typename std::enable_if_t<(bytes_length > 0)>>
struct GetterFixed: GetterCompare<T, compare_function, FunctionToTypeWrapper<T, compare_function>>
{
    static_assert(std::is_unsigned<R>::value, "Getter return value must be of unsigned integer type");
    static_assert(getter_function != nullptr, "Getter function pointer can't be NULL");
    using type = T;
    using RType = R;
    using FuncType = R(*)(def_get_by_t<T>, unsigned);

    static constexpr FuncType function = getter_function;
    static constexpr unsigned bytes = bytes_length;
};

// T - input type (any type), R - return type (unsigned integral),
// getter_function - pointer to function R(def_get_by_t<T>, unsigned, bool &is_finished) which returns from highest (most matters) parts to lowest (least matters) parts
//  if end is reached it sets is_finished flag to true
template<typename T, typename R, R(*getter_function)(def_get_by_t<T>, unsigned, bool&),
         compare_func_t<T> compare_function = nullptr,
         typename = typename std::enable_if_t<std::is_unsigned<R>::value && (getter_function != nullptr)>>
struct GetterNonFixed: GetterCompare<T, compare_function, FunctionToTypeWrapper<T, compare_function>>
{
    using type = T;
    using RType = R;
    using FuncType = R(*)(def_get_by_t<T>, unsigned, bool&);

    static constexpr FuncType function = getter_function;
    static constexpr unsigned bytes = static_cast<unsigned>(-1);
};

template<typename T>
using DefGetter = GetterFixed<T, decltype(def_getter<T>(std::declval<T>(), 0u)), def_getter<T>, def_less<T>>;



namespace
{

    template<typename Getter>
    inline uint16_t getter_func_compress(def_get_by_t<typename Getter::type> value, unsigned part)
    {
        uint16_t result = Getter::function(value, part * 2);
        if(part * 2 + 1 < Getter::parts) // TODO: optimize condition out of cycle
            result |= (static_cast<uint16_t>(Getter::function(value, part * 2 + 1)) << 8);
        return result;
    }

    template<typename Getter, unsigned divisor = getter_parts<typename Getter::RType, uint16_t>()>
    inline uint16_t getter_func_split(def_get_by_t<typename Getter::type> value, unsigned part)
    {
        auto getter_result = Getter::function(value, part / divisor);
        return static_cast<uint16_t>(getter_result >> (sizeof(typename Getter::RType) - sizeof(uint16_t) * (part % divisor + 1)) * 8);
    }


    // TODO: specific implementation optimizations for non-scalar types
    // TODO: add specialization for non-fixed Getter
    template<typename T, typename Getter, typename Enable = void>
    struct BucketsortImpl;

    // Specialization for GetterFixed type
    template<typename T, typename Getter>
    struct BucketsortImpl<T, Getter, std::enable_if_t<(Getter::bytes != static_cast<unsigned>(-1))>>
    {
    private:
        using GR = typename Getter::RType; // Getter::function return type

        template<typename G> // Getter::compare will be used as compare function
        using GetterCompress = GetterFixed<T, uint16_t, getter_func_compress<G>, nullptr, G::bytes>;

        template<typename G> // Getter::compare will be used as compare function
        using GetterSplit = GetterFixed<T, uint16_t, getter_func_split<G>, nullptr, G::bytes>;


        // internal getter:
        //  if Getter returns more than 1 value of 1 byte size, then compresses two subsequent values into one uint16_t type
        //  if Getter return type size is more than 2 bytes, then split it to few uint16_t values
        //  otherwise uses given Getter as is
        using InternalGetter = typename FirstType<
            InstantiateIf<sizeof(GR) == 1 && (Getter::bytes > 1), GetterCompress, Getter>,
            InstantiateIf<(sizeof(GR) > 2), GetterSplit, Getter>,
            ReturnType<Getter>>::type;

        using R = typename InternalGetter::RType; // InternalGetter::function return type
        using B = std::conditional_t<sizeof(R) == 1, uint8_t, uint16_t>; // we will sort using no more than 2 bytes at a time

        static constexpr std::size_t square_sort_elements_max = 800;

        T *start, *end, *copy;
        union buckets_t
        {
            uint16_t *buckets_16;
            uint32_t *buckets_32;

            buckets_t() = default;

            buckets_t(uint16_t *buckets): buckets_16(buckets)
            {}

            buckets_t(uint32_t *buckets): buckets_32(buckets)
            {}

            operator uint16_t*() const
            {
                return buckets_16;
            }

            operator uint32_t*() const
            {
                return buckets_32;
            }
        };
        std::vector<buckets_t> buckets;
        R *values_parts;

        template<typename E, typename B>
        void new_buckets()
        {
            buckets.push_back(new E[static_cast<std::size_t>(std::numeric_limits<B>::max()) + 1u]);
        }

        std::ptrdiff_t length;

        template<typename E, typename B, bool enable = Getter::compare_available>
        struct Impl;

    public:
        BucketsortImpl(T *start, T *end): start(start), end(end), copy(nullptr), buckets(), values_parts(nullptr),
            length(end - start)
        {
            if(static_cast<std::size_t>(length) < square_sort_elements_max)
            {
                Getter::call_sort(start, end);
                return;
            }

            copy = new T[length];
            values_parts = new R[length];

            if(length <= std::numeric_limits<uint16_t>::max()) // amount of elements + 1 fits in 16-bit unsigned integer type
                Impl<uint16_t, B>::call(*this, 0, 0, length);
            else // amount of elements doesn't fit in 16-bit unsigned integer type
                Impl<uint32_t, B>::call(*this, 0, 0, length);
        }

        ~BucketsortImpl()
        {
            delete[] copy;
            delete[] values_parts;
            if(length <= std::numeric_limits<uint16_t>::max())
                for(auto &bucket : buckets)
                    delete[] bucket.buckets_16;
            else
                for(auto &bucket : buckets)
                    delete[] bucket.buckets_32;
        }

    private:
        // B = uint8_t if it's only the last byte left to check
        // B = uint16_t otherwise
        template<typename E, typename B>
        struct Impl<E, B, false>
        {
            static void call(BucketsortImpl &this_ref, unsigned dbytes_pos, std::size_t index_start, std::size_t index_end)
            {
                T *start_part = this_ref.start + index_start;
                typename InternalGetter::FuncType get_part = InternalGetter::function;

                if(this_ref.buckets.size() <= dbytes_pos)
                    this_ref.new_buckets<E, B>();
                E *buckets_part = this_ref.buckets[dbytes_pos];
                fill(buckets_part, static_cast<std::size_t>(std::numeric_limits<B>::max()) + 1u);

                for(std::size_t i = index_start;i < index_end;i++)
                    this_ref.values_parts[i] = get_part(this_ref.start[i], dbytes_pos);

                for(std::size_t i = index_start;i < index_end;i++) // count elements in buckets
                    buckets_part[this_ref.values_parts[i]]++;

                for(R i = 1;i;i++) // will stop on overflow to zero
                    buckets_part[i] += buckets_part[i - 1];

                move_array(this_ref.start, this_ref.length, this_ref.copy);
                for(std::size_t i = index_start;i < index_end;i++)
                    start_part[--buckets_part[this_ref.values_parts[i]]] = std::move(this_ref.copy[i]);

                if(++dbytes_pos * 2 >= InternalGetter::bytes)
                    return;
                std::size_t part_end = index_end - index_start;
                auto impl_next = (dbytes_pos * 2 + 1 < InternalGetter::bytes) ?
                            &BucketsortImpl::Impl<E, uint16_t>::call :
                            &BucketsortImpl::Impl<E, uint8_t>::call;

                for(R i = ~static_cast<R>(0);;part_end = buckets_part[i--])
                {
                    if(part_end - buckets_part[i] > 1)
                        impl_next(this_ref, dbytes_pos, index_start + buckets_part[i], index_start + part_end);
                    if(i == 0)
                        break;
                }
            }
        };

        // B = uint8_t if it's only the last byte left to check
        // B = uint16_t otherwise
        template<typename E, typename B>
        struct Impl<E, B, true>
        {
            static void call(BucketsortImpl &this_ref, unsigned dbytes_pos, std::size_t index_start, std::size_t index_end)
            {
                if(index_end - index_start < square_sort_elements_max)
                {
                    // In C++14 we can use compare compare function as template argument only through the use of Getter class
                    // see: https://stackoverflow.com/questions/27410465/template-instantiation-with-constexpr-function-failure
                    Getter::call_sort(this_ref.start + index_start, this_ref.start + index_end);
                    return;
                }

                Impl<E, B, false>::call(this_ref, dbytes_pos, index_start, index_end);
            }
        };
    };

}



template<typename T, typename Getter = DefGetter<T>>
void bucketsort(T *start, T *end)
{
    static_assert(std::is_unsigned<typename Getter::RType>::value, "Getter return value must be of unsigned integer type");

    BucketsortImpl<T, Getter>(start, end);
}
