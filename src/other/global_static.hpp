#pragma once

const int default_unit_id = 0;
const int test_global_static_id = 3;

template<class S, int id = default_unit_id>
S *global_static_var()
{
    static S s;
    return &s;
}