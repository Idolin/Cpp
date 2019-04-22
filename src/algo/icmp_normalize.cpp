//
// Created by cynder on 20.02.19.
//

#include "icmp_normalize.h"

struct static_array
{
    ~static_array()
    {
        delete[] array;
    }

    unsigned char *get(size_t size)
    {
        if(array == nullptr)
            array = new unsigned char[size];
        return array;
    }

    unsigned char *array = nullptr;
};

static static_array pixels_array = static_array();

double image_convert_diff(unsigned char *pixels, unsigned char *&pixels_res)
{
    if(pixels_res == nullptr)
        pixels_res = new unsigned char[image_geometry.width() * image_geometry.height()];
    for(unsigned y = 1; y < image_geometry.height(); y++)
        {
            unsigned shift = y * image_geometry.width();
            int16_t p1 = static_cast<int16_t>(pixels[shift]),
                    p2 = static_cast<int16_t>(pixels[shift - image_geometry.width()]), p3, p4;
            int16_t p12 = static_cast<int16_t>(abs(p1 - p2)), p34;
            for(unsigned x = 1; x < image_geometry.width(); x++)
            {
                p3 = static_cast<int16_t>(pixels[shift + x]), p4 = static_cast<int16_t>(pixels[shift + x -
                                                                                               image_geometry.width()]);
                p34 = static_cast<int16_t>(abs(p3 - p4));
                pixels_res[shift + x] = static_cast<unsigned char>(( // NOLINT (result is not negative)
                        p12 +
                        abs(p1 - p3) +
                        abs(p2 - p4) +
                        p34
                ) >> 4);
                p1 = p3, p2 = p4, p12 = p34;
            }
        }
        for(size_t i = image_geometry.width(); i < image_geometry.width() * image_geometry.height(); i += image_geometry.width())
            pixels_res[i] = pixels_res[i + 1];
        _copy(pixels_res + image_geometry.width(), image_geometry.width(), pixels_res);
        unsigned sm = _sum<unsigned>(pixels_res, image_geometry.width() * image_geometry.height());
        unsigned char med = static_cast<unsigned char>((sm / (256 * image_geometry.width() * image_geometry.height())) + 1);
        for(unsigned si = 0;si < image_geometry.width() * image_geometry.height();si++)
            if(pixels_res[si] >= med && pixels_res[si] < 255)
                pixels_res[si]++, sm++;
    return static_cast<double>(256 * image_geometry.width() * image_geometry.height()) / (sqrt(sm + image_geometry.width() + image_geometry.height()));
}

double image_convert_diff(const char *path, unsigned char *&pixels_res)
{
    Magick::Image image;
    image.read(path);
    image.type(Magick::GrayscaleType);
    image.depth(8);
    image.scale(image_geometry);
    image.getPixels(0, 0, image_geometry.width(), image_geometry.height());
    unsigned char *pixels = pixels_array.get(image_geometry.width() * image_geometry.height());
    image.writePixels(Magick::GrayQuantum, pixels);
    return image_convert_diff(pixels, pixels_res);
}