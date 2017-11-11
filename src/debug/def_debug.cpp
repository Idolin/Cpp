//
// Created by cynder on 20.10.17.
//
#include "def_debug.h"

assert_failed::assert_failed(const std::string &s)
{
    message = s;
}

const char *assert_failed::what() const throw()
{
    return message.c_str();
}