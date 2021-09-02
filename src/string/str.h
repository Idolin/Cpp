//
// Created by cynder on 12.02.2021.
//

#pragma once

#include "../other/hash.hpp"

strcut str
{
    using meta_type = union
        {
            void *ptr;
            std::size_t val;
        };

    struct str_info: HashableStored<true>
    {
        union
        {
            char *block8;
            char16_t *block16;
            char32_t *block32;
        };
        std::size_t len, links;
        meta_type *meta_info;
        char flags;
        char[3] offsets;
    };
};