#pragma once

namespace
{
    template<typename T, typename Enable = void>
    struct __binary_search_base;

    template<typename T>
    struct __binary_search_base<T, typename std::enable_if<std::is_pointer<T>::value>::type>
    {
        T left_border, right_border, cur;

        __binary_search_base(T left, T right): left_border(left), right_border(right),
                                               cur(middle())
        {}

        T middle() const
        {
            return left_border + (right_border - left_border) / 2;
        }

        int epsilon() const
        {
            return 1;
        }

        void end_search()
        {
            right_border = nullptr;
        }

        bool is_done() const
        {
            return right_border == nullptr;
        }
    };

    template<typename T>
    struct __binary_search_base<T, typename std::enable_if<std::is_integral<T>::value &&
                                                           std::is_unsigned<T>::value>::type>
    {
        T left_border, right_border, cur;

        __binary_search_base(T left, T right): left_border(left), right_border(right),
                                                cur(middle())
        {}

        T middle() const
        {
            return left_border + (right_border - left_border) / 2;
        }

        T epsilon() const
        {
            return 1;
        };

        void end_search()
        {
            left_border = 1;
            right_border = cur = 0;
        }

        bool is_done() const
        {
            return left_border > right_border;
        }
    };

    template<typename T>
    struct __binary_search_base<T, typename std::enable_if<std::is_integral<T>::value &&
                                                           std::is_signed<T>::value>::type>
    {
        T left_border, right_border, cur;

        __binary_search_base(T left, T right): left_border (left), right_border(right),
                                               cur(middle())
        {}

    protected:
        T middle() const
        {
            return left_border / 2 + right_border / 2 + (left_border & right_border & 0b1);
        }

        T epsilon() const
        {
            return 1;
        }

        void end_search()
        {
            left_border = 1;
            right_border = cur = 0;
        }

        bool is_done() const
        {
            return left_border > right_border;
        }
    };

    template<typename T>
    struct __binary_search_base<T, typename std::enable_if<
            std::is_floating_point<T>::value>::type>
    {
        T left_border, right_border, cur;

        __binary_search_base(T left, T right): left_border (left), right_border(right),
                                               cur(middle())
        {}

    protected:
        T middle() const
        {
            return left_border / 2 + right_border / 2;
        }

        T epsilon() const
        {
            return std::numeric_limits<T>::epsilon();
        }

        void end_search()
        {
            left_border = 1;
            right_border = cur = 0;
        }

        bool is_done() const
        {
            return left_border > right_border;
        }
    };
}

template<typename T>
struct binary_search: protected __binary_search_base<T>
{
    binary_search(T left, T right): __binary_search_base<T>(left, right)
    {}

    T current() const
    {
        return this->cur;
    }

    void left()
    {
        if(this->left_border < this->cur)
        {
            this->right_border = this->cur - this->epsilon();
            this->cur = this->middle();
        }
        else
            this->end_search();
    }

    void right()
    {
        if(this->cur < this->right_border)
        {
            this->left_border = this->cur + this->epsilon();
            this->cur = this->middle();
        }
        else
            this->end_search();
    }

    bool done() const
    {
        return this->is_done();
    }
};
