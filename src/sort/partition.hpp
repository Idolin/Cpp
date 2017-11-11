#pragma once

template<typename T>
T *partition(T *start, T *end, T &df, unsigned &count, T &min)
{
    T *sp = start;
    if(count > 0)
    {
        if(df == min)
            df++;
        sp += count;
        count = 0;
        T *ep = sp;
        while(ep < end)
            if(_notmore(*ep, df))
            {
                if(*ep == min)
                {
                    while(*start == min)
                        start++;
                    _swap(start++, ep);
                } else
                {
                    if(_more(df, *ep))
                        _swap(sp++, ep);
                    else
                        count++;
                    ep++;
                }
            } else
                ep++;
    } else
    {
        for(T *ep = sp; ep < end; ep++)
            if(_notmore(*ep, df))
            {
                if(_more(df, *ep))
                    _swap(sp++, ep);
                else
                    count++;
            }
    }
    return sp;
}
