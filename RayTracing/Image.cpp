#include "Image.h"

Image::Image() :
    size{}
{
}

Image::Image(glm::uvec2 size) :
    size{ size }, pixels(size.x * size.y)
{
}

Image::Image(glm::uvec2 size, std::initializer_list<glm::vec3> pixels) :
    size{ size }, pixels{ pixels }
{
    assert(size.x * size.y == pixels.size());
}

glm::vec3& Image::operator[](size_t n)
{
    return pixels[n];
}

const glm::vec3& Image::operator[](size_t n) const
{
    return pixels[n];
}

const std::vector<glm::vec3>& Image::GetPixels() const
{
    return pixels;
}

std::vector<glm::vec3>& Image::GetPixels()
{
    return pixels;
}

void Image::SetColor(const glm::vec3& color)
{
    std::fill(std::execution::par_unseq, pixels.begin(), pixels.end(), color);
}

glm::uint Image::GetWidth() const
{
    return size.x;
}

glm::uint Image::GetHeight() const
{
    return size.y;
}

glm::uvec2 Image::GetSize() const
{
    return size;
}
