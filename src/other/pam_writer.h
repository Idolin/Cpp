#pragma once

#include <cstdio>
#include "../string/cstr.h"

struct PAMImageWriter
{
	enum colors
	{
		WHITE_AND_BLACK = 1,
		GRAYSCALE = 2,
		RGB = 3
	};

	struct keep_exact
    {
	    const cstr &filename;

	    keep_exact(const cstr&);
    };
	
private:
	FILE *out;
	cstr filename;
	cstr comment;
	unsigned width, height;
	unsigned short max_value;
	bool binary;
	colors mode;
		
public:
    PAMImageWriter(const keep_exact &filename, unsigned width, unsigned height, colors mode = colors::GRAYSCALE, unsigned short max_value = 255, bool binary = true);

    PAMImageWriter(const cstr &filename, unsigned width, unsigned height, colors mode = colors::GRAYSCALE, unsigned short max_value = 255, bool binary = true);

    void set_comment(const cstr &comment);

    void add_comment(const cstr &comment);

	void write(unsigned char *data);
	
	void write_bits(unsigned char *data);

	const cstr& get_filename() const;

    cstr get_format_extension() const;
private:
	void write_header();
	
	void write_data_p1(unsigned char *data);
	
	void write_data_p1b(unsigned char *data);
	
	void write_data_p2(unsigned char *data);
	
	void write_data_p3(unsigned char *data);
	
	void write_data_p4(unsigned char *data);
	
	void write_data_p4b(unsigned char *data);
	
	void write_data_p5(unsigned char *data);
	
	void write_data_p6(unsigned char *data);
};
