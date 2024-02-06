#include "PPM_ImageSaver.h"

void PPM_ImageSaver::SaveImage(const Image& image, const std::string& filename) const
{
	std::cout << "Saving image...\n";

	std::ofstream file(filename);

	file << "P3\n";
	file << image.GetWidth() << " " << image.GetHeight() << "\n";
	file << "255\n";

	const std::vector<Color>& pixels = image.GetPixels();

	for (size_t i = 0; i != image.GetWidth() * image.GetHeight(); ++i)
	{
		file << (unsigned)round(pixels[i].r * 255) << " " << (unsigned)round(pixels[i].g * 255) << " " << (unsigned)round(pixels[i].b * 255) << "\n";
	}

	std::cout << "Image saved!\n";
}
