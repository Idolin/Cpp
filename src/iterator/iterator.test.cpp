#include "../debug/test_def.h"
#include "iterator.hpp"
#include "iterator_check.hpp"

#include <cstddef>
#include <iterator>

TEST_PACK(iterator)
{
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
}
