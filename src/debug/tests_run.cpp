#include "../template/arraymethods.hpp"
#include "../other/singleton.hpp"
#include "../other/term.h"
#include "tests_abstract.h"
#include "tests_run.h"

bool run_test(str test_name, str subtest_mask)
{
    bool run_ok = true;
    unsigned run_test_packs = 0;
    vect<_test_pack_class_abstract *> _test_class_packs_defined = \
        g_static::global_static_var<vect<_test_pack_class_abstract *>, g_static::test_global_static_id>();
    vect<_test_pack_class_abstract *> failed_packs;
    for(unsigned i = 0; i < _test_class_packs_defined.size(); i++)
        if((test_name == _test_class_packs_defined[i]->test_pack_name) || (test_name == "*"))
        {
            run_test_packs++;
            bool run_local_test_ok = _test_class_packs_defined[i]->test(subtest_mask);
            run_ok = run_local_test_ok && run_ok;
            if(!run_local_test_ok)
                failed_packs.push(_test_class_packs_defined[i]);
        }
    if(run_test_packs == 0)
        color_fprintf(term_color::RED, DEBUG_OUTPUT_STREAM, "Test not found: %s\n", test_name.c_str());
    if(run_test_packs > 1)
    {
        if(failed_packs.size() > 0)
        {
            set_term_color(term_color::RED, DEBUG_OUTPUT_STREAM);
            if(failed_packs.size() > 1)
                fprintf(DEBUG_OUTPUT_STREAM, "%lu test packs failed:\n", failed_packs.size());
            else
                fputs("1 test pack failed:\n", DEBUG_OUTPUT_STREAM);
            for(_test_pack_class_abstract *test_pack : failed_packs)
            {
                fprintf(DEBUG_OUTPUT_STREAM, "\t%s\n", test_pack->test_pack_name);
                test_pack->print_failed();
            }
        }
        else
            color_fprintf(term_color::GREEN, DEBUG_OUTPUT_STREAM, "All %u test packs succeeded", run_test_packs);
        set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
    }
    return run_ok;
}
