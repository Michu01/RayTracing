#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "Image.h"

class PPM_ImageSaver
{
public:
	void SaveImage(const Image& image, const std::string& filename) const;
};

