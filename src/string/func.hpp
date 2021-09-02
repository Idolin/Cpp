#pragma once

#include <type_traits>

template<typename T>
void *pi(const T &rac, unsigned *result)
{
	ASSERT(result != nullptr);
	if(rac.size() == 0)
        return result;
    result[0] = 0;
    for(unsigned i = 1, k = 0; i < rac.size(); i++)
    {
        while((k > 0) && (rac.at(i) != rac.at(k)))
            k = result[k - 1];
        if(rac.at(i) == rac.at(k))
            k++;
        result[i] = k;
    }
    return result;
}

template<typename T>
unsigned *pi(const T &rac)
{
	unsigned *p = new unsigned[rac.size()];
    pi(rac, p);
	return p;
}

template<typename C, typename T, typename I = unsigned>
inline void pi_step(const C &rac, const unsigned *pi, const T letter, I &current_pi)
{
	while(current_pi > 0 && letter != rac.at(current_pi))
		current_pi = pi[current_pi];
	if(letter == rac.at(current_pi))
		current_pi++;
}
