#include "test_def.h"

vect<test_namespace::_test_class_abstract*> *test_classes_main_sequence;

namespace test_namespace
{
    comma_split_iterator::comma_split_iterator(const char *s): s(s), next(0)
    {}

    comma_split_iterator::comma_split_iterator(const comma_split_iterator& otr): s(otr.s), next(otr.next)
    {}

    comma_split_iterator::comma_split_iterator(comma_split_iterator&& otr): s(otr.s), next(otr.next)
    {
        otr.s = "";
    }

    comma_split_iterator& comma_split_iterator::operator=(const comma_split_iterator& otr)
    {
        s = otr.s;
        next = otr.next;
        return *this;
    }

    comma_split_iterator& comma_split_iterator::operator=(comma_split_iterator&& otr)
    {
        if(this != &otr)
        {
            s = otr.s;
            otr.s = "";
            next = otr.next;
        }
        return *this;
    }

    bool comma_split_iterator::operator==(const comma_split_iterator& otr) const
    {
        return (s == otr.s);
    }

    bool comma_split_iterator::operator!=(const comma_split_iterator& otr) const
    {
        return !(*this == otr);
    }

    comma_split_iterator& comma_split_iterator::operator++()
    {
        if(s[0] != '\0')
        {
            if(next == 0)
                get_next();
            s += next;
            if(s[0])
                s++;
            next = 0;
            while(*s == ' ')
                s++;
        }
        return *this;
    }

    comma_split_iterator comma_split_iterator::operator++(int)
    {
        comma_split_iterator copy = *this;
        ++*this;
        return copy;
    }

    cstr comma_split_iterator::operator*() const
    {
        if(next == 0)
            get_next();
        return cstr(s).substr(0, next);
    }
    
    comma_split_iterator::operator bool() const
    {
        return s[0] != '\0';
    }
    
    void comma_split_iterator::get_next() const
    {
        unsigned brackets_amount = 0;
        char quote = '\0';
        while(s[next])
        {
            if(quote)
            {
                if(s[next] == quote)
                    quote = '\0';
            }
            elif(s[next] == '(' || s[next] == '[' || s[next] == '<' || s[next] == '{')
                brackets_amount++;
            elif(s[next] == ')' || s[next] == ']' || s[next] == '>' || s[next] == '}')
                brackets_amount--;
            elif(brackets_amount == 0 && s[next] == ',')
                return;
            next++;
        }
    }
}
