//
// Created by cynder on 16.11.2020.
//

#include <cstdio>
#include <cmath>
#include <exception>
#include <sys/stat.h>
#include "../ImageMagick-7/Magick++.h"

unsigned char file_type(const char *name, ino_t &inode)  //0 - not exists, 1 - regular file, 2 - directory, 3 - something else
{
    struct stat buffer; //NOLINT (will be initialized in stat)
    if(stat(name, &buffer))
        return 0;
    else
    {
        inode = buffer.st_ino;
        if(static_cast<unsigned>(buffer.st_mode) & static_cast<unsigned>(S_IFREG))
            return 1;
        else if(static_cast<unsigned>(buffer.st_mode) & static_cast<unsigned>(S_IFDIR))
            return 2;
        else
            return 3;
    }
}

inline unsigned char file_type(const char *name)
{
    ino_t ignored;
    return file_type(name, ignored);
}

inline unsigned value_diff(unsigned char a, unsigned char b)
{
    return (a > b) ? a - b : b - a;
}

inline unsigned get_diff(unsigned char *pixels, size_t width, size_t x, size_t y, size_t x2, size_t y2)
{
    unsigned result = value_diff(pixels[3 * (y * width + x)], pixels[3 * (y2 * width + x2)]) +
           value_diff(pixels[3 * (y * width + x) + 1], pixels[3 * (y2 * width + x2) + 1]) +
           value_diff(pixels[3 * (y * width + x) + 2], pixels[3 * (y2 * width + x2) + 2]);
    if(result < 24)
        return 0;
    return result >> 3u;
}

int main(int argc, char **argv)
{
    Magick::InitializeMagick(nullptr);

    if(argc <= 1)
    {
        fprintf(stderr, "Expected path to the image\n");
        return 1;
    }
    if(argc > 2)
    {
        fprintf(stderr, "Too many arguments (expected exactly 1)\n");
        return 1;
    }

    char *file_path = argv[1];
    auto type = file_type(file_path);
    switch(type)
    {
        case 0:
            fprintf(stderr, "No such file: %s\n", file_path);
            return 1;
        case 1:
            break;
        default:
            fprintf(stderr, "Not a file: %s\n", file_path);
            return 1;
    }

    std::size_t height, width;
    unsigned long long diff_metric = 0;

    try
    {
        Magick::Image image;
        image.read(file_path);
        image.colorSpace(MagickCore::RGBColorspace);
        image.depth(8);
//        image.scale(Magick::Geometry(512, 512));

        width = image.columns(), height = image.rows();
        if(height <= 2 || width <= 2)
        {
            fprintf(stderr, "Too small image: %zu * %zu\n", width, height);
            return 1;
        }

        auto *pixels = new unsigned char[3 * width * height];
        image.write(0, 0, width, height, "RGB", Magick::CharPixel, pixels);

        for(std::size_t y = 1;y < height - 1;y++)
            for(std::size_t x = 1 + (y & 1u);x < width - 1;x += 2)
                diff_metric +=
                        get_diff(pixels, width, x, y, x - 1, y) +
                        get_diff(pixels, width, x, y, x + 1, y) +
                        get_diff(pixels, width, x, y, x, y - 1) +
                        get_diff(pixels, width, x, y, x, y + 1);

        delete[] pixels;
    }
    catch(std::exception &ex)
    {
        fprintf(stderr, "Error while reading file: %s\n", file_path);
        fprintf(stderr, "What: %s\n", ex.what());
        return 1;
    }
    catch(...)
    {
        fprintf(stderr, "Error while reading file: %s\n", file_path);
        return 1;
    }

    unsigned long long size_coeff = (height - 2) * (width - 2) >> 1u;
    diff_metric = static_cast<unsigned long long>(lround(static_cast<long double>(diff_metric) * (510L * 510L) /
                                                                 static_cast<long double>(size_coeff)));
    printf("%llu\n", diff_metric);
    return 0;
}