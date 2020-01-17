#include <algorithm>
#include <array>
#include <functional>
#include <type_traits>
#include "../template/compile_time.hpp"
#include "../string/func.hpp"
#include "../template/arraymethods.hpp"
#include "../other/pam_writer.h"

template<typename T, unsigned char check_length = 2, bool show_progress = true, unsigned short show_images_of_length = 0>
std::pair<unsigned long long, T> unique_length_near(std::function<T(T)> next, T initial = 0, 
									  unsigned long long check_to = std::numeric_limits<T>::max() * _min(check_length, 3))
{
	ASSERT(check_length > 0);
	std::array<T, check_length> first = get_compile_time_array<T, check_length>(
		[&, initial](std::size_t i) {
			static T now = initial;
			if(i)
				now = next(now);
			return now;
		});
	unsigned *pi_first = check_length > 1 ? pi(first) : nullptr;
	std::array<T, check_length> some = first;
	unsigned *pi_some = check_length > 1 ? new unsigned[check_length] : nullptr;
	if(check_length > 1)
		_copy(pi_first, check_length, pi_some);
	unsigned long long to = 128, i = check_length;
	T now = first.back();
	unsigned char first_index = 0, some_index = 0;
	T *array_pbm = show_images_of_length > 0 ? new T[show_images_of_length] : nullptr;
	unsigned char pbm_pos = 0;
	PAMImageWriter wr("./seq.pbm", sizeof(T) * 8, 128, PAMImageWriter::colors::WHITE_AND_BLACK);
	for(;i < check_to;i++)
	{
		if(show_images_of_length > 0)
			if(pbm_pos < show_images_of_length)
			{
				array_pbm[pbm_pos] = now;
				if(++pbm_pos == 128)
					wr.write_bits(reinterpret_cast<unsigned char*>(array_pbm));
			}
		if((i & 0xfffffffu) == 0)
		{
			if(show_progress)
				fprintf(stderr, "%llu.%llu\n", i >> 28, i);
			pbm_pos = 0;
		}
		
		now = next(now);
		if(check_length > 1)
		{
			pi_step(first, pi_first, now, first_index);
			if(first_index == check_length)
				break;
		}
		elif(now == first.at(first_index))
			break;
		if(i >= to)
		{
			some[i - to] = now;
			if(i - to == check_length - 1)
			{
				to *= 2;
				if(check_length > 1)
					pi(some, pi_some);
				some_index = 0;
			}
		}
		else
		{
			if(check_length > 1)
			{
				pi_step(some, pi_some, now, some_index);
				if(some_index == check_length)
					break;
			}
			elif(now == some.at(some_index))
				break;
		}
	}
	delete[] pi_first;
	delete[] pi_some;
	delete[] array_pbm;
	return std::make_pair(i, now);
}
