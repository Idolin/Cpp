#include "../debug/test_def.h"
#include "iterator.hpp"
#include "iterator_check.hpp"

#include <cstddef>
#include <iterator>
#include <memory>

TEST_PACK(iterator)
{
    namespace
    {

        struct SomeInputIterator
        {
            struct IntWrapper
            {
                int x;

                int operator*()
                {
                    return x;
                }
            };

            typedef std::input_iterator_tag iterator_category;
            typedef const int value_type;
            typedef const int& reference;
            typedef const int* pointer;
            typedef std::ptrdiff_t difference_type;

            int r = 0;

            SomeInputIterator(int x): r(x)
            {}

            const int& operator*() const
            {
                return r;
            }

            const int* operator->() const
            {
                return &r;
            }

            SomeInputIterator& operator++()
            {
                r++;
                return *this;
            }

            IntWrapper operator++(int)
            {
                IntWrapper copy{r};
                r++;
                return copy;
            }

            bool operator==(const SomeInputIterator& otr) const
            {
                return r == otr.r;
            }

            bool operator!=(const SomeInputIterator& otr) const
            {
                return r != otr.r;
            }
        };

        struct InputIteratorImpl
        {
            int r = 0;

            const int& operator*() const
            {
                return r;
            }

            void increment()
            {
                r++;
            }

            bool operator==(const InputIteratorImpl& otr) const
            {
                return r == otr.r;
            }
        };

        struct GetSetIndexIteratorImpl
        {
            std::shared_ptr<int> a {new int[]{3, 5, 7, 9, 11, 13, 15}, std::default_delete<int[]>()};
            int index = 0;

            int& operator*() const
            {
                return a.get()[index];
            }

            int getIndex() const
            {
                return index;
            }

            void setIndex(int new_index)
            {
                index = new_index;
            }
        };

        struct GetIndexIncrementDecrementMethodIteratorImpl
        {
            std::shared_ptr<int> collatz {new int[]{7, 22, 11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1}, std::default_delete<int[]>()};
            int index = 0;

            int& operator*() const
            {
                return collatz.get()[index];
            }

            int getIndex() const
            {
                return index;
            }

            void increment(int value = 1)
            {
                index += value;
            }

            void decrement(int value = 1)
            {
                index -= value;
            }
        };

        struct MixedIteratorImpl
        {
            struct BoolWrapper
            {
                bool v;

                BoolWrapper(bool f): v(f)
                {}

                explicit operator bool() const
                {
                    return v;
                }
            };

            std::shared_ptr<int> not_random {new int[]{0, 1, 2, 0, 0, 4, 2, 1, 2, 4, 2, 5, 8, 0, 1, 4, 5, 5, 4, 4, 1, 4, 4, 4, 0, 8, 0, 8,
                                        2, 5, 0, 2, 2, 1, 4, 2, 2, 5, 1, 5, 8, 4, 5, 5}, std::default_delete<int[]>()};
            int index = 0;

            int& operator*() const
            {
                return not_random.get()[index];
            }

            MixedIteratorImpl& operator++()
            {
                index++;
                return *this;
            }

            void increment(int value)
            {
                index += value;
            }

            void decrement()
            {
                index--;
            }

            MixedIteratorImpl& operator-=(int value)
            {
                index -= value;
                return *this;
            }

            bool operator==(const MixedIteratorImpl& otr) const
            {
                return index == otr.index;
            }

            bool operator!=(const MixedIteratorImpl& otr) const
            {
                return index % 3 != otr.index % 3;
            }

            bool operator<(const MixedIteratorImpl& otr) const
            {
                return index < otr.index;
            }

            BoolWrapper operator<=(const MixedIteratorImpl& otr) const
            {
                return (index < otr.index) && (index % 3 != otr.index % 3);
            }
        };

        template<typename T>
        struct TemplateIndexIteratorWithFunction
        {
            T index = 0;

            const T& operator*() const
            {
                return index;
            }

            T getIndex() const
            {
                return index;
            }

            void setIndex(T new_index)
            {
                index = new_index;
            }

            int foo(int p) const
            {
                return (index + p) % 42;
            }
        };

    }

    TEST(iterator_checks_empty)
    {
        struct Empty
        {};

        EXPECT_FALSE(is_valid_stl_iterator<Empty>::value);
        EXPECT_FALSE(is_valid_stl_iterator_v<Empty>);
    }

    TEST(iterator_checks_pointer)
    {
        EXPECT_TRUE(is_valid_stl_iterator_v<int*>);
        EXPECT_FALSE(is_valid_stl_iterator_v<void*>);
        EXPECT_TRUE(is_valid_stl_iterator_v<void**>);
        EXPECT_TRUE(is_valid_stl_iterator_v<const int*>);
        EXPECT_TRUE(is_valid_stl_input_iterator_v<const int*>);
        EXPECT_FALSE(is_valid_stl_output_iterator_v<const int*>);
    }

    TEST(output_iterator)
    {
        struct OutputIteratorWithoutTypedefs
        {
            mutable int v;
            int k = 0;

            OutputIteratorWithoutTypedefs() = delete;

            int& operator*() const
            {
                return v;
            }

            OutputIteratorWithoutTypedefs& operator++()
            {
                k++;
                return *this;
            }

            OutputIteratorWithoutTypedefs operator++(int)
            {
                OutputIteratorWithoutTypedefs temp = *this;
                k++;
                return temp;
            }
        };

        struct OutputIteratorWithTypedefs
        {
            typedef std::output_iterator_tag iterator_category;
            typedef void value_type;
            typedef void reference;
            typedef void pointer;
            typedef void difference_type;

            mutable int v;
            int k = 0;

            OutputIteratorWithTypedefs() = delete;

            int& operator*() const
            {
                return v;
            }

            OutputIteratorWithTypedefs& operator++()
            {
                k++;
                return *this;
            }

            OutputIteratorWithTypedefs operator++(int)
            {
                OutputIteratorWithTypedefs temp = *this;
                k++;
                return temp;
            }
        };

        using OutputIteratorByAdapter = output_iterator<OutputIteratorWithoutTypedefs>;

        EXPECT_FALSE(is_valid_stl_output_iterator_v<OutputIteratorWithoutTypedefs>);
        EXPECT_FALSE(is_valid_stl_iterator_v<OutputIteratorWithoutTypedefs>);
        EXPECT_FALSE(is_valid_stl_input_iterator_v<OutputIteratorWithoutTypedefs>);

        EXPECT_TRUE(is_valid_stl_output_iterator_v<OutputIteratorWithTypedefs>);
        EXPECT_TRUE(is_valid_stl_iterator_v<OutputIteratorWithTypedefs>);

        EXPECT_TRUE(is_valid_stl_output_iterator_v<OutputIteratorByAdapter>);
        EXPECT_TRUE(is_valid_stl_iterator_v<OutputIteratorByAdapter>);
    }

    TEST(input_iterator)
    {
        struct NotAnInputIterator
        {
            int k;

            int& operator*()
            {
                return k;
            }

            NotAnInputIterator& operator++()
            {
                return *this;
            }

            NotAnInputIterator operator++(int)
            {
                return *this;
            }

            bool operator==(const NotAnInputIterator&) const
            {
                return false;
            }
        };

        struct NotACopyConstructibleInputIterator
        {
            struct IntWrapper
            {
                int x;

                int operator*()
                {
                    return x;
                }
            };

            typedef std::input_iterator_tag iterator_category;
            typedef const int value_type;
            typedef const int& reference;
            typedef const int* pointer;
            typedef std::ptrdiff_t difference_type;

            int r = 0;

            NotACopyConstructibleInputIterator(int x): r(x)
            {}

            NotACopyConstructibleInputIterator(const NotACopyConstructibleInputIterator&) = delete;

            const int& operator*() const
            {
                return r;
            }

            const int* operator->() const
            {
                return &r;
            }

            NotACopyConstructibleInputIterator& operator++()
            {
                r++;
                return *this;
            }

            IntWrapper operator++(int)
            {
                IntWrapper copy{r};
                r++;
                return copy;
            }

            bool operator==(const NotACopyConstructibleInputIterator& otr) const
            {
                return r == otr.r;
            }

            bool operator!=(const NotACopyConstructibleInputIterator& otr) const
            {
                return r != otr.r;
            }
        };

        using InputIterator = input_iterator<InputIteratorImpl>;


        EXPECT_FALSE(is_valid_stl_iterator_v<NotAnInputIterator>);
        EXPECT_FALSE(is_valid_stl_input_iterator_v<NotAnInputIterator>);
        EXPECT_FALSE(is_valid_stl_output_iterator_v<NotAnInputIterator>);

        EXPECT_TRUE(is_valid_stl_iterator_v<InputIterator>);
        EXPECT_TRUE(is_valid_stl_input_iterator_v<InputIterator>);
        EXPECT_FALSE(is_valid_stl_output_iterator_v<InputIterator>);

        EXPECT_FALSE(is_valid_stl_iterator_v<NotACopyConstructibleInputIterator>);
        EXPECT_FALSE(is_valid_stl_input_iterator_v<NotACopyConstructibleInputIterator>);
        EXPECT_FALSE(is_valid_stl_output_iterator_v<NotACopyConstructibleInputIterator>);

        EXPECT_TRUE(is_valid_stl_iterator_v<SomeInputIterator>);
        EXPECT_TRUE(is_valid_stl_input_iterator_v<SomeInputIterator>);
        EXPECT_FALSE(is_valid_stl_output_iterator_v<SomeInputIterator>);

        EXPECT_TRUE(is_valid_stl_iterator_v<input_iterator<SomeInputIterator>>);
        EXPECT_TRUE(is_valid_stl_input_iterator_v<input_iterator<SomeInputIterator>>);
        EXPECT_FALSE(is_valid_stl_output_iterator_v<input_iterator<SomeInputIterator>>);

        InputIterator it;

        EXPECT_EQ(*it, 0);
        EXPECT_EQ(*++it, 1);
        EXPECT_EQ(*it++, 1);
        EXPECT_EQ(*it, 2);

        EXPECT_TRUE(it == it);

        const InputIterator it_copy = it;
        InputIterator it_copy2 = it;
        EXPECT_FALSE(it_copy == ++it);

        EXPECT_EQ(*it, 3);
        it = it_copy2;
        EXPECT_EQ(*it, 2);
    }

    TEST(forward_iterator)
    {
        using ForwardIterator = forward_iterator<GetSetIndexIteratorImpl>;
        using ForwardIterator2 = forward_iterator<GetIndexIncrementDecrementMethodIteratorImpl>;
        using ForwardIterator3 = forward_iterator<MixedIteratorImpl>;

        EXPECT_TRUE(is_valid_stl_iterator_v<ForwardIterator>);
        EXPECT_TRUE(is_valid_stl_forward_iterator_v<ForwardIterator>);
        EXPECT_TRUE(is_valid_stl_iterator_v<ForwardIterator2>);
        EXPECT_TRUE(is_valid_stl_forward_iterator_v<ForwardIterator2>);
        EXPECT_TRUE(is_valid_stl_iterator_v<ForwardIterator3>);
        EXPECT_TRUE(is_valid_stl_forward_iterator_v<ForwardIterator3>);

        ForwardIterator it_gs = ForwardIterator();

        EXPECT_EQ(*it_gs, 3);
        EXPECT_EQ(*it_gs++, 3);
        EXPECT_EQ(*it_gs, 5);
        EXPECT_EQ(*++it_gs, 7);

        ForwardIterator it_gs_copy = it_gs;
        EXPECT_EQ(*it_gs_copy, 7);
        it_gs++;
        EXPECT_EQ(*it_gs, 9);
        EXPECT_EQ(*it_gs_copy, 7);
        EXPECT_EQ(*++it_gs, 11);
        EXPECT_EQ(*it_gs_copy++, 7);
        EXPECT_EQ(*it_gs_copy, 9);
        EXPECT_EQ(*it_gs, 11);
        EXPECT_NE(it_gs, it_gs_copy);
        EXPECT_EQ(it_gs, ++it_gs_copy);


        ForwardIterator2 it_idm = ForwardIterator2();

        EXPECT_EQ(*it_idm, 7);
        EXPECT_EQ(*it_idm++, 7);
        EXPECT_EQ(*it_idm, 22);
        EXPECT_EQ(*++it_idm, 11);

        ForwardIterator2 it_idm_copy = it_idm;
        EXPECT_EQ(*it_idm_copy, 11);
        it_idm++;
        EXPECT_EQ(*it_idm, 34);
        EXPECT_EQ(*it_idm_copy, 11);
        EXPECT_EQ(*++it_idm, 17);
        EXPECT_EQ(*it_idm_copy++, 11);
        EXPECT_EQ(*it_idm_copy, 34);
        EXPECT_EQ(*it_idm, 17);
        EXPECT_NE(it_idm, it_idm_copy);
        EXPECT_EQ(it_idm, ++it_idm_copy);


        ForwardIterator3 it_m = ForwardIterator3();

        EXPECT_EQ(*it_m, 0);
        EXPECT_EQ(*it_m++, 0);
        EXPECT_EQ(*it_m, 1);
        EXPECT_EQ(*++it_m, 2);

        ForwardIterator3 it_m_copy = it_m;
        EXPECT_EQ(*it_m_copy, 2);
        it_m++;
        EXPECT_EQ(*it_m, 0);
        EXPECT_EQ(*it_m_copy, 2);
        EXPECT_EQ(*++it_m, 0);
        EXPECT_EQ(*++it_m, 4);
        EXPECT_EQ(*it_m_copy++, 2);
        EXPECT_EQ(*it_m_copy, 0);
        EXPECT_EQ(*it_m, 4);
        EXPECT_NE(it_m, it_m_copy);
        EXPECT_EQ(it_m, ++(++it_m_copy));
    }

    TEST(random_access_iterator)
    {
        using TemplateRandomAccessIterator = random_access_iterator<TemplateIndexIteratorWithFunction<short>>;

        TemplateRandomAccessIterator it_a{};
        EXPECT_EQ(*it_a, 0);
        EXPECT_EQ(*it_a++, 0);
        EXPECT_EQ(*++it_a, 2);
        EXPECT_EQ(it_a[7], 9);
        EXPECT_EQ(*(it_a -= -3), 5);
        EXPECT_EQ(*(it_a += -1), 4);
        it_a -= 2;

        TemplateRandomAccessIterator it_b = it_a;
        EXPECT_EQ(*it_b, 2);
        EXPECT_EQ(*(it_a + 9), 11);
        it_b += 4;
        EXPECT_EQ(it_b - it_a, 4);
        EXPECT_TRUE(it_b >= it_a);
        EXPECT_FALSE(it_b == it_a);
        EXPECT_TRUE(it_b > it_a);
        EXPECT_FALSE(it_a >= it_b);
        EXPECT_TRUE(it_a != it_b);
        EXPECT_EQ(it_b + -4, it_a);

        EXPECT_EQ(it_a.foo(41), 1);
        EXPECT_EQ(it_b.foo(41), 5);

        it_a = it_b;
        EXPECT_EQ(it_a + 1, ++it_b);
        EXPECT_TRUE(it_a < it_b);
        EXPECT_TRUE(it_a <= it_b);
        EXPECT_FALSE(it_a++ >= it_b);
        EXPECT_FALSE(it_a < it_b);
        EXPECT_TRUE(it_a <= it_b);
        EXPECT_TRUE(it_a >= it_b);
    }
}
