#include "../template/arraymethods.hpp"
#include "../other/singleton.hpp"
#include "../other/term.h"
#include "tests_abstract.h"
#include "tests_run.h"

bool run_test(cstr test_pack_mask, cstr test_mask)
{
    bool run_ok = true;
    unsigned run_test_packs = 0;
    auto _test_class_packs_defined = \
        g_static::global_static_var<vect<test_namespace::_test_pack_class *>, "Test"_gsh, "All test packs"_gsh>();
    vect<test_namespace::_test_pack_class *> failed_packs;
    for(unsigned i = 0; i < _test_class_packs_defined.size(); i++)
        if((test_pack_mask == _test_class_packs_defined[i]->test_pack_name) || (test_pack_mask == "*"))
        {
            run_test_packs++;
            bool run_local_test_ok = _test_class_packs_defined[i]->test(test_mask);
            run_ok = run_local_test_ok && run_ok;
            if(!run_local_test_ok)
                failed_packs.push(_test_class_packs_defined[i]);
        }
    if(run_test_packs == 0)
        color_fprintf(term_color::RED, DEBUG_OUTPUT_STREAM, "Test not found: %s\n", test_pack_mask.c_str());
    if(run_test_packs > 1)
    {
        fputc('\n', DEBUG_OUTPUT_STREAM);
        if(failed_packs.size() > 0)
        {
            set_term_color(term_color::RED, DEBUG_OUTPUT_STREAM);
            fprintf(DEBUG_OUTPUT_STREAM, "%lu/%u test packs failed:\n", failed_packs.size(), run_test_packs);
            for(test_namespace::_test_pack_class *test_pack : failed_packs)
                fprintf(DEBUG_OUTPUT_STREAM, "\t%s\n", test_pack->test_pack_name);
        }
        else
            color_fprintf(term_color::GREEN, DEBUG_OUTPUT_STREAM, "All %u test packs succeeded\n", run_test_packs);
        set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
    }
    return run_ok;
}
