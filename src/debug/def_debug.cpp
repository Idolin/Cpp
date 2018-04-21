//
// Created by cynder on 20.10.17.
//
#include "def_debug.h"

assert_failed::assert_failed(const char *s, int err_code): message(s), error_code(err_code)
{}

const char *assert_failed::what() const noexcept
{
    return message;
}

int assert_failed::err_code() const noexcept
{
    return error_code;
}