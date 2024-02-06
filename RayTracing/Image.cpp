#include "Image.h"

Image::Image()
{
}

Image::Image(SizeU size) :
    size{ size }, pixels(size.width * size.height)
{
}

Image::Image(SizeU size, std::initializer_list<Color> pixels) :
    size{ size }, pixels{ pixels }
{
    assert(size.width * size.height == pixels.size());
}

Color& Image::operator[](size_t n)
{
    return pixels[n];
}

const Color& Image::operator[](size_t n) const
{
    return pixels[n];
}

const std::vector<Color>& Image::GetPixels() const
{
    return pixels;
}

std::vector<Color>& Image::GetPixels()
{
    return pixels;
}

void Image::SetColor(const Color& color)
{
    std::fill(std::execution::par_unseq, pixels.begin(), pixels.end(), color);
}

size_t Image::GetWidth() const
{
    return size.width;
}

size_t Image::GetHeight() const
{
    return size.height;
}

SizeU Image::GetSize() const
{
    return size;
}
