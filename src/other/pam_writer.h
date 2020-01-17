#pragma once

#include <cstdio>
#include "../string/str.h"

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
	    const str &filename;

	    keep_exact(const str&);
    };
	
private:
	FILE *out;
	str filename;
	str comment;
	unsigned width, height;
	unsigned short max_value;
	bool binary;
	colors mode;
		
public:
    PAMImageWriter(const keep_exact &filename, unsigned width, unsigned height, colors mode = colors::GRAYSCALE, unsigned short max_value = 255, bool binary = true);

    PAMImageWriter(const str &filename, unsigned width, unsigned height, colors mode = colors::GRAYSCALE, unsigned short max_value = 255, bool binary = true);

    void set_comment(const str &comment);

    void add_comment(const str &comment);

	void write(unsigned char *data);
	
	void write_bits(unsigned char *data);

	const str& get_filename() const;

    str get_format_extension() const;
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
