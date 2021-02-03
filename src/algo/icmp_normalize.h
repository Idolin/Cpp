//
// Created by cynder on 20.02.19.
//

#pragma once

#include "../ImageMagick-7/Magick++.h"
#include "../string/cstr.h"
#include "../template/arraymethods.hpp"

const Magick::Geometry image_geometry = Magick::Geometry("64x64!");

double image_convert_diff(unsigned char *pixels, unsigned char *&pixels_res);

double image_convert_diff(const char *path, unsigned char *&pixels_res);