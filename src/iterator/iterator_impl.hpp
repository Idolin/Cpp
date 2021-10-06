#pragma once

#include "../template/type_tags.hpp"
#include "../template/typemethods.hpp"
#include "../other/common.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace iterator_impl_def
{
    namespace
    {

        template<typename It, typename Enable = void>
        struct get_def_value
        {
            static_assert(has_dereference_operator_v<It>, "Iterator must be dereferencable");

            typedef std::remove_const_t<std::remove_reference_t<decltype(*std::declval<It>())>> type;
        };

        template<typename It>
        struct get_def_value<It, std::enable_if_t<has_value_type_typedef_v<It>>>
        {
            typedef typename It::value_type type;
        };


        template<typename It, typename Enable = void>
        struct get_def_reference
        {
            static_assert(has_dereference_operator_v<It>, "Iterator must be dereferencable");

            typedef decltype(*std::declval<It>()) type;
        };

        template<typename It>
        struct get_def_reference<It, std::enable_if_t<has_reference_typedef_v<It>>>
        {
            typedef typename It::reference type;
        };

        template<typename It>
        struct get_def_const_reference // used only for input_iterator
        {
        private:
            typedef typename get_def_reference<It>::type reference_t; // reference_t - return type of dereference operator*()

        public:
            // adds const to referred type if it's not an rvalue_reference(&&)
            typedef std::conditional_t<std::is_rvalue_reference<reference_t>::value,
                reference_t,
                add_const_ignore_reference_t<reference_t>> type;
        };


        template<typename It, typename Enable = void>
        struct get_def_pointer
        {
            static_assert(has_dereference_operator_v<It>, "Iterator must be dereferencable");

            typedef std::add_pointer_t<std::remove_reference_t<decltype(*std::declval<It>())>> type;
        };

        template<typename It>
        struct get_def_pointer<It, std::enable_if_t<!has_pointer_typedef_v<It> && has_arrow_operator_v<It>>>
        {
            typedef decltype(std::declval<It>().operator->()) type;
        };

        template<typename It>
        struct get_def_pointer<It, std::enable_if_t<has_pointer_typedef_v<It>>>
        {
            typedef typename It::pointer type;
        };

        template<typename It>
        struct get_def_const_pointer // used only for input_iterator
        {
            // adds const to referred type
            typedef add_const_ignore_reference_and_pointer_t<typename get_def_pointer<It>::type> type;
        };


        template<typename It, bool is_random_access_iterator = false, typename Enable = void>
        struct get_def_difference
        {
            typedef std::ptrdiff_t type;
        };

        template<typename It, bool is_random_access_iterator>
        struct get_def_difference<It, is_random_access_iterator,
                std::enable_if_t<!has_difference_type_typedef_v<It> && has_subtraction_operator_v<It>>>
        {
        private:
            static constexpr bool f = !is_random_access_iterator || is_subtractable_v<const It>;
            static_assert(f,
                "Random access iterator It: this subtract expression must be valid: const It - const It");
            static constexpr bool f2 = std::is_integral<decltype(std::declval<It>() - std::declval<It>())>::value;
            static_assert(!f || f2,
                "Iterator subtraction must return integral type");
            static_assert(!f || !f2 || std::is_same<decltype(std::declval<const It>() - std::declval<const It>()),
                decltype(std::declval<It>() - std::declval<It>())>::value,
                "Iterator subtraction must return same type as const iterator subtraction");

        public:
            typedef decltype(std::declval<It>() - std::declval<It>()) type;
        };

        template<typename It, bool is_random_access_iterator>
        struct get_def_difference<It, is_random_access_iterator, std::enable_if_t<has_difference_type_typedef_v<It>>>
        {
            typedef typename It::difference_type type;
        };


        template<typename It, typename Enable = void>
        struct has_get_index_method
        {
            static constexpr bool value = false;
        };

        template<typename It>
        struct has_get_index_method<It,
                typename std::enable_if_t<std::is_integral<decltype(std::declval<It>().getIndex())>::value>>
        {
            static constexpr bool value = true;
        };

        template<typename It, typename Enable = void>
        struct has_set_index_method
        {
            static constexpr bool value = false;
        };

        template<typename It>
        struct has_set_index_method<It,
                decltype(std::declval<It>().setIndex(std::declval<typename get_def_difference<It>::type>()), void())>
        {
            static constexpr bool value = true;
        };

        template<typename It>
        constexpr bool has_get_index_method_v = has_get_index_method<It>::value;

        template<typename It>
        constexpr bool has_set_index_method_v = has_set_index_method<It>::value;


        template<typename It, typename Enable = void>
        struct has_increment_method
        {
            static constexpr bool value = false;
        };

        template<typename It>
        struct has_increment_method<It, decltype(std::declval<It>().increment(), void())>
        {
            static constexpr bool value = true;
        };


        template<typename It, typename Enable = void>
        struct has_decrement_method
        {
            static constexpr bool value = false;
        };

        template<typename It>
        struct has_decrement_method<It, decltype(std::declval<It>().decrement(), void())>
        {
            static constexpr bool value = true;
        };


        template<typename It>
        constexpr bool has_increment_method_v = has_increment_method<It>::value;

        template<typename It>
        constexpr bool has_decrement_method_v = has_decrement_method<It>::value;


        template<typename It, typename difference_t, typename Enable = void>
        struct has_increment_value_method
        {
            static constexpr bool value = false;
        };

        template<typename It, typename difference_t>
        struct has_increment_value_method<It, difference_t,
                decltype(std::declval<It>().increment(std::declval<difference_t>()), void())>
        {
            static constexpr bool value = true;
        };


        template<typename It, typename difference_t, typename Enable = void>
        struct has_decrement_value_method
        {
            static constexpr bool value = false;
        };

        template<typename It, typename difference_t>
        struct has_decrement_value_method<It, difference_t,
                decltype(std::declval<It>().decrement(std::declval<difference_t>()), void())>
        {
            static constexpr bool value = true;
        };


        template<typename It, typename difference_t>
        constexpr bool has_increment_value_method_v = has_increment_value_method<It, difference_t>::value;

        template<typename It, typename difference_t>
        constexpr bool has_decrement_value_method_v = has_decrement_value_method<It, difference_t>::value;

    }

    template<typename It>
    using get_def_value_t = typename get_def_value<It>::type;

    template<typename It>
    using get_def_reference_t = typename get_def_reference<It>::type;

    template<typename It>
    using get_def_const_reference_t = typename get_def_const_reference<It>::type;

    template<typename It>
    using get_def_pointer_t = typename get_def_pointer<It>::type;

    template<typename It>
    using get_def_const_pointer_t = typename get_def_const_pointer<It>::type;

    template<typename It, bool is_random_access_iterator = false>
    using get_def_difference_t = typename get_def_difference<It, is_random_access_iterator>::type;


    template<typename Impl, typename It, typename Enable = void>
    struct _equalDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f,
            "Input iterator must have either bool operator==(const It&) or non-member function operator==(const It&, const It&) or getIndex() method");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        bool operator==(const It& otr) const
        {
            return (*static_cast<const It*>(this)).Impl::getIndex() == otr.Impl::getIndex();
        }
    };

    template<typename Impl, typename It>
    struct _equalDefault<Impl, It, typename std::enable_if_t<has_equal_operator_v<Impl>>>
    {
    private:
        static constexpr bool f = has_equal_operator_v<const Impl>;
        static_assert(f, "Input iterator It: this comparison expression must be valid: const It == const It");
        static constexpr bool f2 = is_equal_comparable_v<const Impl>;
        static_assert(!f || f2, "Input iterator It: this comparison expression must return value implicitly convertable to bool: const It == const It");
        static_assert(!f || !f2 || std::is_convertible<decltype(std::declval<Impl>() == std::declval<Impl>()), bool>::value,
            "Input iterator It: expression consi It == const It must return type implicitly convertible to bool");
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _notEqualDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f,
            "Input iterator must have either bool operator==(const It&) or non-member function operator==(const It&, const It&) or getIndex() method");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        bool operator!=(const It& otr) const
        {
            return (*static_cast<const It*>(this)).Impl::getIndex() != otr.Impl::getIndex();
        }
    };

    template<typename Impl, typename It>
    struct _notEqualDefault<Impl, It, typename std::enable_if_t<is_equal_comparable_v<Impl> && !is_not_equal_comparable_v<Impl>>>
    {
    private:
        static constexpr bool f = is_equal_comparable_v<const Impl>;
        static_assert(f,
            "Input iterator It: this comparison expression must be valid: const It == const It");
        static_assert(!f || std::is_convertible<decltype(std::declval<Impl>() == std::declval<Impl>()), bool>::value,
            "Input iterator It: expression const It == const It must return type implicitly convertible to bool");

    public:
        bool operator!=(const It& otr) const
        {
            return !((*static_cast<const It*>(this)).Impl == otr.Impl);
        }
    };


    template<typename Impl, typename It>
    struct _notEqualDefault<Impl, It, typename std::enable_if_t<is_not_equal_comparable_v<Impl>>>
    {
        static_assert(is_not_equal_comparable_v<const Impl>,
            "Input iterator It: this comparison expression must be valid: const It != const It");
    };


    template<typename Impl, typename It, typename R, typename Enable = void>
    struct _arrowDefault
    {
    private:
        static_assert(has_dereference_operator_v<R>, "Iterator pointer type better be dereferencable");

        static constexpr bool f = has_dereference_operator_v<Impl>;
        static_assert(f, "Iterator must be dereferncable");
        static_assert(!f || has_dereference_operator_v<const Impl>, "Iterator derefernce operator*() must be const");

    public:
        R operator->() const
        {
            return *((*static_cast<const It*>(this)).Impl);
        }
    };

    template<typename Impl, typename It, typename R>
    struct _arrowDefault<Impl, It, R, typename std::enable_if_t<has_arrow_operator_v<Impl>>>
    {
    private:
        static_assert(has_dereference_operator_v<R>, "Iterator pointer type better be dereferencable");

        static constexpr bool f = has_dereference_operator_v<Impl>;
        static_assert(f, "Iterator must be dereferncable");
        static_assert(!f || has_dereference_operator_v<const Impl>, "Iterator derefernce operator*() must be const");

        static_assert(has_arrow_operator_v<const Impl>, "Input iterator arrow operator operator->() must be const");
        static_assert(std::is_convertible<decltype(std::declval<Impl>().operator->()), R>::value,
            "Input iterator adapter: arrow operator return type must be convertible to pointer type");

    public:
        R operator->() const
        {
            return (*static_cast<const It*>(this)).Impl::operator->();
        }
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _preIncrementDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f && has_set_index_method_v<Impl>,
            "Iterator must have either pre-increment operator++(), or increment() method, or getIndex() and setIndex(integral_type) methods");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        It& operator++()
        {
            (*static_cast<It*>(this)).Impl::setIndex((*static_cast<It*>(this)).Impl::getIndex() + 1);
            return *static_cast<It*>(this);
        }
    };

    template<typename Impl, typename It>
    struct _preIncrementDefault<Impl, It,
           typename std::enable_if_t<has_increment_method_v<Impl> && !has_pre_increment_operator_v<Impl>>>
    {
        It& operator++()
        {
            (*static_cast<It*>(this)).Impl::increment();
            return *static_cast<It*>(this);
        }
    };

    template<typename Impl, typename It>
    struct _preIncrementDefault<Impl, It, typename std::enable_if_t<has_pre_increment_operator_v<Impl>>>
    {
        static_assert(!has_increment_method_v<Impl>,
            "Iterator must have either pre-increment operator++() or increment() method, not both");

        It& operator++()
        {
           return *static_cast<It*>(&((*static_cast<It*>(this)).Impl::operator++()));
        }
    };


    template<typename Impl, typename It, typename value_type, typename Enable = void>
    struct _postIncrementDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f && has_set_index_method_v<Impl>,
            "Iterator must have either pre-increment operator++(), or increment() method, or getIndex() and setIndex(integral_type) methods");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        It operator++(int)
        {
            It copy = *static_cast<It*>(this);
            (*static_cast<It*>(this)).Impl::setIndex((*static_cast<It*>(this)).Impl::getIndex() + 1);
            return copy;
        }
    };

    template<typename Impl, typename It, typename value_type>
    struct _postIncrementDefault<Impl, It, value_type,
            typename std::enable_if_t<has_pre_increment_operator_v<Impl> && !has_post_increment_operator_v<Impl> && !has_increment_method_v<Impl>>>
    {
        It operator++(int)
        {
            It copy = *static_cast<It*>(this);
            (*static_cast<It*>(this)).Impl::operator++();
            return copy;
        }
    };

    template<typename Impl, typename It, typename value_type>
    struct _postIncrementDefault<Impl, It, value_type,
            typename std::enable_if_t<has_increment_method_v<Impl> && !has_post_increment_operator_v<Impl>>>
    {
        It operator++(int)
        {
            It copy = *static_cast<It*>(this);
            (*static_cast<It*>(this)).Impl::increment();
            return copy;
        }
    };

    template<typename Impl, typename It, typename value_type>
    struct _postIncrementDefault<Impl, It, value_type,
            typename std::enable_if_t<has_post_increment_operator_v<Impl>>>
    {
    private:
        // Impl::operator++(int) return type
        typedef decltype(std::declval<Impl>()++) post_increment_t;

        // type iterator adapter(It) will return for operator++(int): either It or Impl::operator++(int) return type
        typedef std::conditional_t<
            is_same_omit_cv_v<post_increment_t, Impl>,
                copy_cv_refernce_t<Impl, It>, post_increment_t> post_increment_ret;

    public:
        static_assert(has_dereference_operator_v<post_increment_t>,
            "Iterator post-increment operator++(int) must return dereferencable object");
        static_assert(std::is_same<value_type, void>::value ||
            is_same_omit_cv_v<post_increment_t, Impl> ||
            std::is_convertible<decltype(*std::declval<post_increment_t>()), value_type>::value,
                "Input iterator post-increment return type must be convertible to value_type after dereferencing");

        post_increment_ret operator++(int)
        {
            return (*static_cast<It*>(this)).Impl::operator++(0);
        }
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _preDecrementDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f && has_set_index_method_v<Impl>,
            "Bidirectional iterator must have either pre-decrement operator--(), or decrement() method, or getIndex() and setIndex(integral_type) methods");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        It& operator--()
        {
            (*static_cast<It*>(this)).Impl::setIndex((*static_cast<It*>(this)).Impl::getIndex() - 1);
            return *static_cast<It*>(this);
        }
    };

    template<typename Impl, typename It>
    struct _preDecrementDefault<Impl, It,
            typename std::enable_if_t<has_decrement_method_v<Impl> && !has_pre_decrement_operator_v<Impl>>>
    {
        It& operator--()
        {
            (*static_cast<It*>(this)).Impl::decrement();
            return *static_cast<It*>(this);
        }
    };

    template<typename Impl, typename It>
    struct _preDecrementDefault<Impl, It, typename std::enable_if_t<has_pre_decrement_operator_v<Impl>>>
    {
        static_assert(!has_decrement_method_v<Impl>,
            "Bidirectional iterator must have either pre-decrement operator--() or decrement() method, not both");

        It& operator--()
        {
           return *static_cast<It*>(&((*static_cast<It*>(this)).Impl::operator--()));
        }
    };


    template<typename Impl, typename It, typename reference_t, typename Enable = void>
    struct _postDecrementDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f && has_set_index_method_v<Impl>,
            "Bidirectional iterator must have either pre-decrement operator--(), or decrement() method, or getIndex() and setIndex(integral_type) methods");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        It operator--(int)
        {
            It copy = *static_cast<It*>(this);
            (*static_cast<It*>(this)).Impl::setIndex((*static_cast<It*>(this)).Impl::getIndex() - 1);
            return copy;
        }
    };

    template<typename Impl, typename It, typename reference_t>
    struct _postDecrementDefault<Impl, It, reference_t,
            typename std::enable_if_t<has_pre_decrement_operator_v<Impl> && !has_decrement_method_v<Impl> && !has_post_decrement_operator_v<Impl>>>
    {
        It operator--(int)
        {
            It copy = *static_cast<It*>(this);
            (*static_cast<It*>(this)).Impl::operator--();
            return copy;
        }
    };

    template<typename Impl, typename It, typename reference_t>
    struct _postDecrementDefault<Impl, It, reference_t,
            typename std::enable_if_t<has_decrement_method_v<Impl> && !has_post_decrement_operator_v<Impl>>>
    {
        It operator--(int)
        {
            It copy = *static_cast<It*>(this);
            (*static_cast<It*>(this)).Impl::decrement();
            return copy;
        }
    };

    template<typename Impl, typename It, typename reference_t>
    struct _postDecrementDefault<Impl, It, reference_t,
            typename std::enable_if_t<has_post_decrement_operator_v<Impl>>>
    {
    private:
        template<bool has_const_dereference_operator, typename T> // T is convertible to reference_t
        struct ReferenceWrapper;

        template<typename T>
        struct ReferenceWrapper<false, T>: public T
        {
            ReferenceWrapper(const T& otr): T(otr)
            {}

            // this constructor will be used in case T don't have copy constructor (but have move constructor)
            ReferenceWrapper(T&& otr): T(std::move(otr))
            {}

            reference_t operator*()
            {
                return *((*this).T);
            }
        };

        template<typename T>
        struct ReferenceWrapper<true, T>: public T
        {
            ReferenceWrapper(const T& otr): T(otr)
            {}

            // this constructor will be used in case T don't have copy constructor (but have move constructor)
            ReferenceWrapper(T&& otr): T(std::move(otr))
            {}

            reference_t operator*() const
            {
                return *as_const((*this).T);
            }

            reference_t operator*()
            {
                return *((*this).T);
            }
        };
        // Impl::operator--(int) return type
        typedef decltype(std::declval<Impl>()--) post_decrement_t;

        static_assert(has_dereference_operator_v<post_decrement_t>,
            "Bidirectional iterator post-decrement--(int) operator must return dereferencable object");
        static_assert(std::is_convertible<post_decrement_t, const Impl&>::value,
            "Bidirectional iterator It post-decrement return type must be convertible to const It&");

        typedef decltype(*std::declval<post_decrement_t>()) post_decrement_deref_t;

        static_assert(std::is_convertible<post_decrement_deref_t, reference_t>::value,
            "Bidirectional iterator adapter: post-decrement return type after dereferencing must be convertible refernece type "
                "(Bidirectional iterator post-decrement return type after dereferencing must be exactly refernece type)");

        /*
         * Bidirectional iterator adapter(It) will return for operator--(int): [post_decrement_t = Impl::operator--(int) return type]
         *  - It (with same cv and reference qualifiers as post_decrement_t), if post_decrement_t == Impl (without cv and reference qualifiers)
         *  - post_decrement_t, if *post_decrement_t == iterator reference type
         *  - wrapper<post_decrement_t>, if *post_decrement_t convertible to iterator reference type
         */
        typedef std::conditional_t<
            is_same_omit_cv_v<post_decrement_t, Impl>,
                copy_cv_refernce_t<Impl, It>,
                std::conditional_t<std::is_same<decltype(*std::declval<post_decrement_t>()), reference_t>::value,
                    post_decrement_t,
                    copy_cv_refernce_t<Impl, ReferenceWrapper<
                        has_dereference_operator_v<add_const_ignore_reference_t<post_decrement_t>>,
                        std::remove_cv_t<std::remove_reference_t<post_decrement_t>>>>>> post_decrement_ret;

    public:
        post_decrement_ret operator--(int)
        {
            return (*static_cast<It*>(this)).Impl::operator--(0);
        }
    };


    template<typename Impl, typename It, typename difference_t, typename Enable = void>
    struct _additionAssignmentDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f && has_set_index_method_v<Impl>,
            "Random access iterator must have either addition operator+=(integral_type), or increment(integral_type) method, "
            "or getIndex() and setIndex(integral_type) methods");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        It& operator+=(difference_t value)
        {
            (*static_cast<It*>(this)).Impl::setIndex((*static_cast<It*>(this)).Impl::getIndex() + value);
            return *static_cast<It*>(this);
        }
    };

    template<typename Impl, typename It, typename difference_t>
    struct _additionAssignmentDefault<Impl, It, difference_t,
           typename std::enable_if_t<has_increment_value_method_v<Impl, difference_t> && !has_addition_operator_v<Impl, difference_t>>>
    {
        It& operator+=(difference_t value)
        {
            (*static_cast<It*>(this)).Impl::increment(value);
            return *static_cast<It*>(this);
        }
    };

    template<typename Impl, typename It, typename difference_t>
    struct _additionAssignmentDefault<Impl, It, difference_t, typename std::enable_if_t<has_addition_operator_v<Impl, difference_t>>>
    {
        static_assert(!has_increment_value_method_v<Impl, difference_t>,
            "Iterator must have either addition operator+=(integral_type) or increment(integral_type) method, not both");

        It& operator+=(difference_t value)
        {
           return *static_cast<It*>(&((*static_cast<It*>(this)).Impl::operator+=(value)));
        }
    };


    template<typename Impl, typename It, typename difference_t, typename Enable = void>
    struct _subtractionAssignmentDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f && has_set_index_method_v<Impl>,
            "Random access iterator must have either subtraction operator-=(integral_type), or decrement(integral_type) method, "
            "or getIndex() and setIndex(integral_type) methods");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        It& operator-=(difference_t value)
        {
            (*static_cast<It*>(this)).Impl::setIndex((*static_cast<It*>(this)).Impl::getIndex() - value);
            return *static_cast<It*>(this);
        }
    };

    template<typename Impl, typename It, typename difference_t>
    struct _subtractionAssignmentDefault<Impl, It, difference_t,
           typename std::enable_if_t<has_decrement_value_method_v<Impl, difference_t> && !has_subtraction_operator_v<Impl, difference_t>>>
    {
        It& operator-=(difference_t value)
        {
            (*static_cast<It*>(this)).Impl::decrement(value);
            return *static_cast<It*>(this);
        }
    };

    template<typename Impl, typename It, typename difference_t>
    struct _subtractionAssignmentDefault<Impl, It, difference_t, typename std::enable_if_t<has_subtraction_operator_v<Impl, difference_t>>>
    {
        static_assert(!has_decrement_value_method_v<Impl, difference_t>,
            "Iterator must have either subtraction operator-=(integral_type) or decrement(integral_type) method, not both");

        It& operator-=(difference_t value)
        {
           return *static_cast<It*>(&((*static_cast<It*>(this)).Impl::operator-=(value)));
        }
    };


    template<typename Impl, typename It, typename difference_t, typename Enable = void>
    struct _iteratorSubtractionDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f && has_set_index_method_v<Impl>,
            "Random access iterator must have either subtraction operator-(const It&), or getIndex() and setIndex(integral_type) methods");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        difference_t operator-(const It& otr) const
        {
            return (*static_cast<const It*>(this)).Impl::getIndex() - otr.Impl::getIndex();
        }
    };

    template<typename Impl, typename It, typename difference_t>
    struct _iteratorSubtractionDefault<Impl, It, difference_t,
           typename std::enable_if_t<has_subscript_operator_v<Impl, Impl>>>
    {
    private:
        static_assert(has_subtraction_operator_v<const Impl, const Impl>,
            "Random access iterator It: this subtract operation must be valid: const It - const It");

        static constexpr bool f = std::is_integral<decltype(std::declval<Impl>() - std::declval<Impl>())>::value;
        static_assert(f,
            "Iterator subtraction must return integral type");
        static_assert(!f || std::is_same<decltype(std::declval<const Impl>() - std::declval<const Impl>()),
            decltype(std::declval<Impl>() - std::declval<Impl>())>::value,
            "Iterator subtraction must return same type as const iterator subtraction");

    public:
        difference_t operator-(const It& otr) const
        {
            return static_cast<difference_t>((*static_cast<const It*>(this)).Impl - otr.Impl);
        }
    };


    template<typename Impl, typename It, typename reference_t, typename difference_t, typename Enable = void>
    struct _subscriptDefault
    {
        reference_t operator[](difference_t index) const
        {
            return *((*static_cast<const It*>(this)).operator+(index));
        }
    };

    template<typename Impl, typename It, typename reference_t, typename difference_t>
    struct _subscriptDefault<Impl, It, reference_t, difference_t,
            typename std::enable_if_t<has_subscript_operator_v<Impl>>>
    {
        static_assert(has_subscript_operator_v<const Impl>,
            "Random access iterator subscript operator[](integral_type) must have const version");
        static_assert(std::is_convertible<decltype(std::declval<Impl>().operator[](std::declval<difference_t>())), reference_t>::value,
            "Random access iterator subscript operator[](integral_type) return type must be convertible to reference type");
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _comparisonLessDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f,
            "Random access iterator must have either bool operator<(const It&) or non-member function operator<(const It&, const It&) or getIndex() method");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        bool operator<(const It& otr) const
        {
            return (*static_cast<const It*>(this)).Impl::getIndex() < otr.Impl::getIndex();
        }
    };

    template<typename Impl, typename It>
    struct _comparisonLessDefault<Impl, It, typename std::enable_if_t<has_less_operator_v<Impl>>>
    {
    private:
        static_assert(has_less_operator_v<const Impl>,
            "Random access iterator It: this comparison expression must be valid: const It < const It");

        static constexpr bool f = is_less_comparable_v<Impl>;
        static_assert(f,
            "Random access iterator It: this comparison expression must return value contextually convertibel to bool: It < It");
        static_assert(!f || is_less_comparable_v<const Impl>,
            "Random access iterator It: this comparison expression must return value contextually convertibel to bool: const It < const It");
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _comparisonLessOrEqualDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f,
            "Random access iterator must have either bool operator<(const It&) or non-member function operator<(const It&, const It&) or getIndex() method");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        bool operator<=(const It& otr) const
        {
            return (*static_cast<const It*>(this)).Impl::getIndex() <= otr.Impl::getIndex();
        }
    };

    template<typename Impl, typename It>
    struct _comparisonLessOrEqualDefault<Impl, It, typename std::enable_if_t<is_less_comparable_v<Impl>>>
    {
        static_assert(is_less_comparable_v<const Impl>,
            "Random access iterator It: this comparison expression must be valid: const It < const It");

        bool operator<=(const It& otr) const
        {
            return ((*static_cast<const It*>(this)).Impl < otr.Impl) ||
                    ((*static_cast<const It*>(this)) == otr);
        }
    };

    template<typename Impl, typename It>
    struct _comparisonLessOrEqualDefault<Impl, It, typename std::enable_if_t<is_less_or_equal_comparable_v<Impl>>>
    {
        static_assert(is_less_or_equal_comparable_v<const Impl>,
            "Random access iterator It: this comparison expression must be valid: const It <= const It");
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _comparisonGreaterDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f,
            "Random access iterator must have either bool operator<(const It&) or non-member function operator<(const It&, const It&) or getIndex() method");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        bool operator>(const It& otr) const
        {
            return (*static_cast<const It*>(this)).Impl::getIndex() > otr.Impl::getIndex();
        }
    };

    template<typename Impl, typename It>
    struct _comparisonGreaterDefault<Impl, It, typename std::enable_if_t<is_less_comparable_v<Impl>>>
    {
        static_assert(is_less_comparable_v<const Impl>,
            "Random access iterator It: this comparison expression must be valid: const It < const It");

        bool operator>(const It& otr) const
        {
            return !((*static_cast<const It*>(this)).Impl < otr.Impl) && ((*static_cast<const It*>(this)) != otr);
        }
    };

    template<typename Impl, typename It>
    struct _comparisonGreaterDefault<Impl, It, typename std::enable_if_t<is_greater_comparable_v<Impl>>>
    {
        static_assert(is_greater_comparable_v<const Impl>,
            "Random access iterator It: this comparison expression must be valid: const It > const It");
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _comparisonGreaterOrEqualDefault
    {
    private:
        static constexpr bool f = has_get_index_method_v<Impl>;
        static_assert(f,
            "Random access iterator must have either bool operator<(const It&) or non-member function operator<(const It&, const It&) or getIndex() method");
        static_assert(!f || has_get_index_method_v<const Impl>, "Iterator's getIndex() method must be const");

    public:
        bool operator>=(const It& otr) const
        {
            return (*static_cast<const It*>(this)).Impl::getIndex() >= otr.Impl::getIndex();
        }
    };

    template<typename Impl, typename It>
    struct _comparisonGreaterOrEqualDefault<Impl, It, typename std::enable_if_t<is_less_comparable_v<Impl>>>
    {
        static_assert(is_less_comparable_v<const Impl>,
            "Random access iterator It: this comparison expression must be valid: const It < const It");

        bool operator>=(const It& otr) const
        {
            return !((*static_cast<const It*>(this)).Impl < otr.Impl);
        }
    };

    template<typename Impl, typename It>
    struct _comparisonGreaterOrEqualDefault<Impl, It, typename std::enable_if_t<is_greater_or_equal_comparable_v<Impl>>>
    {
        static_assert(is_greater_or_equal_comparable_v<const Impl>,
            "Random access iterator It: this comparison expression must be valid: const It >= const It");
    };


    template<typename Impl, typename It, typename difference_t, typename Enable = void>
    struct _subtractionDefault
    {
        It operator-(difference_t value) const
        {
            It copy = *static_cast<const It*>(this);
            return copy.operator-=(value);
        }
    };

    template<typename Impl, typename It, typename difference_t>
    struct _subtractionDefault<Impl, It, difference_t, typename std::enable_if_t<has_subtraction_operator_v<Impl, difference_t>>>
    {
    private:
        static_assert(has_subtraction_operator_v<const Impl, difference_t>,
            "Random access iterator It: this subtraction expression must be valid: const It - integral_type");

        static constexpr bool f = std::is_same<Impl, decltype(std::declval<Impl>() - std::declval<difference_t>())>::value;
        static_assert(f,
            "Random access iterator It: this subtraction expression return type must be exactly It: It - integral_type");
        static_assert(!f || std::is_same<Impl, decltype(std::declval<const Impl>() - std::declval<difference_t>())>::value,
            "Random access iterator It: this subtraction expression return type must be exactly It: const It - integral_type");

    public:
        It operator-(difference_t value) const
        {
            return (*static_cast<const It*>(this)).Impl - value;
        }
    };

    template<typename Impl, typename It, typename difference_t, typename Enable = void>
    struct _additionDefault
    {
        It operator+(difference_t value) const
        {
            It copy = *static_cast<const It*>(this);
            return copy.operator+=(value);
        }
    };

    template<typename Impl, typename It, typename difference_t>
    struct _additionDefault<Impl, It, difference_t, typename std::enable_if_t<has_addition_operator_v<Impl, difference_t>>>
    {
    private:
        static_assert(has_addition_operator_v<const Impl, difference_t>,
            "Random access iterator It: this addition expression must be valid: const It + integral_type");

        static constexpr bool f = std::is_same<Impl, decltype(std::declval<Impl>() + std::declval<difference_t>())>::value;
        static_assert(f,
            "Random access iterator It: this addition expression return type must be exactly It: It + integral_type");
        static_assert(!f || std::is_same<Impl, decltype(std::declval<const Impl>() + std::declval<difference_t>())>::value,
            "Random access iterator It: this addition expression return type must be exactly It: const It + integral_type");

        It operator+(difference_t value) const
        {
            return (*static_cast<const It*>(this)).Impl + value;
        }
    };

}
