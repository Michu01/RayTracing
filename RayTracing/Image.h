#pragma once

#include <vector>
#include <cassert>
#include <execution>

#include <glm/glm.hpp>

class Image
{
private:
	glm::uvec2 size;
	std::vector<glm::vec3> pixels;

public:
	Image();

	Image(glm::uvec2 size);

	Image(glm::uvec2 size, std::initializer_list<glm::vec3> pixels);

	template<class Container>
	Image(glm::uvec2 size, const Container& container);

	glm::vec3& operator[](size_t n);

	const glm::vec3& operator[](size_t n) const;

	const std::vector<glm::vec3>& GetPixels() const;

	std::vector<glm::vec3>& GetPixels();

	void SetColor(const glm::vec3& color);

	glm::uint GetWidth() const;

	glm::uint GetHeight() const;

	glm::uvec2 GetSize() const;
};

template<class Container>
inline Image::Image(glm::uvec2 size, const Container& container) :
	size{ size }, pixels(container.cbegin(), container.cend())
{
}
