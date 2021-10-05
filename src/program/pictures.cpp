#include <functional>
#include <math.h>
#include "../other/rand.h"
#include "../string/str_func.h"
#include "../template/arraymethods.hpp"
#include "../other/pam_writer.h"

struct pixel
{
	unsigned short red, green, blue;
	
	pixel(double r, double g, double b): red(static_cast<unsigned short>(r)),
		green(static_cast<unsigned short>(g)), blue(static_cast<unsigned short>(b))
	{}
};

template<bool centered=true, bool normalized = true, bool require_angle=true, unsigned iterations=1>
void make_img(cstr name, unsigned width, unsigned height, const std::function<const pixel(double, double, double, double, unsigned)> &f)
{
	PAMImageWriter im(name, width, height, PAMImageWriter::colors::RGB, 65535, false);
	unsigned short *img = new unsigned short[width * height * 3];
	fill(img, width * height * 3);
	for(unsigned iter = 1;iter <= iterations;iter++)
	{
		printf("%u/%u\n", iter, iterations);
		fflush(stdout);
		for(unsigned i = 0;i < height;i++)
			for(unsigned j = 0;j < width;j++)
			{
				double y = i;
				double x = j;
				y /= height;
				x /= width;
				if(centered)
				{
					x -= .5;
					y -= .5;
				}
				double a = 0, r = sqrt(x * x + y * y);
				if(r > 0.000001)
				{
					double xn = x / r;
					double yn = y / r;
					if(normalized)
						x = xn, y = yn;
					if(require_angle)
					{
						a = acos(xn);
						if(y < 0)
							a = 2 * M_PI - a;
					}
				}
				const pixel p = f(x, y, r, a, iter);
				img[(i * width + j) * 3] += p.red;
				img[(i * width + j) * 3 + 1] += p.green;
				img[(i * width + j) * 3 + 2] += p.blue;
			}
	}
	im.write(reinterpret_cast<unsigned char*>(img));
	delete[] img;
}

const pixel waves_1f(double x, double y, double r, double, unsigned iter)
{
	x = abs(x);
	y = abs(y);
	r *= r * iter;
	double v = abs(sin(y - r + acos(y / r) + asin(x / r))) * 2560;
	return pixel(0, 0, v);
}

const pixel waves_2f(double, double, double r, double a, unsigned)
{
	double v = abs(sin(a + r)) * 655360;
	return pixel(0, 0, v);
}

const pixel spiral_1f(double, double, double r, double a, unsigned)
{
	double v = abs(pow(sin(r * 30 + a * 10), 2)) * 32000;
	return pixel(0, 0, v);
}

const pixel spiral_2f(double, double, double r, double a, unsigned iter)
{
	double v = abs(pow(sin(r * iter + a * 10 + r * r * 10), 2)) * 32000;
	return pixel(0, 0, v);
}

const pixel waves_3f(double, double, double r, double a, unsigned)
{
	double rem = 2 * M_PI / 5;
	a = fmod(a, rem) / rem;
	a = abs(a - .5) * 2;
	double v = abs(pow(cos(a * 500 * r), 10)) * 320000;
	return pixel(0, 0, v);
}

const pixel spots_f(double x, double y, double, double, unsigned)
{
	return pixel(x * x * 65536, y * y * 65536, x * y * 65536);
}

const pixel arcs_f(double x, double y, double, double, unsigned)
{
	double s = (x * x + y * y) * 65536;
	double a = x * y * 65536;
	return pixel(s, s + a, s + 2 * a);
}

const pixel arcs_2f(double x, double y, double, double, unsigned)
{
	x = abs(x);
	y = abs(y);
	return pixel(0, 0, tan(x * y * 655.36));
}

const pixel arcs_3f(double x, double y, double, double, unsigned)
{
	x = abs(x) * 40;
	y = abs(y) * 40;
	return pixel(0, 0, sin(abs(x * x + y * y) * 100) + 100 * tan(fmod(x * y, x + y + 1)));
}

const pixel arcs_4f(double x, double y, double, double, unsigned)
{
	x = abs(x) * 100;
	y = abs(y) * 100;
	return pixel(0, 0, sin(abs(x * x + y * y) * 100) + 100 * tan(fmod(x * y, x + y + 1)));
}

const pixel arcs_5f(double x, double y, double, double, unsigned iter)
{
	x = abs(x) * 100;
	y = abs(y) * 100;
	return pixel(0, 0, sin(abs(x * x + y * y) * 100) + (iter) * tan(fmod(x * y, 10)));
}

const pixel spots_2f(double x, double y, double r, double, unsigned)
{
	return pixel(0, 0, (cos(x * 10 + r * r) + sin(y * 10 - r * r)) * 65536);
}

const pixel spiral_3f(double, double, double r, double a, unsigned)
{
	return pixel(0, 0, (sin(r * 4.5 + a * 3)) * 65536);
}

const pixel spiral_4f(double x, double y, double r, double a, unsigned)
{
	x = abs(x);
	y = abs(y);
	return pixel(0, 0, abs(tan(a - r)) * 32568);
}

const pixel border_f(double x, double y, double r, double a, unsigned)
{
	x *= 100;
	y *= 20;
	return pixel(0, 0, abs(cos(x + r) + sin(y - r)) * 32568);
}

const pixel circle_f(double, double, double r, double, unsigned)
{
	return pixel(0, 0, abs(tan(r * 30)) * 32568);
}

const pixel waves_4f(double x, double y, double r, double, unsigned)
{
	x = abs(x);
	y = abs(y);
	r *= 30;
	x *= 10;
	y *= 10;
	return pixel(0, 0, abs(cos(x * y + r) / sin(x * y + r)) * 32568);
}

const pixel circle_2f(double x, double y, double r, double, unsigned iter)
{
	x = abs(x);
	y = abs(y);
	x *= 10;
	y *= 10;
	return pixel(0, 0, abs(cos(x + y + iter) + sin(x + y + iter) + tan(x + y)) * 655360);
}

auto waves_1 = std::bind(make_img<true, false, false, 255>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, waves_1f);

auto waves_2 = std::bind(make_img<true, false, true, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, waves_2f);

auto spiral_1 = std::bind(make_img<true, false, true, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, spiral_1f);

auto spiral_2 = std::bind(make_img<true, false, true, 255>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, spiral_2f);

auto waves_3 = std::bind(make_img<true, false, true, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, waves_3f);

auto spots = std::bind(make_img<false, false, false, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, spots_f);

auto arcs = std::bind(make_img<false, false, false, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, arcs_f);

auto arcs2 = std::bind(make_img<true, false, false, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, arcs_2f);

auto arcs3 = std::bind(make_img<true, false, false, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, arcs_3f);

auto arcs4 = std::bind(make_img<true, false, false, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, arcs_4f);

auto arcs5 = std::bind(make_img<true, false, false, 255>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, arcs_5f);

auto spots2 = std::bind(make_img<true, false, true, 255>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, spots_2f);

auto spiral3 = std::bind(make_img<true, true, true, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, spiral_3f);

auto spiral4 = std::bind(make_img<true, true, true, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, spiral_4f);

auto border = std::bind(make_img<false, false, false, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, border_f);

auto circle = std::bind(make_img<true, false, false, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, circle_f);

auto waves_4 = std::bind(make_img<true, false, false, 1>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, waves_4f);

auto circle2 = std::bind(make_img<true, false, false, 255>, 
								   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, circle_2f);


int main()
{
	circle2("./imstr", 400, 400);
}

