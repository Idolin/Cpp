#include "../other/arraymethods.hpp"
#include "../other/global_static.hpp"
#include "tests_abstract.h"
#include "tests_run.h"

bool run_test(str test_name)
{
    bool run_ok = true;
    bool exists = false;
    vect<_test_pack_class_abstract *> _test_class_packs_defined = \
        *global_static_var<vect<_test_pack_class_abstract *>, test_global_static_id>();
    for(unsigned i = 0; i < _test_class_packs_defined.maxs; i++)
        if(equals(_test_class_packs_defined[i]->test_pack_name, test_name.c_str()) || (test_name == "*"))
        {
            exists = true;
            bool run_local_test_ok = _test_class_packs_defined[i]->test();
            run_ok = run_local_test_ok && run_ok;
        }
    DEBUGIFMSG(!exists, "Test not found: %s", test_name.c_str());
    return run_ok;
}
