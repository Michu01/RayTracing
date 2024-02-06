#include "PPM_ImageSaver.h"

void PPM_ImageSaver::SaveImage(const Image& image, const std::string& filename) const
{
	std::cout << "Saving image...\n";

	std::ofstream file(filename);

	glm::uvec2 size = image.GetSize();

	file << "P3\n";
	file << size.x << " " << size.y << "\n";
	file << "255\n";

	const std::vector<glm::vec3>& pixels = image.GetPixels();

	for (size_t i = 0; i != (size_t)size.x * size.y; ++i)
	{
		file << (unsigned)round(pixels[i].r * 255) << " " << (unsigned)round(pixels[i].g * 255) << " " << (unsigned)round(pixels[i].b * 255) << "\n";
	}

	std::cout << "Image saved!\n";
}
