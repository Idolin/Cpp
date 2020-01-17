//
// Created by cynder on 20.02.19.
//

#include <sys/stat.h>
#include <cstdio>
#include <dirent.h>
#include <exception>
#include "../string/str.h"
#include "../other/defdef.h"
#include "../template/arraymethods.hpp"
#include "../algo/icmp_normalize.h"
#include "../debug/def_debug.h"
#include "../other/pam_writer.h"

bool is_picture(const str &name)
{
    static const str extensions[] = {".jpeg", ".jpg", ".png", ".bmp", ".xmp", ".svg", ".gif", ".xcf"};
    for(const str& ext : extensions)
        if(name.endswith(ext))
            return true;
    return false;
}

unsigned char file_type(const str &name, ino_t &inode)  //0 - not exists, 1 - regular file, 2 - directory, 3 - something else
{
    struct stat buffer; //NOLINT (will be initialized in stat)
    if(stat(name.c_str(), &buffer))
        return 0;
    else
    {
        inode = buffer.st_ino;
        if(buffer.st_mode & S_IFREG)
            return 1;
        elif(buffer.st_mode & S_IFDIR)
            return 2;
        else
            return 3;
    }
}

inline unsigned char file_type(const str &name)
{
    ino_t ignored;
    return file_type(name, ignored);
}

void write_picture(const char *file_name, unsigned char *pixels, unsigned width, unsigned height,
                   const char *comment = nullptr)
{
    FILE *img = fopen(file_name, "wb");
    ASSERT(img != NULL, "Can't write to file: %s", file_name);
    fputs("P5\n", img);
    if(comment)
        fprintf(img, "#%s\n", comment);
    fprintf(img, "%u %u\n255\n", width, height);
    fwrite(pixels, width * height, 1, img);
    fclose(img);
}

int main(int argc, char **argv)
{
    if(argc != 3 && argc != 2)
    {
        fprintf(stderr, "Expected 1 or 2 arguments\nUsage: %s <path_to_image> [<path_to_save>]\n", argv[0]);
        return 1;
    }
    if(c_str_equals(argv[1], "-h") || c_str_equals(argv[1], "--help"))
    {
        printf("icmp_diff - program for creating images thumbnails useful for image comparison\n"
               "Usage: %s <path_to_image> [<path_to_save>]\n"
               "\tby default: <path_to_save> = <path_to_image>.pgm\n"
               "\tpath_to_save can point to a directory but a full path\n");
        return 0;
    }
    unsigned char type = file_type(argv[1]);
    bool ok = false;
    if(!(type))
        fprintf(stderr, "File not found: %s\n", argv[1]);
    elif(type != 1)
        fprintf(stderr, "Not a file: %s\n", argv[1]);
    elif(!is_picture(argv[1]))
        fprintf(stderr, "File doesn't seems to be a picture: %s\n", argv[1]);
    else
        ok = true;
    if(!ok)
        return 1;
    const char *write_to = nullptr;
    if(argc == 3)
    {
        type = file_type(argv[2]);
        if(type > 2)
        {
            fprintf(stderr, "Can't rewrite file(not a regular file): %s\n", argv[2]);
            return 1;
        }
        if(type < 2)
            write_to = argv[2];
    }
    if(write_to == nullptr)
    {
        write_to = argv[1];
        unsigned last_dot = 0, last_dir = 0, i = 0;
        while(write_to[i])
        {
            if(write_to[i] == '.')
                last_dot = i;
            elif(write_to[i] == '/')
                last_dir = i;
            i++;
        }
        str write_path;
        if(argc == 3)
            write_path = argv[2];
        else
            last_dir = 0;
        if(last_dot > last_dir)
            write_path += str(argv[1]).substr(last_dir, last_dot);
        else
            write_path += str(argv[1]).substr(last_dir);
        write_path += ".pgm";
        write_to = write_path;
    }
    try
    {
        unsigned char *pixels = nullptr;
        double coeff = image_convert_diff(argv[1], pixels);

        str comment("Coefficient:");
        comment += std::to_string(coeff);
        PAMImageWriter writer(PAMImageWriter::keep_exact(write_to), image_geometry.width(), image_geometry.height());
        writer.set_comment(comment);
        writer.write(pixels);
        delete[] pixels;
    }
    catch(std::exception &exception)
    {
        printf("\rException occurred while converting image: %s\n", argv[1]);
        printf("what(): %s\n", exception.what());
        return 1;
    }
    return 0;
}
