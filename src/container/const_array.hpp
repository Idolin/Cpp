#pragma once

template<typename T>
struct const_array
{
    const T *array;
    bool delete_array;

    const_array(const T *array, bool delete_array = true): array(array),
                                                           delete_array(delete_array)
    {}

    ~const_array()
    {
        if(delete_array)
            delete[] array;
    }

    operator const T*()
    {
        return array;
    }
};
