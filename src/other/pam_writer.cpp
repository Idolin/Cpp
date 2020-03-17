#include "pam_writer.h"

#include "../debug/def_debug.h"

PAMImageWriter::keep_exact::keep_exact(const str &s): filename(s)
{}

PAMImageWriter::PAMImageWriter(const keep_exact &filename, unsigned width, unsigned height, colors mode, unsigned short max_value, bool binary):
    filename(filename.filename), comment(), width(width), height(height), max_value(max_value), binary(binary), mode(mode)
{
    ASSERT(max_value > 0);
    ASSERT(width > 0 && height > 0);
    if(mode == colors::WHITE_AND_BLACK)
        max_value = 1;
}


PAMImageWriter::PAMImageWriter(const str &filename, unsigned width, unsigned height, colors mode, unsigned short max_value, bool binary):
    PAMImageWriter(keep_exact(filename), width, height, mode, max_value, binary)
{
    str ext = str('.') + get_format_extension();
    if(!this->filename.endswith(ext))
        this->filename += ext;
}

void PAMImageWriter::set_comment(const str &comment)
{
    this->comment = comment;
}

void PAMImageWriter::add_comment(const str &comment)
{
    if(this->comment.length() > 0)
        this->comment += '\n';
    this->comment += comment;
}
    
void PAMImageWriter::write(unsigned char *data)
{
    write_header();
    switch(mode)
    {
        case colors::WHITE_AND_BLACK:
            binary ? write_data_p4(data) : write_data_p1(data);
            break;
        case colors::GRAYSCALE:
            binary ? write_data_p5(data) : write_data_p2(data);
            break;
        case colors::RGB:
            binary ? write_data_p6(data) : write_data_p3(data);
            break;
    }
    fclose(out);
}
    
void PAMImageWriter::write_bits(unsigned char *data)
{
    ASSERT(mode == colors::WHITE_AND_BLACK);
    ASSERT((width & 0x7) == 0);
    write_header();
    binary ? write_data_p4b(data) : write_data_p1b(data);
    fclose(out);
}

const str& PAMImageWriter::get_filename() const
{
    return filename;
}

str PAMImageWriter::get_format_extension() const
{
    return mode == colors::WHITE_AND_BLACK ? ".pbm" : (mode == colors::GRAYSCALE ? ".pgm" : ".ppm");
}

void PAMImageWriter::write_header()
{
    out = fopen(filename.c_str(), "wb");
    ASSERT(out != NULL, "Can't write to file: %s", filename.c_str());
    fprintf(out, "P%d\n", mode + (binary ? 3 : 0));
    if(comment)
        for(str comment_line : comment.split())
            fprintf(out, "#%s\n", comment_line.c_str());
    fprintf(out, "%u %u\n", width, height);
    if(mode != colors::WHITE_AND_BLACK)
        fprintf(out, "%hu\n", max_value);
}
    
void PAMImageWriter::write_data_p1(unsigned char *data)
{
    for(unsigned h = 0;h < height;h++)
    {
        fputc('\n', out);
        fputc(data[h * width] ? '1' : '0', out);
        for(unsigned w = 1;w < width;w++)
            fputc(' ', out), fputc(data[h * width + w] ? '1' : '0', out);
    }
}
    
void PAMImageWriter::write_data_p1b(unsigned char *data)
{
    std::size_t index = 0;
    for(unsigned h = 0;h < height;h++)
    {
        char ws = '\n';
        for(unsigned w = 0;w < width;w += 8, index++)
            for(unsigned char b = 0b10000000;b > 0;b >>= 1)
                fputc(ws, out), fputc((data[index] & b) ? '1' : '0', out), ws = ' ';
    }
}
    
void PAMImageWriter::write_data_p2(unsigned char *data)
{
    if(max_value < 256)
        for(unsigned h = 0;h < height;h++)
        {
            fputc('\n', out);
            fprintf(out, "%hhu", data[h * width]);
            for(unsigned w = 1;w < width;w++)
                fprintf(out, " %hhu", data[h * width + w]);
        }
    else
        for(unsigned h = 0;h < height;h++)
        {
            fputc('\n', out);
            fprintf(out, "%hu", reinterpret_cast<unsigned short*>(data)[h * width]);
            for(unsigned w = 1;w < width;w++)
                fprintf(out, " %hu", reinterpret_cast<unsigned short*>(data)[h * width + w]);
        }
}
    
void PAMImageWriter::write_data_p3(unsigned char *data)
{
    std::size_t index = 0;
    if(max_value < 256)
        for(unsigned h = 0;h < height;h++)
        {
            fputc('\n', out);
            fprintf(out, "%hhu %hhu %hhu", data[index], data[index + 1], data[index + 2]);
            index += 3;
            for(unsigned w = 1;w < width;w++, index += 3)
                fprintf(out, "  %hhu %hhu %hhu", data[index], data[index + 1], data[index + 2]);
        }
    else
    {
        unsigned short *data_short = reinterpret_cast<unsigned short*>(data);
        for(unsigned h = 0;h < height;h++)
        {
            fputc('\n', out);
            fprintf(out, "%hu %hu %hu", data_short[index], data_short[index + 1], data_short[index + 2]);
            index += 3;
            for(unsigned w = 1;w < width;w++, index += 3)
                fprintf(out, "  %hu %hu %hu", data_short[index], data_short[index + 1], data_short[index + 2]);
        }
    }
}
    
void PAMImageWriter::write_data_p4(unsigned char *data)
{
    unsigned char byte = (data[0]) ? 1 : 0;
    for(std::size_t i = 1;i <= height * width;i++)
    {
        if((i & 0x7) == 0)
            fputc(byte, out), byte = (data[i]) ? 1 : 0;
        else
            byte = (byte << 1) + ((data[i]) ? 1 : 0);
    }
}
    
void PAMImageWriter::write_data_p4b(unsigned char *data)
{
    fwrite(data, 1, height * width << 3, out);
}
    
void PAMImageWriter::write_data_p5(unsigned char *data)
{
    fwrite(data, (max_value < 256) ? 1 : 2, height * width, out);
}
    
void PAMImageWriter::write_data_p6(unsigned char *data)
{
    fwrite(data, (max_value < 256) ? 1 : 2, height * width * 3, out);
}
